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
        conflict,
        backoff
    };
protected:
    std::thread task_;
    std::chrono::steady_clock::time_point time_point_;
public:
    State state_;
    static std::atomic<bool> lock_;
    static std::string buffer_;
    int delay_;
    std::string content_;
    int id_;
public:
    CMSA2CD(std::string data, int delay, int ID);
    CMSA2CD(const CMSA2CD& sample);
    CMSA2CD(CMSA2CD &sample);
    ~CMSA2CD();
    int BackoffAlgorithm(int retansmit_count);
    void StartTimeStamp();
    int GetTime();
    void CreateTask();
    void EndTask();
    void Stimulate();
};



