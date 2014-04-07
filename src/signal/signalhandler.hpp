/**
 * \file signalhandler.hpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief signal handler to provide a C++ interface for UNIX sigaction()
 */

#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP

class ISignalCallback;

class SignalHandler
{
public:
    static void registerCallback(ISignalCallback* callback);
};

#endif // SIGNALHANDLER_HPP
