#include <dirent.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <iomanip>
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float total_memory;
  float free_memory;

  string line;
  string category;
  string value;
  string units;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

      std::istringstream line_stream(line);
      while (line_stream >> category >> value >>units) {
        if (category == "MemTotal:") {
          total_memory = std::stod(value);
        } else if (category == "MemFree:") {
          free_memory = std::stod(value);
        }
      }
    }
  }

  return (total_memory - free_memory)/total_memory; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string system_uptime, time_in_idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> system_uptime >> time_in_idle;

    return (float) std::stof(system_uptime);
  }

  return 0;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
   return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> cpu_utilisation = LinuxParser::CpuUtilization(pid);

  long user_code_time = std::stol(cpu_utilisation[13]);
  long kernel_code_time = std::stol(cpu_utilisation[14]);
  long waited_user_time = std::stol(cpu_utilisation[15]);
  long waited_kernel_time = std::stol(cpu_utilisation[16]);

  float process_total_time = user_code_time + kernel_code_time + waited_user_time + waited_kernel_time;

  return process_total_time; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::vector<std::string> cpu_values = LinuxParser::CpuUtilization();

  long user_time = std::stol(cpu_values[LinuxParser::CPUStates::kUser_]);
  long nice_time = std::stol(cpu_values[LinuxParser::CPUStates::kNice_]);
  long system_time = std::stol(cpu_values[LinuxParser::CPUStates::kSystem_]);
  long interrupt_time = std::stol(cpu_values[LinuxParser::CPUStates::kIRQ_]);
  long soft_interrupt_time = std::stol(cpu_values[LinuxParser::CPUStates::kSoftIRQ_]);
  long involuntary_wait_time = std::stol(cpu_values[LinuxParser::CPUStates::kSteal_]);

  float total_non_idle_time = user_time + nice_time + system_time + interrupt_time + soft_interrupt_time + 
    involuntary_wait_time;
  return total_non_idle_time; 
}

//  Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> cpu_values = LinuxParser::CpuUtilization();

  long idle_time = std::stol(cpu_values[LinuxParser::CPUStates::kIdle_]);
  long io_wait_time = std::stol(cpu_values[LinuxParser::CPUStates::kIOwait_]);
  
  float total_idle_time = idle_time + io_wait_time;
  return total_idle_time; 
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string buffer;
  vector<string> cpu_values;

  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

      std::stringstream string_stream(line);
      string_stream >> buffer;
      if (buffer == "cpu") {
        while (string_stream >> buffer) {
          cpu_values.push_back(buffer);
        }

        return cpu_values;
      }
    }
  }
  return {}; 
}

int GetStatValue(std::string stat_key) {
  string line;
  string category;
  string value;

  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

      std::istringstream line_stream(line);
      while (line_stream >> category >> value) {
        if (category == stat_key) {
          return std::stoi(value); 
        }
      }
    }
  }

  return 0;  
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return GetStatValue("processes"); 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
   return GetStatValue("procs_running"); 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string(); 
}

vector<string> GetStatusValues(string stat_name, int pid) {
  string line;
  string buffer;
  vector<string> status_values;

  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::stringstream string_stream(line);
      string_stream >> buffer;

      if (buffer == stat_name) {
        while (string_stream >> buffer) {
          status_values.push_back(buffer);
        }
        return status_values;
      }
    }
  }
  return {};  
}


// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 

  vector<string> status_values = GetStatusValues("VmSize:", pid);
  int memory_in_mb = std::stoi(status_values[0]) * 0.001;
  return std::to_string(memory_in_mb); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return GetStatusValues("Uid:", pid)[0];
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string user_id = Uid(pid);

  char delimiter = ':';
  string line;
  string buffer;

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

      std::stringstream string_stream(line);
      vector<string> user_values;

      while (std::getline(string_stream, buffer, delimiter)) {
        user_values.push_back(buffer);
      }

      string current_uid = user_values[2];
      if (current_uid == user_id) {
        return user_values[0].substr(0, 4);
      }
    }
  }
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  vector<string> cpu_utilisation = LinuxParser::CpuUtilization(pid);
  string process_uptime = cpu_utilisation[21];
  return std::stol(process_uptime); 
}

vector<string> LinuxParser::CpuUtilization(int pid) {
  string line;
  string buffer;
  vector<string> cpu_values;

  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) +LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

      std::stringstream string_stream(line);
      while (string_stream >> buffer) {
        cpu_values.push_back(buffer);
      }
      return cpu_values;
      
    }
  }
  return {}; 
}