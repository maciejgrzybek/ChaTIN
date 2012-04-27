#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <exception>
#include <string>

class Exception : public std::exception
{
    public:
    virtual ~Exception() throw();
    virtual const char* what() const throw();
};

class DataParsingException : public Exception
{
//FIXME: implement this
};

namespace Socket
{

class SocketException : public Exception
{
public:
    virtual ~SocketException() throw();
    virtual const char* what() const throw();
};

class ServerSocketException : public SocketException
{
public:
    virtual ~ServerSocketException() throw();
    virtual const char* what() const throw();
};

class ClientSocketException : public SocketException
{
//FIXME: implement this
};

class WrongPortException : public SocketException,
                           public DataParsingException
{
public:
    WrongPortException(const int);
//FIXME: implement this
};

class WrongAddressException : public SocketException,
                              public DataParsingException
{
public:
    WrongAddressException(int);
    virtual ~WrongAddressException() throw();
    virtual const char* what() const throw();
private:
    std::string address;
};

class ConnectionFailureException : public ClientSocketException
{
//FIXME: implement this like above
public:
    ConnectionFailureException(int);
};

class NotConnectedException : public ClientSocketException 
{
//FIXME: implement this like above
};

class ListenFailureException : public ServerSocketException
{
//FIXME: implement this like above
public:
    ListenFailureException(int);
};

class ResolveException : public SocketException
{
public:
    ResolveException(int);
//FIXME: implement this like above
};

class SelectFailureException : public SocketException
{
public:
    SelectFailureException(int);
//FIXME: implement this like above
};

class AcceptFailureException : public ServerSocketException
{
public:
    AcceptFailureException(int);
//FIXME: implement this like above
};

class ReceiveFailureException : public SocketException
{
//FIXME: implement this like above
public:
    ReceiveFailureException(int);
};

class SendFailureException : public SocketException
{
//FIXME: implement this like above
public:
    SendFailureException(int);
};

}; // namespace Socket

#endif
