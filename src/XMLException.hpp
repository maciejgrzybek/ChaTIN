#ifndef _XML_PARSING_EXCEPTION_HPP_
#define _XML_PARSING_EXCEPTION_HPP_

#include "DataParsingException.hpp"

namespace XML
{
class XMLParsingException : public DataParsingException
{
public:
    XMLParsingException();
    XMLParsingException(const std::string&);
    ~XMLParsingException() throw();
    virtual const char* what() const throw();
protected:
    std::string message;
};

class WrongTypeOfValueException : public XMLParsingException
{
public:
    WrongTypeOfValueException(const std::string&, const std::string&);
    ~WrongTypeOfValueException() throw();
    virtual const char* what() const throw();
protected:
    std::string message;
};

class ValueNotExistsException : public XMLParsingException
{
public:
    ValueNotExistsException(const std::string&);
    virtual ~ValueNotExistsException() throw();
    virtual const char* what() const throw();
protected:
    std::string message;
};

}; //namespace XML

#endif
