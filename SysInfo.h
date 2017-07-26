//
// Created by xczhang on 17-7-12.
//

#ifndef GETSYSINFO_SYSINFO_H
#define GETSYSINFO_SYSINFO_H
#include <fstream>
#include <time.h>
#include <vector>
using namespace std;
struct cpuState
{
    uint64_t user = 0;
    uint64_t nice = 0;
    uint64_t system = 0;
    uint64_t idle = 0;
    uint64_t iowait = 0;
    uint64_t irq = 0;
    uint64_t softirq = 0;
    uint64_t stealstolen = 0;
    uint64_t guest = 0;
    uint64_t total = 0;
};
class SysInfo {

public:
    SysInfo();
    ~SysInfo();

    float getCpuUsage();

    float getMemUsage();

    float getDiskUsage();


private:
    timespec mSysTicksPre;
    ifstream mCpuInfo;
    ifstream mMemInfo;

    int mParseCpuUsage(string cpuID, struct cpuState &CpuStateCurr);
    int split(const string& str, vector<string>& ret_, string sep = ",")
    {
        if (str.empty())
        {
            return 0;
        }

        string tmp;
        string::size_type pos_begin = str.find_first_not_of(sep);
        string::size_type comma_pos = 0;

        while (pos_begin != string::npos)
        {
            comma_pos = str.find(sep, pos_begin);
            if (comma_pos != string::npos)
            {
                tmp = str.substr(pos_begin, comma_pos - pos_begin);
                pos_begin = comma_pos + sep.length();
            }
            else
            {
                tmp = str.substr(pos_begin);
                pos_begin = comma_pos;
            }

            if (!tmp.empty())
            {
                ret_.push_back(tmp);
                tmp.clear();
            }
        }
        return 0;
    }

    struct cpuState cpuStatePre;

    float mCpuUsage = 0;
    float mMemUsage = 0;
    float mDiskUsage = 0;


};


#endif //GETSYSINFO_SYSINFO_H
