#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Subject {
 public:
  virtual void Request(int limit) const = 0;
};

class RealSubject : public Subject {
 public:
  void Request(int limit) const override {
    std::cout << "Limit is "<<limit<<" from RealSubject \n";
  }
};

class Proxy : public Subject {

 private:
  std::shared_ptr<Subject> RealSubjPtr;
    unsigned int limitToCheck = 10;
  bool CheckAccess(int limit) const {

    std::cout << "Check limit with proxy.\n";
    if(limit<limitToCheck)
    return true;
    else
    return false;
  }
  void LogAccess() const {
    std::cout << "Another access.\n";
  }

 public:
  Proxy(std::shared_ptr<Subject> ptr):RealSubjPtr(ptr){}

  ~Proxy() {}

  void Request(int limit) const override {
    if (this->CheckAccess(limit)) {
      this->RealSubjPtr->Request(limit);
      this->LogAccess();
    }
    else
    {
        std::cout<<"limit is not enough"<<std::endl;
    }
  }
};

int main()
{
    std::shared_ptr<Subject> ptr = std::make_shared<RealSubject>();
    ptr->Request(12);

    std::shared_ptr<Subject> ProxyPtr = std::make_shared<Proxy>(ptr);
    ProxyPtr->Request(12);
    ProxyPtr->Request(9);
}