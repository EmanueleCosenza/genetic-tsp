#ifndef SYNC_H
#define SYNC_H

#include <mutex>
#include <condition_variable>
#include <iostream>

#include <iostream>
#include <atomic>

class SpinBarrier {

private:
    std::atomic<int> n;

public:
    SpinBarrier() {}
    SpinBarrier(int n): n(n) {}

    void set_t(int n);
    void bwait();

};

class BlockingBarrier {

private:
    std::mutex lock;
    std::condition_variable cv;
    int nt;
    int now = 0;

public:
    BlockingBarrier() {
        nt = 1;
        now = 0;
    }

    void set_t(int n);
    void bwait();

};

#endif
