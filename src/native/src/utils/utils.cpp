/*!
 * \author David
 * \date 18-May-16.
 */

#include <vector>
#include <string>
#include <sstream>
// #include <easylogging++.h>

// void initialize_logging() {
//     // Configure the logger
//     el::Configurations conf("config/logging.conf");

//     // Turn debug and trace off in release builds
// #ifdef NDEBUG
//     conf.parseFromText("*DEBUG:\n ENABLED=false");
//     conf.parseFromText("*TRACE:\n ENABLED=false");
// #else
//     conf.parseFromText("*ALL: FORMAT = \"%datetime{%h:%m:%s} [%level] at %loc - %msg\"");
// #endif

//     el::Loggers::reconfigureAllLoggers(conf);
// }

std::vector<std::string> split_string(const std::string &s, char delim = ' ') {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;

    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}

