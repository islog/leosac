/**
 * \file isignalcallback.hpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief signal callback interface to use with signalhandler
 */

#ifndef ISIGNALCALLBACK_HPP
#define ISIGNALCALLBACK_HPP

class ISignalCallback
{
public:
    virtual ~ISignalCallback() {}
    virtual void    handleSignal(int signal) = 0;
};

#endif // ISIGNALCALLBACK_HPP
