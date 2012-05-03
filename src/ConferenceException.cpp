#include "ConferenceException.hpp"

ConferenceNotExistsException::ConferenceNotExistsException(const ChaTIN::ConferenceId& conferenceId)
{
    message = "Conference does not exist exception. Conference name: "+std::string(conferenceId.name.c_str())+" IP: "+std::string(static_cast<std::string>(conferenceId.ownerip));
}

ConferenceNotExistsException::~ConferenceNotExistsException() throw()
{
}

const char* ConferenceNotExistsException::what() const throw()
{
    return message.c_str();
}
