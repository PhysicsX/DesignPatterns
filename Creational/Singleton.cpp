#include <iostream>
#include <memory>


/*

	Singleton Design Pattern Example with unique_ptr<>
	No leak possible
	g++ Singleton.cpp -std=c++14 -o singleton
	
	Author : ulas dikme
	e-mail : ulasdikme@gmail.com
	website : www.ulasdikme.com

*/

class Singleton
{
	private:
		static std::unique_ptr<Singleton> instance;
		struct _cons {explicit _cons() = default; };

	public:
		int X;
		Singleton(_cons){std::cout<<"const is called"<<std::endl;}
		static std::unique_ptr<Singleton> instanceFactory()
		{
			return std::make_unique<Singleton>(_cons{});
		}

	static std::unique_ptr<Singleton> &getInstance();
	~Singleton(){std::cout<<"Dest is called"<<std::endl;}
	void setX(int x)
	{
		X = x;
	}
	int getX()
	{
		return X;
	}

};

std::unique_ptr<Singleton> Singleton::instance = NULL;

std::unique_ptr<Singleton>& Singleton::getInstance()
{
	if(instance == NULL)
	{
		instance = Singleton::instanceFactory();
	}

	return instance;
}

int main()
{

	//std::unique_ptr<Singleton> s = std::make_unique<Singleton>; //Error	

	std::unique_ptr<Singleton> &p = Singleton::getInstance();
	p->setX(1);
	std::cout<<"X: "<<p->getX()<<std::endl;

	std::unique_ptr<Singleton> &p2 = Singleton::getInstance();
	std::cout<<"X: "<<p2->getX()<<std::endl;

	p2->setX(2);
	std::cout<<"X: "<<p->getX()<<std::endl;

	return 0;
}
