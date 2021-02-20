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

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {

    vector<int> process_ids = LinuxParser::Pids();
    for (int pid: process_ids) {
        Process process;
        process.Pid(pid);

        string user = LinuxParser::User(pid);
        process.User(user);

        long system_uptime = LinuxParser::UpTime();
        float process_total_time = LinuxParser::ActiveJiffies(pid);
        vector<string> cpu_utilisation = LinuxParser::CpuUtilization(pid);
        float process_start_time = std::stof(cpu_utilisation[21]);
        process.CpuUtilisation(system_uptime, process_start_time, process_total_time);

        string ram = LinuxParser::Ram(pid);
        process.Ram(ram);

        long int uptime = LinuxParser::UpTime(pid);
        process.UpTime(uptime);

        processes_.push_back(process);
    }

    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }