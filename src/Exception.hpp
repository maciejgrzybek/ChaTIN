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
public:
    virtual ~DataParsingException() throw();
    virtual const char* what() const throw();
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
public:
    virtual ~ClientSocketException() throw();
    virtual const char* what() const throw();
};

class WrongPortException : public SocketException,
                           public DataParsingException
{
public:
    WrongPortException(const int);
    virtual ~WrongPortException() throw();
    virtual const char* what() const throw();
protected:
    const int port;
    std::string message;
};

class WrongAddressException : public SocketException,
                              public DataParsingException
{
public:
    WrongAddressException(const std::string&, const int);
    virtual ~WrongAddressException() throw();
    const char* what() const throw();
protected:
    const std::string address;
    const int errorNumber;
    std::string message;
};

class ConnectionFailureException : public ClientSocketException
{
public:
    ConnectionFailureException(int);
    const char* what() const throw();
protected:
    int errorNumber;
};

class NotConnectedException : public ClientSocketException 
{
public:
    const char* what() const throw();
};

class ListenFailureException : public ServerSocketException
{
public:
    ListenFailureException(int);
    const char* what() const throw();
protected:
    int errorNumber;
};

class ResolveException : public SocketException
{
public:
    ResolveException(int);
    const char* what() const throw();
protected:
    int errorNumber;
};

class SelectFailureException : public SocketException
{
public:
    SelectFailureException(int);
    const char* what() const throw();
protected:
    int errorNumber;
};

class AcceptFailureException : public ServerSocketException
{
public:
    AcceptFailureException(int);
    const char* what() const throw();
protected:
    int errorNumber;
};

class ReceiveFailureException : public SocketException
{
public:
    ReceiveFailureException(int);
    const char* what() const throw();
protected:
    int errorNumber;
};

class SendFailureException : public SocketException
{
public:
    SendFailureException(int);
    const char* what() const throw();
protected:
    int errorNumber;
};

}; // namespace Socket

#endif
