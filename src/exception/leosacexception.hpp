/**
 * \file leosacexception.hpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief Exception class for LEOSAC Project related errors
 */

#ifndef LEOSACEXCEPTION_HPP
#define LEOSACEXCEPTION_HPP

#include <exception>
#include <string>

class LEOSACException : public std::exception
{
public:
    explicit LEOSACException(const std::string& message) : _message(message) {}
    virtual ~LEOSACException() {}
    virtual const char* what() const noexcept final { return (_message.c_str()); }

private:
    const std::string   _message;
};

#endif // LEOSACEXCEPTION_HPP
