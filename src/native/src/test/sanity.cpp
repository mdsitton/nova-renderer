/*!
 * \author David
 * \date 19-May-16.
 */

#include <cassert>
#include <spdlog/spdlog.h>
#include "sanity.h"
#include <glad/glad.h>
#include "test_utils.h"

static void sanity::check_gl_version() {
    int gl_version_major;
    int gl_version_minor;

    glGetIntegerv(GL_MAJOR_VERSION, &gl_version_major);
    glGetIntegerv(GL_MINOR_VERSION, &gl_version_minor);

    spdlog::get("nova")->info("Using OpenGL version {}.{}", gl_version_major, gl_version_minor);

    assert(gl_version_major == 4);
    assert(gl_version_minor == 5);
}

void sanity::run_all() {
    run_test(check_gl_version, "check_gl_version");
}
