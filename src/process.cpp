#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <unistd.h>

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
    long clocks_per_second = sysconf(_SC_CLK_TCK);
    float total_elapsed_time = system_uptime - (process_start_time/clocks_per_second);
    cpu_utilisation_ = process_total_time/clocks_per_second/total_elapsed_time; 
}

// Return the command that generated this process
string Process::Command() { return command_; }

void Process::Command(string command) { command_ = command; }

// Return this process's memory utilization
string Process::Ram() { return ram_; }

void Process::Ram(string ram) { ram_ = ram; }

// Return the user (name) that generated this process
string Process::User() { return user_; }

void Process::User(std::string user) { user_ = user; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

void Process::UpTime (long int uptime) {
    long clocks_per_second = sysconf(_SC_CLK_TCK);
    uptime_ = (float) uptime/clocks_per_second; 
}

//  Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return std::stol(ram_) > std::stol(a.ram_); 
}