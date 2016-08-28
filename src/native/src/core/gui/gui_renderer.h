/*!
 * \author David
 * \date 13-May-16.
 */

#ifndef RENDERER_GUI_RENDERER_H
#define RENDERER_GUI_RENDERER_H

#include <memory>
#include <atomic>

#include <spdlog/spdlog.h>

#include "core/uniform_buffer_store.h"
#include "interfaces/ivertex_buffer.h"
#include "mc/mc_gui_objects.h"
#include "core/texture_manager.h"
#include "core/shaders/uniform_buffer_definitions.h"
#include "shaderpack_loading/shaderpack.h"

/*!
 * \brief Defines a bunch of methods to render different GUI elements, including buttons, text, and images
 *
 * Something interesting to note - GUI layout is all calculated in pixels, then all the vertices are scaled down by the
 * view matrix.
 *
 * The GUI renderer works directly with the data received from Minecraft. There's no wrapper classes, no conversions to
 * a format the renderer can handle better. This is mostly done because I don't hae a good reason to do it another way.
 * The design of the GUI renderer is such that using the data from MC directly is simpler.
 *
 * What is the design of the GUI renderer, you ask? Well, let me tell you. The entire GUI is laid out in a single VBO.
 * I use atlases to have the textures for multiple GUI elements available. MC does this too, but I shove every
 * GUI-related texture into the same atlas. This includes the current font. Then, when the GUI receives a GUI screen, I
 * compare it to the previous GUI screen. If it's exactly the same, I don't re-build the GUI VBO. If it's the same
 * screen but a button is pressed or an item is highlighted or something, I only change the updated element(s). I only
 * completely rebuild the GUI geometry when the new gui screen is different from the current GUI screen. I do this so I
 * don't have to send a lot of information to the GPU. While it's true that the GUI geometry wil be relatively small and
 * probably not a bottleneck, I want to ensure that every part of this mod is built for speed and efficiency.
 */
class gui_renderer {
public:
    gui_renderer(texture_manager& textures, shaderpack& shaders, uniform_buffer_store& uniform_buffers);
    ~gui_renderer();

    /*!
     * \brief Sets up the uniform buffer for GUI geometry
     *
     * This needs to be a separate method. I can't use RAII because the uniform buffer needs OpenGL calls, which aren't
     * available until after I create a window, because coupling is weird.
     *
     * So this method needs to be called, but I can't enforce is being called. Whoops.
     */
    void setup_buffers();

    /*!
     * \brief Sets the GUI screen to render as the given screen
     */
    void set_current_screen(mc_gui_screen* screen);

    /*!
     * \brief Renders the current GUI screen
     *
     * Note: for right now, it just draws the unpressed button, just to prove that I can
     */
    void render();

    /*!
     * \brief Checks to see if a new GUI screen is available. If so, sets the current GUI screen to the new GUI screen
     * and re-builds GUI geometry
     */
    void update();

private:
    std::vector<float> basic_unpressed_uvs = {
            0.0f,       0.3359375f,
            0.78125f,   0.3359375f,
            0.0f,       0.4156963f,
            0.78125f,   0.4156963f
    };

    std::vector<float> basic_pressed_uvs = {
            0.0f,       0.2578112f,
            0.78125,    0.2578112f,
            0.0f,       0.3359375f,
            0.78125f,   0.3359375f
    };

    std::vector<unsigned short> index_buffer = {
            0, 1, 2,
            2, 1, 3
    };

    mc_gui_screen cur_screen;

    texture_manager& tex_manager;
    shaderpack& shaders;
    uniform_buffer_store & ubo_manager;

    std::string WIDGETS_TEXTURE_NAME = "textures/gui/widgets.png";
    std::string GUI_SHADER_NAME = "gui";

    std::unique_ptr<ivertex_buffer> cur_screen_buffer;

    // Memory that will be accessed from both the render thread and the Java thread
    mc_gui_screen new_screen;
    std::atomic<bool> has_screen_available;
    
    std::shared_ptr<spdlog::logger> logger;

    /*!
     * \brief Compares two mc_gui_screen objects, determining if they represent the same visual data
     *
     * I'l like to have had this function in the same header file as the mc_gui_screen struct. However, I need
     * mc_gui_screen to be a C struct so I can properly assign to it from Java. The compiler yelled at me about "You
     * can't compare structs" so I couldn't use the == operator and here we are.
     */
    bool is_different_screen(mc_gui_screen& screen1, mc_gui_screen & screen2) const;

    /*!
     * \brief Determines whether or not the two given buttons are the same
     *
     * Two buttons are the same if they have the same position, size, and pressed status. If they can be drawn using
     * the exact same geometry and texture, then they are the same.
     *
     * \param button1 The first button to compare
     * \param button2 The second button to compare
     *
     * \return True if the buttons are the same, false otherwise
     */
    bool same_buttons(mc_gui_button& button1, mc_gui_button& button2) const;

    /*!
     * \brief Constructs the geometry needed to render the current GUI screen
     *
     * Note that the GUI screen does not include things like the spinning background on the main menu screen, because
     * that's going to be rendered as if it was a scene
     */
    void build_gui_geometry();

    /*!
     * \brief Adds the vertex with the given parameters to the given vertex buffer
     *
     * Note that the z position of the vertices is always set to 0. This is maybe what I want.
     *
     * \param vertex_buffer The thing to add vertices to
     * \param x The x position of the vertex
     * \param y The y position of the vertex
     * \param u The u texture coordiante of the vertex
     * \param v The v texture coordinate of the vertex
     */
    void add_vertex(std::vector<float> &vertex_buffer, int x, int y, float u, float v);

    /*!
     * \brief Adds all the vertices from the given button to the given vertex buffer. uvs holds the uv
     * coordinates for this button
     *
     * \param vertex_buffer The vertex buffer to add vertices to
     * \param button The button to get vertices from
     * \param uvs The uv coordinates to use for this button
     */
    void add_vertices_from_button(std::vector<float> &vertex_buffer, const mc_gui_button &button,
                                  const std::vector<float> &uvs);

    void add_indices_for_button(std::vector<unsigned short> &indices, unsigned short start_pos);
};


#endif //RENDERER_GUI_RENDERER_H
