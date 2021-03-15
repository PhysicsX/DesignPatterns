#include <iostream>
#include <algorithm>
#include <list>


class IObserver
{

    public:
        virtual void update() = 0; // pure virtual
};


class ISubject
{

    public:
        virtual void addObs(IObserver* ob) = 0;
        virtual void deleteObs(IObserver* ob) = 0;
        virtual void notifyObs() = 0;

};

int main()
{


    return 0;
}