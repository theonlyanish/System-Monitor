#include <dirent.h>
#include <unistd.h> 
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel,version;// <----- Declared a new variable version 
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel>>version;// <-- Extracting the version in a variable version
  }
  return version;//<---- Return the third variable
}


// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string key, value, line;
  float FM, TM; //Free Memory and Total memory 
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
    std::istringstream linestream(line);
    while (linestream>> key>>value){
      if (key == "MemTotal:"){
        TM = std::stof(value);
      }
      if (key == "MemAvailable:"){
        FM = std::stof(value);
      }
    }
    }
  }
  return (TM-FM)/TM; 
}

long LinuxParser::UpTime() { 
  string line, uptime, idletime; 
  std::ifstream FS(kProcDirectory + kUptimeFilename);
  if (FS.is_open()){
    std::getline(FS,line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  long UT = std::stol(uptime); 
  return UT; 
}

long LinuxParser::Jiffies() { 
  return 0; 
}

long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  return 0; 
}

long LinuxParser::ActiveJiffies() { 
  return 0; 
}

long LinuxParser::IdleJiffies() { 
  return 0; 
}

vector<string> LinuxParser::CpuUtilization() { 
  return {}; 
}

int LinuxParser::TotalProcesses() { 
  string key, value, line;
  int pcs;
  std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()){
    while(std::getline(filestream,line)){
    std::istringstream linestream(line);
    while (linestream>> key>>value){
      if (key == "processes"){
        pcs = std::stoi(value);
      }

    }
    }
  }  
  
  return pcs; 
}

int LinuxParser::RunningProcesses() {
    string key, value, line;
  int pcs;
  std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()){
    while(std::getline(filestream,line)){
    std::istringstream linestream(line);
    while (linestream>> key>>value){
      if (key == "procs_running"){
        pcs = std::stoi(value);
      }

    }
    }
  }  
  
  return pcs; 
}

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);

  }
  
  return line; 
}

string LinuxParser::Ram(int pid) {
  string key, value, line, ram{"0"};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          ram = value;
        }
      }
    }
  }
  return std::to_string(std::stoi(ram) / 1000);
}


string LinuxParser::Uid(int pid) { 
  string key, value, line, ram, U_ID;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
    std::istringstream linestream(line);
    while (linestream>> key>>value){
      if (key == "Uid:"){
        U_ID =value;
      } } }
  }

  return U_ID; 
}

string LinuxParser::User(int pid) { 
  string line, user;
  string uid {Uid(pid)};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      char split_char = ':';
      std::vector<std::string> tokens;
      for (std::string each; std::getline(linestream, each, split_char); tokens.push_back(each));
      if (tokens[2] == uid){
          user = tokens[0];
      }

    }
  }
  
  return user; 
}

long LinuxParser::UpTime(int pid) { 
  string key, value, line, ram;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::getline(filestream,line);
  std::istringstream linestream(line);
  std::vector<std::string> results(std::istream_iterator<std::string>{linestream},     	    		std::istream_iterator<std::string>()); 
  //Conditional in case   
  long v{0}; 
  if(results.size() >= 22) {
   	v = std::stol(results[21]); 
  } else {
    return v; 
  } 
 
  return (UpTime() - v/sysconf(_SC_CLK_TCK));
  
}