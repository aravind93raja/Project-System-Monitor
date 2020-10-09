#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    
    cpu_ = ((float)LinuxParser::ActiveJiffies()/(LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies()));
    
    return cpu_; }