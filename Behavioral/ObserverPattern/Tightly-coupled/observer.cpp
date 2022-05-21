#include <iostream>
#include <string>
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
    
    // can not make protected the notify because sbj will
    // need it in the main. But in the concObserver , 
    // notif() can be accessible it is not a good idea to
    // have highly coupled dependency like this. 
    // to solve this issue only function pointer can be passed 
    // to concobservers, instead the object.
    //protected: 
    virtual void notify() = 0;

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

    void remove(observerIf* obs)
    {   
        std::vector<observerIf*>::iterator position = std::find(mListObs.begin(), mListObs.end(), obs);
        if (position != mListObs.end())
        {
            mListObs.erase(position);
        }
    }

    protected:
    void notify()
    {
        for(auto s : mListObs)
            s->update();
    }
};

class concObserver : public observerIf
{
    public:
    concObserver(subjectIf* sbj):mSbj(sbj)
    {
                (*mSbj).addObs(this);
    }
    
    void update(){std::cout<<"concObserver is updated"<<std::endl;}
   
    ~concObserver(){
        (*mSbj).remove(this);
    };
    private:
    subjectIf* mSbj;
};


int main()
{
    subjectIf* sbj = new subject;

    concObserver* obs1 = new concObserver(sbj);
    //sbj->addObs(obs1);
    sbj->notify();

    delete sbj;
    std::cout<<"end"<<std::endl;
}