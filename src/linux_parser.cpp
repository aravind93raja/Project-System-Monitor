#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, kernel ,version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
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
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  
  string line;
  string key;
  string value;
  string unitOfBytes;
  float MemTotal,MemFree;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unitOfBytes) {
        if (key == "MemTotal") {
          MemTotal = stof(value);
        }
        if (key == "MemFree") {
          MemFree = stof(value);
        }
      }
    }
  }
  return ((MemTotal-MemFree)/MemTotal);
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  
  string upTime, idleTime , line;
  
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
    return stol(upTime);
  }
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  
  string cpuDes,j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,line;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpuDes >> j1 >> j2 >>j3 >> j4 >> j5 >> j6 >>j7 >> j8 >>j9 >> j10;
    return stoi(j1) + stoi(j2) + stoi(j3)+ stoi(j4)+stoi(j5)+stoi(j6)+stoi(j7)+stoi(j8)+stoi(j9) +stoi(j10);
  }
   
  return 0; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 

 string value, line;
  vector<string> values;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  
return stol(values[13]) + stol(values[14]) +stol(values[15]) +stol(values[16]) ;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string cpuDes,j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,line;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpuDes >> j1 >> j2 >>j3 >> j4 >> j5 >> j6 >>j7 >> j8 >>j9 >> j10;
    
  }
   
  return (stol(j1) + stol(j2) + stol(j3)+stol(j6)+stol(j7)+stol(j8)+stol(j9) +stol(j10));
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  string cpuDes,j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,line;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpuDes >> j1 >> j2 >>j3 >> j4 >> j5 >> j6 >>j7 >> j8 >>j9 >> j10;
    
  } 
    return (stol(j4) + stol(j5));
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line ,value, key;
  vector<string> values;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      if (key != "cpu") {
        values.push_back(key);
      }
    }
  }
  return values;
   }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
  
  string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
    string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  
  return 0;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
  string command, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
  
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  
  string key, value, unitOfBytes, line;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unitOfBytes) {
        if (key == "VmSize:") {
          return to_string(stoi(value)/1024);
         
        }
      }
    }
  }
  
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
  string line ,key, value;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  
  return string();
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  
  string user,one,uid,line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> one >> uid) {
        if ( uid == LinuxParser::Uid(pid)) {
         return user;
        }
      }
    }
  }
   return string();
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  
  string value, line;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  
  return (LinuxParser::UpTime()-(stol(values[21]) / sysconf(_SC_CLK_TCK)));
  
  
  }

  float LinuxParser::CpuUtilizationProcess(int pid){

  string value, line;
  vector<string> values;
  float stime ,utime ,cutime,cstime,totaltime, seconds;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
    //uptime = stof(values[1]);
    utime = stof(values[14]);
    stime = stof(values[15]);
    cutime = stof(values[16]);
    cstime = stof(values[17]);
    //starttime = stof(values[22]);
  }
  totaltime = utime +stime +cutime +cstime ;
  seconds = LinuxParser::UpTime(pid);

  return 100*((totaltime/sysconf(_SC_CLK_TCK))/seconds);

  }