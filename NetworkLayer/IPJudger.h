#pragma once
#include <string>
#include <vector>
class IPJudger {
    enum class IpResult
    {
        undefine,
        error,
        AType,
        BType,
        CType,
        DType,
        other,
    };

public:
    IPJudger():State(IpResult::undefine){};
    ~IPJudger(){};
    IpResult State;
    bool IsValid(std::string ip);
    std::vector<uint8_t> Split(std::string x);
    void Judge(std::string ip_address);





};


