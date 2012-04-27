#include "Exception.hpp"
#include <unistd.h>
#include <netdb.h>
#include <cstring>

const char* Exception::what() const throw()
{
    return "Undefined ChaTIN exception";
}

const char* DataParsingException::what() const throw()
{
    return "Data parsing exception";
}

namespace Socket
{

    const char* SocketException::what() const throw()
    {
        return "Undefined socket exception";
    }

    const char* ServerSocketException::what() const throw()
    {
        return "Undefined server socket exception";
    }

    const char* ClientSocketException::what() const throw()
    {
        return "Undefined client socket exception";
    }

    WrongPortException::WrongPortException(const int port) : port(port)
    {
        message = "Wrong port. Should be value from 0 to 65535. You typed: ";
        message += port;
    }

    const char* WrongPortException::what() const throw()
    {
        return message.c_str();
    }

    WrongAddressException::WrongAddressException(const std::string &address, const int errorNumber) : address(address), errorNumber(errorNumber)
    {
        message = "Wrong address. Should be in proper IPv6 format. You typed: ";
        message += address;
        message += strerror(errorNumber);
    }

    const char* WrongAddressException::what() const throw()
    {
        return message.c_str();
    }

    ConnectionFailureException::ConnectionFailureException(int errorNumber) : errorNumber(errorNumber)
    {
    }

    const char* NotConnectedException::what() const throw()
    {
        return "Not connected exception.";
    }

    ListenFailureException::ListenFailureException(int errorNumber) : errorNumber(errorNumber)
    {
    }

    const char* ListenFailureException::what() const throw()
    {
        return strerror(errorNumber);
    }

    SelectFailureException::SelectFailureException(int errorNumber) : errorNumber(errorNumber)
    {
    }

    const char* SelectFailureException::what() const throw()
    {
        return strerror(errorNumber);
    }

    const char* ConnectionFailureException::what() const throw()
    {
        return strerror(errorNumber); 
    }

    AcceptFailureException::AcceptFailureException(int errorNumber) : errorNumber(errorNumber)
    {
    }

    const char* AcceptFailureException::what() const throw()
    {
        return strerror(errorNumber);
    }

    ReceiveFailureException::ReceiveFailureException(int errorNumber) : errorNumber(errorNumber)
    {
    }

    const char* ReceiveFailureException::what() const throw()
    {
        return strerror(errorNumber);
    }

    SendFailureException::SendFailureException(int errorNumber) : errorNumber(errorNumber)
    {
    }

    const char* SendFailureException::what() const throw()
    {
        return strerror(errorNumber);
    }

    ResolveException::ResolveException(int errorNumber) : errorNumber(errorNumber)
    {
    }

    const char* ResolveException::what() const throw()
    {
        return gai_strerror(errorNumber);
    }

}; // namespace Socket
