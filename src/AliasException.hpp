#ifndef ALIAS_EXCEPTION_HPP
#define ALIAS_EXCEPTION_HPP

#include <string>

#include "types.hpp"

class AliasException : public std::exception
{
//FIXME implement this
};

class AliasDoesNotExistException : public AliasException
{
public:
    AliasDoesNotExistException(ChaTIN::Alias alias)
    {
        exceptionMsg = "Alias does not exist: " + alias;
    }
    
    ~AliasDoesNotExistException() throw()
    {}

    const char* what() const throw()
    {
        return exceptionMsg.c_str();
    }

private:
    std::string exceptionMsg;
};

#endif
