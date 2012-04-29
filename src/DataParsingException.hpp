#ifndef _DATA_PARSING_EXCEPTION_HPP_
#define _DATA_PARSING_EXCEPTION_HPP_

class DataParsingException : public Exception
{
public:
    virtual const char* what() const throw();
};

#endif
