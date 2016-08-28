/*!
 * \author David
 * \date 23-Jun-16.
 */

#ifndef RENDERER_CONFIG_H
#define RENDERER_CONFIG_H

#include <string>
#include <vector>
#include <fstream>

#include <json.hpp>
#include <spdlog/spdlog.h>

class config;

/*!
 * \brief Anything which inherits from this class wants to know about the configuration and any changes to it
 */
class iconfig_listener {
public:
    /*!
     * \brief Tells the listeners that there has been a change in the configuration
     *
     * This method is called throughout Nova's lifetime whenever a configuration value changes. This method should
     * handle changing configuration values such as the size of the window and what shaderpack the user has loaded
     *
     * Note that this method only recieves the read-write config values (the 'settings' node)
     *
     * \param new_config The updated configuration
     */
    virtual void on_config_change(nlohmann::json& new_config) = 0;

    /*!
     * \brief Tells listeners that the configuration has been loaded
     *
     * When Nova starts up, this method is called on all config listeners, then on_config_change is called.
     * on_config_change should be used to listen for any config values that change throughout the program's life, so
     * then this method should be used for any initial configuration whose values will not change throughout the
     * program's lifetime. An example of this is reading in the bind points of the UBOs: the bind points won't change
     * throughout the program's life, so they should be handled in this function
     *
     * We may want to consider two config files: one for read-only values and one for read-write values. Probably a
     * good idea, but I don't feel like implimenting that just yet
     *
     * \param config The configuration that was loaded
     */
    virtual void on_config_loaded(nlohmann::json& config) = 0;
};

/*!
 * \brief Holds the configuration of Nova
 *
 * Stores values like the graphics settings, performance settings, any settings shaderpacks define, etc. Uses a JSON
 * document as the data model
 */
class config {
public:
    /*!
     * \brief Constructs this config from the given JSON document
     *
     * \param filename The name of the file to load the config from
     */
    config(std::string filename);

    /*!
     * \brief Registers the given iconfig_change_listener as an Observer
     */
    void register_change_listener(iconfig_listener* new_listener);

    nlohmann::json& get_options();

    /*!
     * \brief Updates all the change listeners with the current state of the settings
     *
     * This method is public so that whatever changes values can delay calling it. You can set a bunch of options that
     * are pretty computationally intensive to change, the update listeners after all the values are changed
     *
     * Note that this method only send the read-write config values (children of the node 'settings') to the listeners
     */
    void update_config_changed();

    /*!
     * \brief Tells all the config listeners that the configuration has been loaded for the first time
     */
    void update_config_loaded();
private:
    nlohmann::json options;
    std::vector<iconfig_listener*> config_change_listeners;
    std::shared_ptr<spdlog::logger> logger;
};

#endif //RENDERER_CONFIG_H
