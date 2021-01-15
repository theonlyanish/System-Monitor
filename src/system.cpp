  
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


Processor& System::Cpu() { 
    return cpu_; 
}

vector<Process>& System::Processes() { 
    vector <int> pids = LinuxParser::Pids();
    processes_.clear();
    for (vector<int>::iterator it = pids.begin(); it != pids.end(); ++it){
        Process ProcessA(*it);
        processes_.push_back(ProcessA);
    }
    std::sort(processes_.rbegin(), processes_.rend());
    return processes_; }

System::System(){
    cpu_ = Cpu();
    processes_ = Processes();

}

std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}


float System::MemoryUtilization() {  
    return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() {    
    return LinuxParser::UpTime(); 
}