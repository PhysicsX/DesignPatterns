// adaptor design pattern example
#include <stdio.h>
#include <iostream>

class socketInterface
{
	public:
		virtual std::string socketType()=0;
		virtual bool power()=0;
};

class homeSocket : public socketInterface
{
	public:
		virtual std::string socketType(){return "Type C";}
		virtual bool power(){return true;}
};

class adaptorInterface
{
	public:
		virtual std::string socketType()=0;
		virtual bool power()=0;
		virtual void plugin(socketInterface* s)=0;
};


class fiveVoltAdaptor : public adaptorInterface
{
	socketInterface* socket = nullptr;
	public:
		virtual void plugin(socketInterface* s)
		{
			socket = s;
		}
		virtual std::string socketType(){return "USB";}
		virtual bool power(){if(socket == nullptr){return false;}return socket->power();}
	
};

class raspberrypi
{
	adaptorInterface* socket = nullptr;

	public:
		void plugin(adaptorInterface* s)
		{
			socket = s;
		}

		void run()
		{
			if(socket->power())
			{
				if(socket->socketType() != "USB")
				{
					std::cout<<"Wrong Socket Type"<<std::endl;
					return;		
				}
				std::cout<<"Raspberry pi is running"<<std::endl;
			}
			else
			{
				std::cout<<"there is no power"<<std::endl;
			}
		}
};



int main()
{

	socketInterface* soc = new homeSocket;
	adaptorInterface* adap = new fiveVoltAdaptor;
	raspberrypi* rasp = new raspberrypi;

	adap->plugin(soc);
	rasp->plugin(adap);
	rasp->run();
	
	return 0;
}





