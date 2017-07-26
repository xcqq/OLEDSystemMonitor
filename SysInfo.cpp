//
// Created by xczhang on 17-7-12.
//

#include <iostream>
#include <backward/strstream>
#include <cstring>
#include <unistd.h>
#include <regex>
#include "SysInfo.h"

SysInfo::SysInfo()
{
    clock_gettime(CLOCK_MONOTONIC,&mSysTicksPre);//get systicks
    mCpuInfo.open("/proc/stat",ios_base::in);
    mMemInfo.open("/proc/meminfo",ios_base::in);
    mParseCpuUsage("",cpuStatePre);
}

float SysInfo::getCpuUsage()
{
    struct cpuState cpuStateCurr;
    timespec sysTicksCurr;
    clock_gettime(CLOCK_MONOTONIC,&sysTicksCurr);
    mParseCpuUsage("",cpuStateCurr);
    mCpuUsage=1-(float)(cpuStateCurr.idle-cpuStatePre.idle)/(float)(cpuStateCurr.total - cpuStatePre.total);
    cout << "Current CPU usage: " << mCpuUsage*100 << "%" <<endl;
    cpuStatePre=cpuStateCurr;
    return mCpuUsage;
}

float SysInfo::getMemUsage()
{
    regex memTotalPatten("[\\d\\D]*MemTotal:\\D*(\\d+) *kB[\\d\\D]*");
    regex memAvailablePatten("[\\d\\D]*MemAvailable:\\D*(\\d+) *kB[\\d\\D]*");
    smatch resultsMemTotal;
    smatch resultsMemAvailable;
    string strMemTotal,strMemAvailable;
    char buffer[100];
    string bufferString;
    mMemInfo.seekg(0,ios_base::beg);
    if(mMemInfo.is_open()) {
        while (!mMemInfo.eof()) {
            memset(buffer, 100, sizeof(char));
            mMemInfo.getline(buffer, 100);
            bufferString = buffer;
            if (regex_match(bufferString, resultsMemAvailable, memAvailablePatten)) {
                strMemAvailable=resultsMemAvailable.str(1);
                break;
            }
        }
    }
    mMemInfo.seekg(0,ios_base::beg);
    if(mMemInfo.is_open()) {
        while (!mMemInfo.eof()) {
            memset(buffer, 100, sizeof(char));
            mMemInfo.getline(buffer, 100);
            bufferString = buffer;
            if (regex_match(bufferString, resultsMemTotal, memTotalPatten)) {
                strMemTotal=resultsMemTotal.str(1);
                break;
            }
        }
    }
    mMemUsage=1-atof(strMemAvailable.c_str())/atof(strMemTotal.c_str());
    cout << "Current Mem usage: " << mMemUsage*100 << "%" << endl;
    return mMemUsage;
}

float SysInfo::getDiskUsage()
{
    return 0;
}

SysInfo::~SysInfo()
{
    mCpuInfo.close();
    mMemInfo.close();
}

int SysInfo::mParseCpuUsage(string cpuID, struct cpuState &CpuStateCurr)
{

    stringstream stringStreamPatten;
    vector<string> stringVector;
    uint8_t i;
    stringStreamPatten << "[\\d\\D]*cpu" << cpuID << " +(\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +\\d+ +\\d+)[\\d\\D]*";
    regex regexPatten(stringStreamPatten.str());
    match_results<std::string::const_iterator> resultsCpuUsage;
    char buffer[100];
    string bufferString;
    mCpuInfo.seekg(0,ios_base::beg);
    if(mCpuInfo.is_open())
    {
        while(!mCpuInfo.eof())
        {
            memset(buffer,100,sizeof(char));
            mCpuInfo.getline(buffer,100);
            bufferString=buffer;
            //regex_match(bufferString,resultsCpuUsage,regexPatten);
            if(regex_match(bufferString, resultsCpuUsage, regexPatten))
            {
                split(resultsCpuUsage.str(1),stringVector," ");
                CpuStateCurr.user= (uint64_t) atoi(stringVector[0].c_str());
                CpuStateCurr.nice= (uint64_t) atoi(stringVector[1].c_str());
                CpuStateCurr.system= (uint64_t) atoi(stringVector[2].c_str());
                CpuStateCurr.idle= (uint64_t) atoi(stringVector[3].c_str());
                CpuStateCurr.iowait= (uint64_t) atoi(stringVector[4].c_str());
                CpuStateCurr.irq= (uint64_t) atoi(stringVector[5].c_str());
                CpuStateCurr.softirq= (uint64_t) atoi(stringVector[6].c_str());
                CpuStateCurr.stealstolen= (uint64_t) atoi(stringVector[7].c_str());
                CpuStateCurr.guest= (uint64_t) atoi(stringVector[8].c_str());
                CpuStateCurr.total=0;
                for(i=0;i<=8;i++)
                {
                    CpuStateCurr.total+=atoi(stringVector[i].c_str());
                }

                break;
            }
        }
        //todo set cpustate

    }
    else
    {
        cerr << "Open /proc/stat fail" << endl;
        return -1;
    }

    return 0;
}
