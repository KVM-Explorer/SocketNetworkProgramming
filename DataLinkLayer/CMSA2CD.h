#pragma once
#include <mutex>
#include <vector>
#include <atomic>
#include <unistd.h>
#include <chrono>
#include <memory>
#include <thread>


class CMSA2CD {
public:
    enum class State
    {
        listen,
        confirming,
        sending,
        backoff
    };
protected:
    std::thread task_;              // 线程
    int bit_time_;                  // 每个bit特传输时间
    int flag;                       // 0 ->  1 <-
    std::shared_ptr<char[]> buffer_ptr_;
    static std::mutex mutex;
public:
    State state_;                   // 当前状态
    char id_;                // 当前主机ID名称

public:
    CMSA2CD(char hostname,int flag,std::shared_ptr<char[]> ptr);
    ~CMSA2CD();

    void Listen(int pos);
    void Stimulate();
    void BackoffAlgorithm(int retansmit_count);
    bool Send(int st);
    void Clear();
    int AddPos(int &x);
//    void Print(std::string messgae);
};



