#include "CMSA2CD.h"
#include <iostream>
#include <cmath>
#include <cstring>

using State = CMSA2CD::State;
CMSA2CD::CMSA2CD(char hostname, int flag,std::shared_ptr<char[]> ptr) {
    id_ = hostname;
    buffer_ptr_ = ptr;
    bit_time_ = 10;
    Clear();
}
CMSA2CD::~CMSA2CD() {}

/**
 * @brief 模拟监听96bit信道
 * @param pos当前客户端的发送起始位置
 */
void CMSA2CD::Listen(int pos) {
    using namespace std::chrono;
    using namespace std;
    string message = "Host: "+to_string(id_)+" Listening";
    std::cout<<message<<std::endl;
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
                    std::cout<<"Host: "<<id_<<" Send message successfully"<<endl;
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
    using namespace std;
    using namespace std::chrono;
    int k = std::min(retansmit_count,10);
    // 时间过于相近而倒是srand内容相同
    int r = rand()%(int)pow(2,k);       // 取模数后范围在0——2^(k-1)

    string messgae = "Host: "+ to_string(id_)+" Back off: "+ to_string(retansmit_count)
            +" r="+ to_string(r);
    std::cout<<messgae<<std::endl;
//    r = r%4;    //缩小r以强化显示冲突
    // 以太网默认的征用期为512bit是以10M/s网速和其他内容计算所得，实际上从协议讲应该是2*t t是单程时延
    std::this_thread::sleep_for(milliseconds(r*2*(100)*bit_time_));     //r倍的征用期以太网以一个太帧64字节*8bit
}

bool CMSA2CD::Send(int st) {
    using namespace std::chrono;
    using namespace std;
    std::cout<<"Host: "<<id_<<" Sending Message"<<endl;
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
            // 发生冲突
            using namespace std;
            //Todo 冲突为中总是50???
            string message  = "Meeting Confict in "+ to_string(abs(delta_pos))+'\n';
            cout<<message;
            return false;
        }
        std::this_thread::sleep_for(milliseconds(bit_time_));
    }
}

void CMSA2CD::Clear() {
    for(int i=0;i<100;i++) buffer_ptr_[i] = '-';
}

int CMSA2CD::AddPos(int &x) {
    return flag==0?x=x+1:x=x-1;
}



