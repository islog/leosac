/**
 * \file rplethprotocol.hpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief Rpleth protocol implementation
 */

#ifndef RPLETHPROTOCOL_HPP
#define RPLETHPROTOCOL_HPP

#include "rplethpacket.hpp"
#include "network/circularbuffer.hpp"

#include <queue>

class RplethProtocol
{
    static const std::size_t    TypeByteIdx     = 0;
    static const std::size_t    CommandByteIdx  = 1;
    static const std::size_t    SizeByteIdx     = 2;
    static const std::size_t    PacketMinSize   = 4;

    RplethProtocol() = delete;

public:
    enum TypeCode {
        Rpleth  = 0x00,
        HID     = 0x01,
        LCD     = 0x02,
        MaxType
    };

    enum RplethCommands {
        DHCPState   = 0x01,
        SetDHCP     = 0x02,
        SetMAC      = 0x03,
        SetIP       = 0x04,
        SetSubnet   = 0x05,
        SetGateway  = 0x06,
        SetPort     = 0x07,
        Message     = 0x08, // NOTE not documented
        Reset       = 0x09,
        Ping        = 0x0a
    };

    enum HIDCommands {
        Beep            = 0x00,
        Greenled        = 0x01,
        Redled          = 0x02,
        Nop             = 0x03,
        Badge           = 0x04,
        Com             = 0x05,
        Wait_insertion  = 0x06,
        Wait_removal    = 0x07,
        Connect         = 0x08,
        Disconnect      = 0x09,
        GetReaderType   = 0x0a,
        GetCSN          = 0x0b,
        SetCardType     = 0x0c
    };

    enum StatusCode {
        Success     = 0x00,
        Failed      = 0x01,
        BadChecksum = 0x02,
        Timeout     = 0x03,
        BadSize     = 0x04,
        BadType     = 0x05,
        MaxStatus
    };

public:
    static RplethPacket decodeCommand(CircularBuffer& buffer);
    static std::size_t  encodeCommand(const RplethPacket& packet, Byte* buffer, std::size_t size);
    static RplethPacket processClientPacket(const RplethPacket& packet);
};

#endif // RPLETHPROTOCOL_HPP
