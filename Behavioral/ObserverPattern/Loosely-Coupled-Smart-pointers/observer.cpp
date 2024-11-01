#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

// Observer interface
class observerIf {
public:
    virtual void update() = 0;
    virtual ~observerIf() {}
};

// Subject interface
class subjectIf {
public:
    virtual void notify() = 0;
    virtual std::function<void(std::shared_ptr<observerIf>)>& removeCallback() = 0;
    virtual std::function<void(std::shared_ptr<observerIf>)>& addObsCallback() = 0;
    virtual ~subjectIf() {}
};

// Subject implementation
class subject : public subjectIf, public std::enable_shared_from_this<subject> {
public:
   // Use std::make_shared to create and return a shared_ptr to subject
    static std::shared_ptr<subject> create()
    {
        struct make_shared_enabler : public subject
        {
            using subject::subject; // Inherit constructors from `subject`
        };
        return std::make_shared<make_shared_enabler>();
    }

    void notify() override {
        if (mListObs.empty()) {
            std::cout << "list is empty" << std::endl;
        } else {
            std::cout << "list has " << mListObs.size() << " observer(s)" << std::endl;
        }

        for (const auto& obs : mListObs) {
            obs->update();
        }
    }

    std::function<void(std::shared_ptr<observerIf>)>& removeCallback() override {
        return mCallbackRemove;
    }

    std::function<void(std::shared_ptr<observerIf>)>& addObsCallback() override {
        return mCallbackAdd;
    }

    ~subject() {
        std::cout << "subject is destroyed" << std::endl;
    }

private:
    subject()
        : mCallbackRemove{ std::bind(&subject::remove, this, std::placeholders::_1) },
          mCallbackAdd{ std::bind(&subject::addObs, this, std::placeholders::_1) } {}

    void addObs(const std::shared_ptr<observerIf>& obs) {
        std::cout << "Observer added" << std::endl;
        mListObs.push_back(obs);
    }

    void remove(const std::shared_ptr<observerIf>& obs) {
        std::cout << "Observer removed" << std::endl;
        auto it = std::find(mListObs.begin(), mListObs.end(), obs);
        if (it != mListObs.end()) {
            mListObs.erase(it);
        }
    }

    std::function<void(std::shared_ptr<observerIf>)> mCallbackRemove;
    std::function<void(std::shared_ptr<observerIf>)> mCallbackAdd;
    // list has observers with ownership, if observer wants to be deleted then
    // it must be deleted from the list
    std::vector<std::shared_ptr<observerIf>> mListObs;
};

// Concrete observer
class concObserver : public observerIf, public std::enable_shared_from_this<concObserver> {
public:
    // factory method is needed to not use sharede_from_this in cons or destructor.
    // if it is used it will throw bad weak ptr
    //     SM generation compiler returned: 0
    // Execution build compiler returned: 0
    // Program returned: 139
    // terminate called after throwing an instance of 'std::bad_weak_ptr'
    //   what():  bad_weak_ptr
    // Program terminated with signal: SIGSEGV
    // Create concObserver
    // instaed of crate method, initialize function can be called too but creae
    // methods wraps initialization related issues
    // shared_from_this or weak_from_this can only be called on an object that is already managed
    // by shared_ptr
    static std::shared_ptr<concObserver> create(std::function<void(std::shared_ptr<observerIf>)>& remove, std::function<void(std::shared_ptr<observerIf>)>& add) {
        struct make_shared_enabler : public concObserver {
            make_shared_enabler(std::function<void(std::shared_ptr<observerIf>)>& remove, std::function<void(std::shared_ptr<observerIf>)>& add)
                : concObserver(remove, add) {}
        };

        // Step 1: Create a temporary shared_ptr to the object without a custom deleter
        auto tempPtr = std::make_shared<make_shared_enabler>(remove, add);

        // Step 2: Initialize the object after it is fully managed by a shared_ptr
        tempPtr->initialize(tempPtr);

        // Step 3: Create the final shared_ptr with the custom deleter
        return std::shared_ptr<concObserver>(tempPtr.get(), [remove](concObserver* ptr) {
            // Custom deleter calls the remove callback
            remove(ptr->shared_from_this());
        });
    }

    void update() override {
        std::cout << "concObserver is updated" << std::endl;
    }

    ~concObserver() {
        std::cout << "concObserver is destroyed" << std::endl;
    }

private:
    concObserver(std::function<void(std::shared_ptr<observerIf>)>& remove, std::function<void(std::shared_ptr<observerIf>)>& add)
        : mCallbackRemove{ remove },
          mCallbackAdd{ add } {}

    void initialize(std::shared_ptr<concObserver> self) {
        mCallbackAdd(self);
    }

    std::function<void(std::shared_ptr<observerIf>)>& mCallbackRemove;
    std::function<void(std::shared_ptr<observerIf>)>& mCallbackAdd;
};

// Main function
int main() {
    auto sbj = subject::create();  // Now using the `create` method with make_shared

    {
        std::cout << "Create concObserver" << std::endl;
        auto obsCall = concObserver::create(sbj->removeCallback(), sbj->addObsCallback());
        sbj->notify(); // The observer should be notified here.
    } // `obsCall` goes out of scope here, and the observer object is destroyed.

    sbj->notify(); // The observer is no longer valid, so it won't be notified.
    std::cout << "End" << std::endl;
}