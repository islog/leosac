/**
 * \file hwmanager.cpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief hardware managing class
 */

#include "hwmanager.hpp"

#include "device/gpiomanager.hpp"

HWManager::HWManager() {}

HWManager::~HWManager() {}

GPIO* HWManager::reserveGPIO(int id)
{
    return (_gpioManager.reserve(id));
}
