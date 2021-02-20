#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

void Process::Pid(int pid) { pid_ = pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_utilisation_; }

void Process::CpuUtilisation(long system_uptime, float process_start_time, float process_total_time) { 
    long clocks_per_second = CLOCKS_PER_SEC;
    float total_elapsed_time = system_uptime - (process_start_time/clocks_per_second);
    cpu_utilisation_ = process_total_time/clocks_per_second/total_elapsed_time * 100; 
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// Return the user (name) that generated this process
string Process::User() { return user_; }

void Process::User(std::string user) { user_ = user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }