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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

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

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {

  string line;
  string buffer;
  vector<string> uid_values;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {

      std::stringstream string_stream(line);
      string_stream >> buffer;

      if (buffer == "Uid:") {
        while (string_stream >> buffer) {
          uid_values.push_back(buffer);
        }
        return uid_values[0];
      }
    }
  }
  return {}; 
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

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

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