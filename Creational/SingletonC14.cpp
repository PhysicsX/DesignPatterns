#include <iostream>
#include <memory>
#include <thread>
#include <mutex>

// thread safe - no leak possible Singleton Example
// g++ Singleton.cpp -std=c++14 -o singleton

class Singleton
{
	private:
		static std::unique_ptr<Singleton> instance;
		struct _cons {explicit _cons() = default; };
		std::mutex m;

	public:
		int X;
		Singleton(_cons){}
		static std::unique_ptr<Singleton> instanceFactory()
		{
			return std::make_unique<Singleton>(_cons{});
		}
		
	static std::unique_ptr<Singleton>& getInstance();

	void setX(int x)
	{
		std::lock_guard<std::mutex> guard(m);
		X = x;
	}

	int getX()
	{
		std::lock_guard<std::mutex> guard(m);
		return X;
	}

};


std::unique_ptr<Singleton>& Singleton::getInstance()
{
	/*
		If control enters the declaration concurrently while the variable is being initialized, 
		the concurrent execution shall wait for completion of the initialization.
	*/

	static std::unique_ptr<Singleton> instance { Singleton::instanceFactory() };
	return instance;
}

int main()
{

	//std::unique_ptr<Singleton> s = std::make_unique<Singleton>; //Error	
    {
	std::unique_ptr<Singleton> &p = Singleton::getInstance();
    }

    std::thread([&](){
        std::unique_ptr<Singleton> &p2 = Singleton::getInstance();
        p2->setX(1);
		int var = p2->getX();
    }).detach();

    std::thread([&](){
        std::unique_ptr<Singleton> &p3 = Singleton::getInstance();
		int var = p3->getX();

    	p3->setX(2);
		var = p3->getX();
    }).detach();

	std::this_thread::sleep_for (std::chrono::seconds(4));
	return 0;
}
