//
// Created by geek on 2022/3/7.
//

#include "CMSA2CD.h"

std::atomic<bool> CMSA2CD::lock_(false);

std::string CMSA2CD::buffer_  = "";

CMSA2CD::CMSA2CD(std::string data, int delay, int id)
{

    content_ = data;
    state_ = State::listen;
    delay_ = delay;
    id_ = id;


}
CMSA2CD::CMSA2CD(const CMSA2CD &sample) :
    delay_(sample.delay_),content_(sample.content_),
    state_(sample.state_),id_(sample.id_){}
CMSA2CD::CMSA2CD(CMSA2CD &sample) :
    delay_(sample.delay_),content_(sample.content_),
    state_(sample.state_),id_(sample.id_){}
CMSA2CD::~CMSA2CD() {}



int CMSA2CD::BackoffAlgorithm(int retansmit_count)
{

    auto pow = [](int x,int p)
    {
        int ans = 1;
        while (p)
        {
            if (p&1) ans = ans * x;
            x = x * x%p;
            p = p >> 1;
        }
        return ans;
    };

    int k = std::min(retansmit_count,10);
    srand((unsigned ) time(NULL));
    int r = rand()%pow(2,k);

    return 2*delay_*r;
}

void CMSA2CD::StartTimeStamp() {
    time_point_ = std::chrono::steady_clock::now();
}

int CMSA2CD::GetTime() {
    using namespace std;
    using namespace chrono;
    auto now = steady_clock::now();
    auto duration = duration_cast<milliseconds>(now-time_point_).count();
    return duration;
}


void  CMSA2CD::CreateTask()
{
        task_ = std::thread(&CMSA2CD::Stimulate,this);
}


void CMSA2CD::Stimulate()
{
//    auto host = static_cast<CMSA2CD*>(object);
    int retansmit_count_ = 0;
    srand((unsigned int) time(NULL));
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 2000));    //  随即休眠0～2s模拟开始
    while (retansmit_count_ < 16) {

        switch (CMSA2CD::state_) {
            case CMSA2CD::State::listen:
                if (CMSA2CD::lock_ == true) continue;
                state_ = CMSA2CD::State::confirming;
                StartTimeStamp();
                break;
            case CMSA2CD::State::confirming:
                // Todo 确认等待时间
                if (GetTime() > 2 * delay_) {
                    if (CMSA2CD::lock_ == false) {
                        lock_ = 1;
                        state_ = CMSA2CD::State::sending;
                    } else {
                        // Todo 发送干扰信号
                        state_ = CMSA2CD::State::backoff;
                        printf("ID %d Meet Conflict\n", id_);
                    }

                }

                break;
            case CMSA2CD::State::sending:
                buffer_ = "";
                for (int i = 0; i < content_.length(); i++) {
                    // 模拟发送中其他插入造成冲突
                    if (content_.length() / 2 < i) lock_ = 0;
                    buffer_ = buffer_ + content_[i];
                }
                if (lock_ == 0) {
                    printf("ID %d Successful send message:  %s with %d epoch \n", id_, buffer_.c_str(),retansmit_count_);
                    // Todo 等待帧间隔
                    std::this_thread::sleep_for(std::chrono::milliseconds(2 * delay_));
                    return;
                } else {
                    printf("ID %d Meeting Conflict \n", id_);
                    // Todo 发送干扰信号
                    state_ = CMSA2CD::State::backoff;
                }
                break;
            case CMSA2CD::State::backoff:
                auto sleep_time = BackoffAlgorithm(retansmit_count_);
                std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
                retansmit_count_++;
                state_ = CMSA2CD::State::listen;
                break;

        }
    }

}

void CMSA2CD::EndTask() {
    if(task_.joinable())    task_.join();
}
