#ifndef _CONFERENCE_HPP_
#define _CONFERENCE_HPP_

#include "Exception.hpp"
#include "types.hpp"

class ConferenceNotExistsException : public Exception
{
public:
    ConferenceNotExistsException(const ChaTIN::ConferenceId&);
    virtual const char* what() const throw();
    virtual ~ConferenceNotExistsException() throw();
protected:
    std::string message;
};

#endif
