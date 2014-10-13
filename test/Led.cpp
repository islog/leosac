#include <tools/runtimeoptions.hpp>
#include <string>
#include "modules/led/LEDModule.hpp"
#include "helper/TestHelper.hpp"
#include "hardware/device/FLED.hpp"

bool run_module(zmqpp::context *ctx, zmqpp::socket *pipe)
    {
    boost::property_tree::ptree cfg, module_cfg, leds_cfg, led1_cfg;

    led1_cfg.add("name", "a_nice_led");
    led1_cfg.add("gpio", "a_cool_gpio");
        led1_cfg.add("default_blink_duration", "1000");
        led1_cfg.add("default_blink_speed", "100");

    leds_cfg.add_child("led", led1_cfg);
    module_cfg.add_child("leds", leds_cfg);
    cfg.add_child("module_config", module_cfg);

        return test_run_module<LEDModule>(ctx, pipe, cfg);
    }

bool bus_read_extract(zmqpp::message *m)
{
    return true;
}

template<typename T, typename ...Content>
bool bus_read_extract(zmqpp::message *m, T first_arg, Content... content)
{
    T value;
    *m >> value;

    if (value != first_arg)
    {
        std::cout << "LAM2A {" << value << "}, {" << first_arg << "}" << std::endl;
        return false;
    }
    return bus_read_extract(m, content...);
}

template<typename ...Content>
bool bus_read_extract(zmqpp::message *m, const char * first_arg, Content... content)
{
    std::string value;
    *m >> value;

    if (strcmp(value.c_str(), first_arg) != 0)
    {
        std::cout << "LAM3{" << value << "}, {" << first_arg << "}" << std::endl;
        return false;
    }
    return bus_read_extract(m, content...);
}

/**
* Make a blocking read on the bus, return true if content match the message.
* false otherwise.
* false if cannot read.
*/
template<typename ...Content>
bool bus_read(zmqpp::socket &sub, Content... content)
{
    zmqpp::message msg;

    if (!sub.receive(msg))
    {
        std::cout << "LAMA" << std::endl;
        return false;
    }

    return bus_read_extract(&msg, content...);
}

/**
* Test that we can turn a led ON, and that it will tell the GPIO to turn ON.
*/
TEST(Led, turnOn)
{
    zmqpp::context ctx;
    MessageBus bus(ctx);
    zmqpp::socket bus_sub(ctx, zmqpp::socket_type::sub);

FakeGPIO gpio(ctx, "a_cool_gpio");
zmqpp::actor my_gpio_actor(std::bind(&FakeGPIO::run, &gpio, std::placeholders::_1));

    std::vector<std::shared_ptr<FakeGPIO>> gpios;

    bus_sub.connect("inproc://zmq-bus-pub");
    bus_sub.subscribe("S_a_cool_gpio");
    zmqpp::actor a(std::bind(&run_module, &ctx, std::placeholders::_1));

FLED my_led(ctx, "a_nice_led");
ASSERT_TRUE(my_led.turnOn());
ASSERT_TRUE(bus_read(bus_sub, std::string("S_a_cool_gpio"), std::string("ON")));

ASSERT_TRUE(my_led.turnOff());
ASSERT_TRUE(bus_read(bus_sub, "S_a_cool_gpio", "OFF"));

ASSERT_TRUE(my_led.toggle());
ASSERT_TRUE(bus_read(bus_sub, "S_a_cool_gpio", "ON"));
ASSERT_TRUE(my_led.toggle());
ASSERT_TRUE(bus_read(bus_sub, "S_a_cool_gpio", "OFF"));
}
