#include <chrono>

#pragma once

namespace Leosac
{
    namespace Auth
    {
        /**
        * Holds information about access permission
        */
        class IAccessProfile
        {
        public:
            /**
            * Does the profile allow access to the user.
            *
            * @param date the time point at which we try to get access
            * @param target the door we want to open
            */
            virtual bool isAccessGranted(const std::chrono::system_clock::time_point &date,
                    const std::string &target) = 0;
        };
    }
}
