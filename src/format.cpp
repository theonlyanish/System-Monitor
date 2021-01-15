#include <string>
#include <iomanip>
#include "format.h"

using std::setfill;
// using std:setw;
using std::string;
#include <iomanip>

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hour, min, sec;
    hour = seconds/3600;
    min = (seconds%3600) / 60;
    sec = (seconds%3600) % 60;

    std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << hour << ":" 
     << std::setw(2) << std::setfill('0') << min << ":"
     << std::setw(2) << std::setfill('0') << sec;
  return stream.str();
}
