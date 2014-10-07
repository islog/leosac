#pragma once
#include <list>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <dynlib/dynamiclibrary.hpp>
#include <modules/moduleconfig.h>
#include <boost/property_tree/ptree.hpp>

#include "zmqpp/actor.hpp"

/**
* A second module manager that loads "ZMQ aware" module -- modules that talks to the application through message passing.
* A second loader is necessary because the procedure of loading a module differs.
* No reference to an object is returned when loading a module. The module will lives in its own thread and other components
* can talk to it.
*
* @note: Only the zModuleManager object has "direct access" to the module object. It is zmqpp::actor launch the module
* in its own thread.
*
* @note: Use the "level" property to define module initialization order.
* This initialization order is mandatory, and the lower the value is, the sonner the module is loaded.
*/
class zModuleManager
    {
public:
    /**
    * Contruct the module manager.
    * @param ctx the zeroMQ context to pass around to module.
    */
    zModuleManager(zmqpp::context &ctx);
    ~zModuleManager();

    void        unloadLibraries();

    /**
    * Actually call the init_module() function of each library we loaded.
    * The module initialization order in honored (see the "level" property).
    * @throws: may throw ModuleException if init_module() fails for a library (or actor init exception).
    */
    bool        initModules();

    /**
    * Opposite of init module. this stop all modules thread and perform cleanup.
    * To dynamic library handlers are NOT released.
    */
    void        stopModules();

    /**
    * Add a directory to a path. If the path already exist, it is ignored.
    */
    void addToPath(const std::string &dir);

    /**
    * Search the path and load a module based on a property tree for this module.
    * Mandatory data are the module "name" and the "file" location (the name of the .so to load).
    *
    * The first file to match (looping over the path array) will be loaded.
    */
    bool loadModule(const boost::property_tree::ptree &cfg);

private:
    /**
    * This will load (actually calling dlopen()) the library file located at full_path.
    * It returns a DynamicLibrary pointer that can be used to retrieve symbol from the shared object.
    * It it failed, returns nullptr.
    */
    DynamicLibrary    *load_library_file(const std::string &full_path);

    /**
    * Internal helper struct that store informations related to module
    * that are useful to the module manager.
    */
    struct ModuleInfo
    {
        /**
    * Name of the module, as specified in the configuration file.
    */
    std::string name_;

        /**
        * Pointer to the library object.
        */
        DynamicLibrary *lib_;

        /**
        * Config tree for the module.
        */
        boost::property_tree::ptree config_;

        /**
        * Comparaison operator: used by the module manager set.
        * We do not really care about uniqueness, we use this
        * to sort module by their loading priority.
        */
        bool operator<(const ModuleInfo &o) const
        {
            int level_me = config_.get<int>("level", 100);
            int level_o = o.config_.get<int>("level", 100);

            return level_me < level_o;
        }
    };

    std::vector<std::string> path_;
    std::set<ModuleInfo> BLABLA_;

    /**
    * Since module live is their own thread, we use the actor pattern.
    */
    std::vector<zmqpp::actor> modules_;

    zmqpp::context &ctx_;
};