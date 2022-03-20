#include "CMSA2CD.h"
#include <iostream>
#include <cmath>
#include <cstring>

using State = CMSA2CD::State;
CMSA2CD::CMSA2CD(char hostname, int flag,std::shared_ptr<char[]> ptr) {
    id_ = hostname;
    buffer_ptr_ = ptr;
}
CMSA2CD::~CMSA2CD() {}

/**
 * @brief 模拟监听96bit信道
 * @param pos当前客户端的发送起始位置
 */
void CMSA2CD::Listen(int pos) {
    using namespace std::chrono;
    state_ = State::confirming;
    int res_bits = 96;
    for(res_bits;res_bits>0;res_bits--)
    {
        if(buffer_ptr_[pos]!='-') res_bits = 96 ;
        std::this_thread::sleep_for(milliseconds(bit_time_));
    }
}
/**
 * @brief 模拟在100bits信道上发送消息
 * @details 0-99
 */
void CMSA2CD::Stimulate()
{
    using namespace std;
    int st_pos,retransmit;
    if(flag == 0) st_pos = 0;
    else st_pos = 99;
    retransmit = 0;
    state_ = State::listen;

    while (retransmit<16)
    {
        switch (state_) {
            case State::listen:
                Listen(st_pos);
                state_ = State::sending;
                break;
            case State::sending:
                if(Send(st_pos))
                {   // 发送成功继续发送
                    state_ = State::listen;
                    retransmit = 0;
                    Clear();
                }else
                {
                    using namespace chrono;
                    retransmit ++;
                    // 发送失败发送干扰信号
                    std::this_thread::sleep_for(milliseconds((48+100)*bit_time_));
                    Clear();
                    state_ = State::backoff;
                }
                break;
            case State::backoff:
                BackoffAlgorithm(retransmit);
                state_ = State::listen;
                break;

        }
    }
    cout<<"发送重试次数过多，发送失败！"<<endl;
}

void CMSA2CD::BackoffAlgorithm(int retansmit_count)
{
    using namespace std::chrono;
    int k = std::min(retansmit_count,10);
    srand((unsigned ) time(NULL));
    int r = rand()%(int)pow(2,k);

    //Todo 缩小r以强化显示冲突
    r = r%4;
    std::this_thread::sleep_for(milliseconds((r*64*8)*bit_time_));     //r倍的征用期一个以太帧64字节*8bit
}

bool CMSA2CD::Send(int st) {
    using namespace std::chrono;
    int delta_pos = 0;
    int data = 64*8+8*8; // 最小帧长加帧前同步
    while (data>0)
    {
        if(buffer_ptr_[st+delta_pos]=='-')  // 发送前检查下
        {
            // 完成从一端到另一端的传输
            if(abs(delta_pos)==100-1)
            {
                buffer_ptr_[st+delta_pos] = id_;
                std::this_thread::sleep_for(milliseconds(bit_time_));
                if(data>0)
                {
                    std::this_thread::sleep_for(milliseconds(bit_time_*data));
                    Clear();    // 重置信道
                    return true;
                }
            }else
            {
                buffer_ptr_[st+delta_pos] = id_;
                AddPos(delta_pos);
            }

        }else
        {
            return false;
        }
        std::this_thread::sleep_for(milliseconds(bit_time_));
    }

    // 发生冲突
    return false;
}

void CMSA2CD::Clear() {
    for(int i=0;i<100;i++) buffer_ptr_[i] = '-';
}

int CMSA2CD::AddPos(int &x) {
    return flag==0?x=x+1:x=x-1;
}


