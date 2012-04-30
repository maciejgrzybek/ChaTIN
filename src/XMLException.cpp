#include "XMLException.hpp"

namespace XML
{

XMLParsingException::XMLParsingException()
{
    message = "XML parsing exception";
}

XMLParsingException::XMLParsingException(const std::string& fileName)
{
    message = "XML parsing exception (filename: "+fileName+")";
}

XMLParsingException::~XMLParsingException() throw()
{
}

const char* XMLParsingException::what() const throw()
{
    return message.c_str();
}

WrongTypeOfValueException::WrongTypeOfValueException(const std::string& type, const std::string& value)
{
    message = "Wrong type of value exception. Got value: "+value+" is not required type: "+type;
}

WrongTypeOfValueException::~WrongTypeOfValueException() throw()
{
}

const char* WrongTypeOfValueException::what() const throw()
{
    return message.c_str();
}

ValueNotExistsException::ValueNotExistsException(const std::string& key)
{
    message = "Value does not exist at key: "+key;
}

ValueNotExistsException::~ValueNotExistsException() throw()
{
}

const char* ValueNotExistsException::what() const throw()
{
    return message.c_str();
}
}; //namespace XML
