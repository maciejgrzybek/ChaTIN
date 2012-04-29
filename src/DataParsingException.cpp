const char* Exception::what() const throw()
{
    return "Undefined ChaTIN exception";
}

const char* DataParsingException::what() const throw()
{
    return "Data parsing exception";
}
