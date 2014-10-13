#pragma once

#include <string>
#include <chrono>
#include <zmqpp/socket.hpp>

/**
* A Facade to a LED object.
*
* This object require a valid GPIO object to work. It abstract a LED, and add a simple command on top of GPIO.
* It requires a module that implements the behavior to work properly. However, unlike GPIO modules, we are unlikely
* to have multiple implementation of a LED module.
*
* ### Configuration Requirement
* When defining a LED device in a configuration file, 2 optional configuration parameters shall be available.
*     1. `default_blink_duration` which defines how long a LED will blink if no blink duration is specified.
*     2. `default_blink_speed` which defines how fast a led will blink.
*
* Both theses options are expressed in milliseconds.
*
*
* ### Find below the LED object specifications:
*
* Note that GPIO commands are also valid LED command.
*
* Commands are sent (and response received) using REQ/REP socket. Here is a message specs:
*    1. Frame 1: `COMMAND_NAME`
*    2. Frame 2: `PARAMETER_1`
*    3. Frame 3: `PARAMETER_2`
*
* See command description for more info about parameter.
* - - - - -
*
* We define 4 commands that can be send to a LED device:
*    + `ON` to turn the LED on.
*    + `TOGGLE` the LED: if on if goes off; if off it goes on.
*    + `OFF` to turn the LED off.
*    + `BLINK` so that the LED will blink.
*
* #### `ON`
* This turns the LED on. It accepts an optional `duration` parameter.
* If set, this parameter express the `duration` for which the LED shall stay on.
* This `duration` is expressed in milliseconds.
*
* The implementation shall turn the LED off after this `duration` has expired.
*
* #### `OFF`
* This turns the LED low. There is no parameter.
*
* #### `TOGGLE`
* Toggle the LED, setting it to low it was set to high, and vice versa. This command
* doesn't expect any parameter either.
*
* #### `BLINK`
* This makes the link blink, useful for controlling your christmas tree.
* The `BLINK` command accepts 2 optionals parameter: a `duration` and a `speed`. Both are expressed in milliseconds.
*
* The second frame shall contain the duration (use -1 for infinite blink) and the third frame the speed.
*/
class FLED
{
    FLED(zmqpp::context &ctx, const std::string &gpio_name);

    /**
    * Disabled copy-constructor.
    * Manually create a new facade using the LED's name instead.
    */
    FLED(const FLED &) = delete;

    /**
    * Default destructor
    */
    ~FLED() = default;

    /**
    * Turn the LED ON and turn it OFF duration milliseconds later.
    */
    bool turnOn(std::chrono::milliseconds duration);

    /**
    * Turn the LED ON by sending a message to the backend LED impl.
    */
    bool turnOn();

    /**
    * Turn the LED OFF by sending a message to the backend LED impl.
    */
    bool turnOff();

    /**
    * Toggle the LED value by sending a message to the backend LED impl.
    */
    bool toggle();

    /**
    * Make the LED blink. No optional parameter so the module shall use the default for the device.
    */
    bool blink();

    /**
    * Blink with a duration and a speed.
    */
    bool blink(std::chrono::milliseconds duration, std::chrono::milliseconds speed);

private:
    /**
    * A socket to talk to the backend GPIO.
    */
    zmqpp::socket backend_;
};
