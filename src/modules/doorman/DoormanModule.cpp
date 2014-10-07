#include "DoormanModule.hpp"
#include "DoormanInstance.hpp"
#include "tools/log.hpp"

DoormanModule::DoormanModule(zmqpp::context &ctx,
        zmqpp::socket *pipe,
        const boost::property_tree::ptree &cfg) :
        ctx_(ctx),
        pipe_(*pipe),
        config_(cfg),
        is_running_(true)
{
    process_config();

    for (auto doorman : doormen_)
    {
        reactor_.add(doorman->bus_sub(),
                std::bind(&DoormanInstance::handle_bus_msg, doorman));
    }
    reactor_.add(pipe_, std::bind(&DoormanModule::handle_pipe, this));
}

DoormanModule::~DoormanModule()
{
    for (auto doorman : doormen_)
    {
        delete doorman;
    }
}

void DoormanModule::handle_pipe()
{
    zmqpp::signal s;

    pipe_.receive(s, true);
    if (s == zmqpp::signal::stop)
        is_running_ = false;
}

void DoormanModule::run()
{
    while (is_running_)
    {
        reactor_.poll(-1);
    }
}

void DoormanModule::process_config()
{
    boost::property_tree::ptree module_config = config_.get_child("module_config");

    for (auto &node : module_config.get_child("instances"))
    {
        // one doorman instance
        boost::property_tree::ptree cfg_doorman = node.second;

        std::vector<std::string> auth_ctx_names;
        std::vector<DoormanAction> actions;
        std::string doorman_name = cfg_doorman.get_child("name").data();
        int timeout = cfg_doorman.get<int>("timeout", 1000);

        for (auto &auth_contexts_node : cfg_doorman.get_child("auth_contexts"))
        {
            // each auth context we listen to
            auth_ctx_names.push_back(auth_contexts_node.second.get<std::string>("name"));
        }

        for (auto &action_node : cfg_doorman.get_child("actions"))
        {
            // every action we take
            boost::property_tree::ptree cfg_action = action_node.second;
            DoormanAction a;

            a.on_ = cfg_action.get<std::string>("on");
            a.target_ = cfg_action.get<std::string>("target");
            int frame_count = 1;
            for (auto &cmd_node : cfg_action.get_child("cmd"))
            {
                // each frame in command
                //fixme ORDER
                //fixme ONLY ONE FRAME
                a.cmd_.push_back(cmd_node.second.data());
            }
            actions.push_back(a);
        }

        LOG() << "Creating Doorman instance " << doorman_name;
        doormen_.push_back(new DoormanInstance(ctx_,
                doorman_name, auth_ctx_names, actions, timeout));
    }
}
