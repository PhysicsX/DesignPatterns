#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
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
		
    // ownership will not be exposed to client.
	static Singleton* getInstance()
	{
		/*
			If control enters the declaration concurrently while the variable is being initialized, 
			the concurrent execution shall wait for completion of the initialization.
		*/
		static std::unique_ptr<Singleton> instance { Singleton::instanceFactory() };
		return instance.get();
	}

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

int main()
{
	//Singleton* s = new Singleton(); //Error	
    {
	Singleton* p = Singleton::getInstance();
    }
    std::thread([&](){
        Singleton* p2 = Singleton::getInstance();
        p2->setX(1);
		int var = p2->getX();
    }).detach();
    std::thread([&](){
        Singleton* p3 = Singleton::getInstance();
		int var = p3->getX();
    	p3->setX(2);
		var = p3->getX();
    }).detach();
	std::this_thread::sleep_for (std::chrono::seconds(4));
	return 0;
}