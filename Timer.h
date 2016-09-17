#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>

class Timer
{   // L for long, so that it doesn't become an int
    std::thread Thread; // our thread for asynchronous code execution
    bool Alive = false; // the state of the timer, is it running or not?
    long CallNumber = -1L;  // to determine how many times we would like to call a certain function
    long repeat_count = -1L;    // to count the number of times a function has already been called
    std::chrono::milliseconds interval = std::chrono::milliseconds(0);  // interval between function calls (default value is 0)
    // new type in C++ 11 which can contain functions, similar to a pointer to a function
    std::function<void(void)> funct = nullptr;

    void SleepAndRun()
    {
        std::this_thread::sleep_for(interval);  // pretty self-explanatory
        if(Alive)
            Function()(); // the first () to call the function "Function" and the second () to call the function that "Function" returns
    }
    // we will pass this function to a thread to have more control over it (call, sleep etc)
    void ThreadFunc()
    {
        if(CallNumber == Infinite)
            while(Alive) SleepAndRun();
        else
            while(repeat_count--) SleepAndRun();
    }

public:
    static const long Infinite = -1L;
    // our constructors
    Timer(){}
    // this constructors accepts a reference to a constant function object
    Timer(const std::function<void(void)> &f) : funct(f) {}
    // this one will have more parameters
    Timer(const std::function<void(void)> &f, const unsigned long &i, const long repeat = Timer::Infinite) : funct(f), interval(std::chrono::milliseconds(i)), CallNumber(repeat) {}
    // method for starting the timer
    void Start(bool Async = true)
    {
        if(IsAlive())
            return;
        Alive = true;
        repeat_count = CallNumber;
        if(Async)
            Thread = std::thread(ThreadFunc, this); // ThreadFunct = the function that will be executed, this a pointer to our class
        else
            ThreadFunc();   // he has this->ThreadFunct();
    }
    // method for stopping the timer
    void Stop()
    {
        Alive = false;
        Thread.join();
    }
    // setters and getters
    void SetFunction(const std::function<void(void)> &f)  // for when we use the empty constructor
    {
        funct = f;
    }
    bool IsAlive() const
    {
        return Alive;
    }
    void RepeatCount(const long r)
    {
        if(Alive)
            return;
        CallNumber = r;
    }
    long GetLeftCount() const   // to know how many iterations are left
    {
        return repeat_count;
    }
    long RepeatCount() const    // get the total number of repeatances
    {
        return CallNumber;
    }
    void SetInterval(const unsigned long &i)
    {
        if(Alive)
            return;
        interval = std::chrono::milliseconds(i);
    }
    unsigned long Interval() const
    {
        return interval.count();    // .count() to get an actual number from interval, because interval is of type chrono::milliseconds
    }
    const std::function<void(void)> &Function() const
    {
        return funct;
    }
};

#endif // TIMER_H
