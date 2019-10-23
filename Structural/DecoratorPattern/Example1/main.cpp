#include <iostream>
#include <memory>

//Decorator pattern example 
//g++ -std=c++14 main.cpp decorator
class I
{
	public:
		virtual ~I(){};
		virtual void foo()=0;
};

class A : public I
{
	public:
		A(){std::cout<<"A ctor"<<std::endl;}
		~A(){std::cout<<"A dtor"<<std::endl;}
		virtual void foo(){std::cout<<"A";}
};

class D : public I
{
	public:
		D(std::unique_ptr<I> inner)
		{
			std::cout<<"D ctor"<<std::endl;
			m_wrappee = std::move(inner);
		}
		~D(){std::cout<<"D dtor"<<std::endl;}
		virtual void foo(){std::cout<<"D";m_wrappee->foo();}
	private:
		std::unique_ptr<I> m_wrappee;
};

class X : public D
{

	public:
		X(std::unique_ptr<I> core):D(std::move(core)){std::cout<<"X ctor"<<std::endl;}
		~X(){std::cout<<"X dtor"<<std::endl;}
		virtual void foo(){std::cout<<"X";D::foo();}
};

class Y : public D
{
	public:
		Y(std::unique_ptr<I> core):D(std::move(core)){std::cout<<"Y ctor"<<std::endl;}
		~Y(){std::cout<<"Y dtor"<<std::endl;}
		virtual void foo(){std::cout<<"Y";D::foo();}
};

class Z : public D
{
	public:
		Z(std::unique_ptr<I> core):D(std::move(core)){std::cout<<"Z ctor"<<std::endl;}
		~Z(){std::cout<<"Z dtor"<<std::endl;}
		virtual void foo(){std::cout<<"Z";D::foo();}
};

int main()
{
	//I* anX = new X(new A);
	std::unique_ptr<I> mX = std::make_unique<X>(std::make_unique<A>());
	mX->foo();
	std::cout<<std::endl;

	//A ctor
	//D ctor
	//X ctor
	//XDA
	//X dtor
	//D dtor
	//A dtor

	std::unique_ptr<I> mY = std::make_unique<Y>(std::make_unique<X>(std::make_unique<A>()));
	mY->foo();
	std::cout<<std::endl;

	//YDXDA

	std::unique_ptr<I> mZ = std::make_unique<Z>
				(std::make_unique<Y>
				(std::make_unique<X>
				(std::make_unique<A>())));
	mZ->foo();
	std::cout<<std::endl;	

	//ZDYDXDAls

	

	return 0;
}

