#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // See src/process.cpp
  void Pid(int pid);
  std::string User();                      // See src/process.cpp
  void User(std::string user);
  std::string Command();                   // See src/process.cpp
  void Command(std::string command);
  float CpuUtilization();                  // See src/process.cpp
  void CpuUtilisation(long system_uptime, float process_start_time, float process_total_time);
  std::string Ram();                       // See src/process.cpp
  void Ram(std::string ram);
  long int UpTime();                       // See src/process.cpp
  void UpTime(long int uptime);
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // Declare any necessary private members
 private:
    int pid_;
    std::string user_;
    float cpu_utilisation_;
    std::string ram_;
    long int uptime_;
    std::string command_;
};

#endif