#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>


// Return the aggregate CPU utilization
float Processor::Utilization() {
    float active_jiffies = LinuxParser::ActiveJiffies();
    return active_jiffies/LinuxParser::Jiffies(); 
}