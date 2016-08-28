//
// Created by David on 05-Apr-16.
//

#include <spdlog/spdlog.h>
#include "key_forwarder.h"

void key_forwarder::forward_keypress(int key, int action) {
	spdlog::get("nova")->info("Received key {} with action {}", key, action);
}

