#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>


// Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpu_values = LinuxParser::CpuUtilization();

    long user_time = std::stol(cpu_values[LinuxParser::CPUStates::kUser_]);
    long nice_time = std::stol(cpu_values[LinuxParser::CPUStates::kNice_]);
    long system_time = std::stol(cpu_values[LinuxParser::CPUStates::kSystem_]);
    long interrupt_time = std::stol(cpu_values[LinuxParser::CPUStates::kIRQ_]);
    long soft_interrupt_time = std::stol(cpu_values[LinuxParser::CPUStates::kSoftIRQ_]);
    long involuntary_wait_time = std::stol(cpu_values[LinuxParser::CPUStates::kSteal_]);
    long idle_time = std::stol(cpu_values[LinuxParser::CPUStates::kIdle_]);
    long io_wait_time = std::stol(cpu_values[LinuxParser::CPUStates::kIOwait_]);

    float total_non_idle_time = user_time + nice_time + system_time + interrupt_time + soft_interrupt_time + 
        involuntary_wait_time;
    float total_idle_time = idle_time + io_wait_time;

    return total_non_idle_time/(total_non_idle_time + total_idle_time); 
}