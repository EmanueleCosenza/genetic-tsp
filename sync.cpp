#include <mutex>
#include <condition_variable>
#include <iostream>

#include "sync.hpp"

// initially set number of threads to await
void Barrier::set_t(int n) {
    nt = n;
}

void Barrier::bwait() {
    std::unique_lock<std::mutex> lk(lock);
    now++;
    if(now == nt) {
        cv.notify_all();
    } else {
        auto pred = [&]() { return (now==nt); };
        cv.wait(lk, pred);
    }
}
