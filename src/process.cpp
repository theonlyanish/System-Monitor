#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include <unistd.h>

using std::string;
using std::to_string;
using std::vector;

Process::Process (int PID){
    pid = PID;
    uptime = LinuxParser::UpTime(PID);
    ram = LinuxParser::Ram(PID);
    user = LinuxParser::User(PID);
    command = LinuxParser::Command(PID);
    cpu_ = CpuUtilization();
    
};

int Process::Pid() { 
    return pid; 
}

float Process::CpuUtilization() { 
    string line;
    std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
    std::getline(filestream,line);
    std::istringstream linestream(line);
    std::vector<std::string> results(std::istream_iterator<std::string>{linestream}, std::istream_iterator<std::string>());

    float UT, ST, CT, CsT, Start, total,secs;
    UT = std::stof(results[13]);
    ST = std::stof(results[14]);
    CT = std::stof(results[15]);
    CsT= std::stof(results[16]);
    Start= std::stof(results[21]);
    
    total = UT + ST+ CT + CsT;
    secs = (UpTime() - Start / sysconf(_SC_CLK_TCK));
    return (total/ sysconf(_SC_CLK_TCK)/secs); 
}

string Process::Command() { 
    return LinuxParser::Command(pid); 
}

string Process::Ram() { 
    return ram; 
}

string Process::User() { 
    return user; 
}

long int Process::UpTime() { 
    return uptime; 
}

bool Process::operator<(Process const& a) const { 
    return a.cpu_>this->cpu_; 
}