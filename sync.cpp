#include <mutex>
#include <condition_variable>
#include <iostream>

#include "sync.hpp"

void ActiveBarrier::set_t(int n) {
    this->n = n;
}

void ActiveBarrier::bwait() {
    n--;
    while(n != 0);
}


void BlockingBarrier::set_t(int n) {
    this->nt = n;
}

void BlockingBarrier::bwait() {
    std::unique_lock<std::mutex> lk(lock);
    now++;
    if(now == nt) {
        cv.notify_all();
    } else {
        auto pred = [&]() { return (now==nt); };
        cv.wait(lk, pred);
    }
}
