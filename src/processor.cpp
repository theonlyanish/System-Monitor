#include "processor.h"
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
using std::stof;
using std::string;
using std::to_string;
using std::vector;
#include <fstream>
#include "linux_parser.h"


float Processor::Utilization() { 
    string key, value, line, abc; 

    std::ifstream filestream("/proc/stat");
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice; 

    int usertime = std::stoi(user) - std::stoi(guest);
    int nicetime = std::stoi(nice) - std::stoi(guest_nice);
    int idealtime = std::stoi(idle) + std::stoi(iowait);
    int systemalltime = std::stoi(system) + std::stoi(irq) + std::stoi(softirq);
    int virtualtime = std::stoi(guest) + std::stoi(guest_nice);
    int totaltime = usertime + nicetime + systemalltime + idealtime + std::stoi(steal) + virtualtime;
    

    
    return float((totaltime-idealtime)/((float) totaltime)); 
}