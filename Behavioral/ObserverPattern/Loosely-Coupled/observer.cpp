// Example program
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

class observerIf
{
    public:
    virtual void update() = 0;
    virtual ~observerIf(){};
};

class subjectIf
{
    public:

    virtual void remove(observerIf* obs) = 0;

    virtual void addObs(observerIf* obs) = 0;

    virtual void notify() = 0;

    virtual ~subjectIf(){};

    protected:
    std::vector<observerIf*> mListObs;
};

class subject : public subjectIf
{
    public:
    subject(){}
    void addObs(observerIf* obs)
    {
        mListObs.push_back(obs);
    }
    
    void notify()
    {
        for(auto s : mListObs)
            s->update();
    }

    void remove(observerIf* obs)
    {   
        std::vector<observerIf*>::iterator position = std::find(mListObs.begin(), mListObs.end(), obs);
        if (position != mListObs.end())
        {
            mListObs.erase(position);
        }
    }

    ~subject(){}
};

class concObserver : public observerIf
{
    public:
    concObserver(std::function<void(observerIf*)>& remove,
                      std::function<void(observerIf*)>& add)
    :mCallbackRemove(remove),
    mCallbackAdd(add)
    {
        mCallbackAdd(this);
    }

    void update(){
        std::cout<<"concObserverCall4 is updated"<<std::endl;
    }

    ~concObserver()
    {
        mCallbackRemove(this);
    }

    private:

    std::function<void(observerIf*)>& mCallbackRemove;
    std::function<void(observerIf*)>& mCallbackAdd;

};

int main()
{
    subjectIf* sbj = new subject;
    std::cout<<sbj<<std::endl;

    // callback function to distribute to all observers
    // to remove them from the list
    std::function<void(observerIf*)> removeCallBack
    = std::bind(&subject::remove, dynamic_cast<subject*>(sbj), std::placeholders::_1);
    
    std::function<void(observerIf*)> addCallBack
    = std::bind(&subject::addObs, dynamic_cast<subject*>(sbj), std::placeholders::_1);


    std::cout<<"Create concObserver"<<std::endl;
    observerIf* obsCall4 = 
    new concObserver(removeCallBack, addCallBack);
    sbj->notify();
    
    std::cout<<"delete concObserver"<<std::endl;
    delete obsCall4;
    sbj->notify();


    delete sbj;
    std::cout<<"end"<<std::endl;
}