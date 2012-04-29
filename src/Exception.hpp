#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
    virtual const char* what() const throw();
};

class DataParsingException : public Exception
{
public:
    virtual const char* what() const throw();
};

class FileOpenException : public Exception
{
public:
    FileOpenException(const std::string&);
    virtual const char* what() const throw();
    virtual ~FileOpenException() throw();
protected:
    std::string message;
};

class WrongTypeOfValueException : public DataParsingException
{
public:
    WrongTypeOfValueException(const std::string&, const std::string&);
    virtual const char* what() const throw();
};

namespace Socket
{

class SocketException : public Exception
{
public:
    virtual const char* what() const throw();
};

class SocketCreationFailureException : public SocketException
{
public:
    SocketCreationFailureException(int);
    virtual const char* what() const throw();
protected:
    int errorNumber;
};

class ServerSocketException : public SocketException
{
public:
    virtual const char* what() const throw();
};

class ClientSocketException : public SocketException
{
public:
    virtual const char* what() const throw();
};

class WrongPortException : public SocketException,
                           public DataParsingException
{
public:
    WrongPortException(const int);
    ~WrongPortException() throw();
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
    ~WrongAddressException() throw();
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
