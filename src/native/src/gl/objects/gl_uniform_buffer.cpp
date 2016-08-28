/*!
 * \author David
 * \date 22-May-16.
 */

#include "gl_uniform_buffer.h"

gl_uniform_buffer::gl_uniform_buffer(GLuint size) {
    logger = spdlog::get("nova");
    glGenBuffers(1, &gl_name);
    bind();
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}

gl_uniform_buffer::gl_uniform_buffer(gl_uniform_buffer &&old) noexcept {
    gl_name = old.gl_name;
    bind_point = old.bind_point;
    old.gl_name = 0;
    old.bind_point = 0;
}

gl_uniform_buffer::~gl_uniform_buffer() {
    logger->trace("Deleting buffer {}", gl_name);
    glDeleteBuffers(1, &gl_name);
}

void gl_uniform_buffer::bind() {
    logger->trace("Binding buffer {}", gl_name);
    glBindBuffer(GL_UNIFORM_BUFFER, gl_name);
}

void gl_uniform_buffer::set_bind_point(GLuint bind_point) {
    logger->trace("Setting buffer {} to bind point {}", gl_name, bind_point);
    glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, gl_name);
    this->bind_point = bind_point;
}

void gl_uniform_buffer::operator=(gl_uniform_buffer && old) noexcept {
    gl_name = old.gl_name;
    bind_point = old.bind_point;
    old.gl_name = 0;
    old.bind_point = 0;
}

void gl_uniform_buffer::set_name(std::string name) noexcept {
    this->name = name;
}

const std::string & gl_uniform_buffer::get_name() const noexcept {
    return name;
}

GLuint gl_uniform_buffer::get_bind_point() const noexcept {
    return bind_point;
}

GLuint gl_uniform_buffer::get_gl_name() const noexcept {
    return gl_name;
}




