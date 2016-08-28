/*!
 * \author David
 * \date 17-May-16.
 */

#include <spdlog/spdlog.h>

#include "test_utils.h"

 static std::shared_ptr<spdlog::logger> logger;

void run_test(test_func test_function, std::string name) {
	logger = spdlog::get("nova");
	logger->info("Starting test {}", name);
    try {
        test_function();
    } catch(std::exception & e) {
    	logger->error("Exception with message {}", e.what());
    }
}
