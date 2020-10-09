#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 

    int hours_ = seconds/3600;
    int minutes_ = (seconds%3600)/60;
    int seconds_ = (seconds%3600)%60;

    string time = std::to_string(hours_) + ":" + std::to_string(minutes_) + ":" + std::to_string(seconds_);

    return time; 
    
}