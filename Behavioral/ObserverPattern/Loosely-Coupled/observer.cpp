#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

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

    // return callback functions for remove and add
    virtual std::function<void(observerIf*)>& removeCallback() = 0;
    virtual std::function<void(observerIf*)>& addObsCallback() = 0;

    virtual ~subjectIf(){};
};

class subject : public subjectIf
{
    public:
    subject():mCallbackRemove{std::bind(&subject::remove, this, std::placeholders::_1)},
              mCallbackAdd{std::bind(&subject::addObs, this, std::placeholders::_1)}{}
    void addObs(observerIf* obs)
    {
        mListObs.push_back(obs);
    }
    
    void notify()
    {   
        const auto& sizeList {mListObs.size()};
        if(0 == sizeList)
        {
            std::cout<<"list is empty"<<std::endl;
        }
        else
        {
            std::cout<<"list has "<<sizeList<<" observer"<<std::endl;
        }

        for(auto s : mListObs)
            s->update();
    }

    std::function<void(observerIf*)>& removeCallback()
    {
        return mCallbackRemove;
    }

    std::function<void(observerIf*)>& addObsCallback()
    {
        return mCallbackAdd;
    }

    void remove(observerIf* obs)
    {   
        std::vector<observerIf*>::iterator position = std::find(mListObs.begin(), mListObs.end(), obs);
        if (position != mListObs.end())
        {
            mListObs.erase(position);
        }
    }

    ~subject()
    {
        std::cout<<"subject is destroyed"<<std::endl;
    }

    std::function<void(observerIf*)> mCallbackRemove;
    std::function<void(observerIf*)> mCallbackAdd;

    private:
    std::vector<observerIf*> mListObs;
};

class concObserver : public observerIf
{
    public:
    concObserver(std::function<void(observerIf*)>& remove,
                 std::function<void(observerIf*)>& add)
    :mCallbackRemove{remove},
    mCallbackAdd(add)
    {
        mCallbackAdd(this);
    }

    void update(){
        std::cout<<"concObserverCall4 is updated"<<std::endl;
    }

    ~concObserver()
    {
        std::cout<<"concObserver is destroyed"<<std::endl;

        mCallbackRemove(this);
    }

    private:

    std::function<void(observerIf*)>& mCallbackRemove;
    std::function<void(observerIf*)>& mCallbackAdd;
};

int main()
{
    // std::shared_ptr<subjectIf> sbjPtr {std::make_shared<subject>()};

    // std::shared_ptr<observerIf> obsPtr { 
    // std::make_shared<concObserver>(sbjPtr->removeCallback(), sbjPtr->addObsCallback())};
        
    // sbjPtr->notify();
    // obsPtr.reset();

    // sbjPtr->notify();

    subjectIf* sbj = new subject;
    std::cout<<sbj<<std::endl;

    std::cout<<"Create concObserver"<<std::endl;
    observerIf* obsCall4 = 
    new concObserver(sbj->removeCallback(), sbj->addObsCallback());
    sbj->notify();
    
    std::cout<<"delete concObserver"<<std::endl;
    delete obsCall4;
    sbj->notify();

    delete sbj;
    std::cout<<"end"<<std::endl;
}