#include <string>
#include <iomanip>

#include "format.h"

using std::string;
using std::stringstream;


string FormatNumber(long number) {
    stringstream string_stream;
    string_stream << std::setw(2) << std::setfill('0') << number;

    return string_stream.str();
}

// Helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    long total_seconds = seconds;
    long total_hours = 0;
    long total_minutes = 0;

    const int seconds_in_minute = 60;
    const int seconds_in_hour = 60 * seconds_in_minute;

    if (total_seconds >= seconds_in_hour) {
        total_hours = total_seconds / seconds_in_hour;
        total_seconds = total_seconds % seconds_in_hour;
    }

    if (total_seconds >= seconds_in_minute) {
        total_minutes = total_seconds / seconds_in_minute;
        total_seconds = total_seconds % seconds_in_minute;
    } 


    return FormatNumber(total_hours) + ":" + FormatNumber(total_minutes) + ":" + FormatNumber(total_seconds); 
}