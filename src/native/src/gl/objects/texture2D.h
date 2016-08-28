//
// Created by David on 26-Apr-16.
//

#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H


#include <exception>
#include <vector>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

/*!
 * \brief Encapsulates the parameters for texture filtering
 */
class texture_filtering_params {
public:
    enum filter {
        POINT,
        BILINEAR,
        TRILINEAR,
    };

    filter texture_upsample_filter;
    filter texture_downsample_filter;

    int num_mipmap_levels;
    int anisotropic_level;
};

class texture_creation_exception : public std::exception {};
class texture_bind_exception : public std::exception {};

/*!
 * \brief Represents a two-dimensional OpenGL texture
 */
class texture2D {
public:
    /*!
     * \brief Instantiates a new texture2D, allocating a new texture object on the GPU
     */
    texture2D();

    /*!
     * \copydoc itexture::bind(unsigned int)
     *
     * For best performance, call this function once, do ABSOLUTELY EVERYTHING you need to do to this texture, then
     * unbind it
     *
     * It's worth noting that, if you give me the location that this texture is already bound to, this function won't
     * make any GL calls. For that reason, it is ABSOLUTELY IMPERATIVE that you call unbind. Always. Forever. When this
     * texture is no longer active, call unbind. Do it. You're going to forget, and you'll be really sad you can't find
     * the problem, then you're going to find the bit of code that doesn't bind the texture if it's already bound and
     * you'll see this comment and you'll understand what you did wrong.
     *
     * \param location The location to bind this texture to
     *
     * \throws texture_bind_exception if the texture can't be bound
     */
    virtual void bind(unsigned int location);

    virtual void unbind();

    /*!
     * \brief Sets this texture's data to the given parameters
     *
     * It's worth noting that this function doesn't do any validation on its data. You specified a LDR texture format
     * but you gave me HDR data? Sure hope the GPU can deal with that
     *
     * \param pixel_data The raw pixel_data
     * \param dimensions An array of the dimensions in this texture. For a texture2D that array MUST have two elements
     * \param format The format of the texture data
     */
    virtual void set_data(std::vector<float> & pixel_data, std::vector<int> & dimensions, GLenum format);

    virtual void set_filtering_parameters(texture_filtering_params & params);

    /*!
     * \brief Returns the width of this texture
     *
     * \return The width of this texture
     */
    int get_width();

    /*!
     * \brief returns the height of this texture
     *
     * \return The height of this texture
     */
    int get_height();

    /*!
     * \brief Returns the format of this texture, as assigned in the constructor
     *
     * \return The format of this texture
     */
    GLint get_format();

    /*!
     * \brief Returns the OpenGL identifier used to identify this texture
     */
    const unsigned int &get_gl_name();

private:
    int width;
    int height;
    GLint format;
    GLuint gl_name;
    GLint current_location = -1;
    std::shared_ptr<spdlog::logger> logger;
};


#endif //RENDERER_TEXTURE_H
