#pragma once
#include <glibmm/ustring.h>
#include <memory>

class Event;
typedef std::shared_ptr<Event> EPtr;

#include "FromViewParser.hpp"
#include "types.hpp"

class FromViewParser;

class Event
{
    protected:
    Glib::ustring input;
    Event(const Glib::ustring& input);
    public:
    virtual void doCommand(FromViewParser&) = 0;    
};

class EventDialog : public Event
{
    ChaTIN::Alias alias;
    public:
    EventDialog(const ChaTIN::Alias& alias, const Glib::ustring& input);
    virtual void doCommand(FromViewParser&);    
};

class EventConference : public Event
{
    ChaTIN::ConferenceId name;
    public:
    EventConference(const ChaTIN::ConferenceId& name, const Glib::ustring& input);
    virtual void doCommand(FromViewParser&);
};

class EventLog : public Event
{
    Glib::ustring name;
    public:
    EventLog(const Glib::ustring& name, const Glib::ustring& input);
    virtual void doCommand(FromViewParser&);
};

