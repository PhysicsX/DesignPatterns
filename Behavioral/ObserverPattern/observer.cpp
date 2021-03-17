#include <iostream>
#include <algorithm>
#include <list>

//Abstract Observer
class IObserver
{

    public:
        virtual void update(int data) = 0; // pure virtual
};

//Abstract Subject/publisher - Subject will update the clients/observers/subscribers
class ISubject
{

    public:
        virtual void addObs(IObserver* ob) = 0;
        virtual void deleteObs(IObserver* ob) = 0;
        virtual void notifyObs() = 0;

};


//Concrete Subject
class Subject : public ISubject
{  
    public:
        void dataChange(int val)
        {
            m_data = val;
            notifyObs();
        }

        void addObs(IObserver* ob)
        {
            m_obs.push_back(ob);

        }

        void deleteObs(IObserver* ob)
        {
            m_obs.remove(ob);

        }

        void notifyObs()
        {
            std::list<IObserver*>::iterator pos = m_obs.begin();
            while(pos != m_obs.end())
            {
                ((IObserver*)(*pos))->update(m_data);
                ++pos;
            }
        }

    private:
        int m_data;
        std::list<IObserver*> m_obs;

};

//Concrete Observer
class Observer : public IObserver
{
    public:
        Observer(ISubject& a):m_subs(a)
        {
            m_subs.addObs(this);
        }

        void update(int val)
        {
            m_data = val;
            std::cout<<m_data<<std::endl;
        }

    private:
        int m_data;
        ISubject& m_subs;
};

int main()
{
    Subject* subj = new Subject;
    Observer* obs = new Observer(*subj);

    subj->dataChange(21);

    return 0;
}