#ifndef SYNC_H
#define SYNC_H

#include <mutex>
#include <condition_variable>
#include <iostream>

class Barrier {

private:
    std::mutex lock;
    std::condition_variable cv;
    int nt = 1;
    int now = 0;

public:
    Barrier() {
        nt = 1;
        now = 0;
    }
    // initially set number of threads to await
    void set_t(int n);
    void bwait();

};

#endif
