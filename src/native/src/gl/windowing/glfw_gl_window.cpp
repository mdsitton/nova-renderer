//
// Created by David on 15-Apr-16.
//

#include "glfw_gl_window.h"
#include "io/key_forwarder.h"
#include "core/nova_renderer.h"
#include "utils/utils.h"

#include <pthread.h>


void error_callback(int error, const char * description) {
    spdlog::get("nova")->error("Error {}: {}", error, description);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    key_forwarder::forward_keypress(key, scancode);
}

glfw_gl_window::glfw_gl_window() {
    logger = spdlog::get("nova");
    // initialize_logging();

    glfwSetErrorCallback(error_callback);

    if(!glfwInit()) {
        logger->critical("Could not initialize GLFW");
    }

    init();
}

int glfw_gl_window::init() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(640, 480, "Minecraft Nova Renderer", NULL, NULL);
    if(!window) {
        logger->critical("Could not initialize window :(");
    }

    logger->info("GLFW window created");

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if(!gladLoadGL()) {
        logger->critical("Could not load OpenGL");
    }

    const GLubyte * vendor = glGetString(GL_VENDOR);
    logger->info("Vendor: {}", vendor);

    glfwGetFramebufferSize(window, &window_dimensions.x, &window_dimensions.y);
    glViewport(0, 0, window_dimensions.x, window_dimensions.y);

    glfwSetKeyCallback(window, key_callback);

    pthread_t thread = pthread_self();
    logger->debug("Initialized context in thread {}", thread);

    return 0;
}

glfw_gl_window::~glfw_gl_window() {
    glfwTerminate();
}

void glfw_gl_window::destroy() {
    glfwDestroyWindow(window);
    window = NULL;
}

void glfw_gl_window::set_fullscreen(bool fullscreen) {
    // TODO: RAVEN WRIT THIS
}

bool glfw_gl_window::should_close() {
    return (bool) glfwWindowShouldClose(window);
}

glm::vec2 glfw_gl_window::get_size() {
    return window_dimensions;
}

void glfw_gl_window::end_frame() {
    glfwSwapBuffers(window);
    glfwPollEvents();

    glm::ivec2 new_window_size;
    glfwGetFramebufferSize(window, &new_window_size.x, &new_window_size.y);

    if(new_window_size != window_dimensions) {
        set_framebuffer_size(new_window_size);
    }
}

void glfw_gl_window::set_framebuffer_size(glm::ivec2 new_framebuffer_size) {
    window_dimensions = new_framebuffer_size;
    glViewport(0, 0, window_dimensions.x, window_dimensions.y);
}

void glfw_gl_window::on_config_change(nlohmann::json& new_config) {
    logger->info("gl_glfw_window received the updated config");
    glfwSetWindowSize(window, new_config["viewWidth"], new_config["viewHeight"]);
}

void glfw_gl_window::on_config_loaded(nlohmann::json &config) {
    // Don't do anything, we don't rely on any read-only config values... yet
}




