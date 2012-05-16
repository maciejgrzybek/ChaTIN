#include "Event.hpp"
#include <glibmm/ustring.h>
#include "types.hpp"


Event::Event(const Glib::ustring& input) : input(input)
{}


EventDialog::EventDialog(const ChaTIN::Alias& alias, const Glib::ustring& input) 
    : Event(input), alias(alias)
{}
void EventDialog::doCommand(FromViewParser& p)
{
    p.doCommand(alias,input);
}

EventConference::EventConference(const ChaTIN::ConferenceId& name, const Glib::ustring& input)
    : Event(input), name(name)
{}
void EventConference::doCommand(FromViewParser& p)
{
    p.doCommand(name,input);
}

EventLog::EventLog(const Glib::ustring& name, const Glib::ustring& input)
    : Event(input), name(name)
{}
void EventLog::doCommand(FromViewParser& p)
{
    p.doCommand(name,input);
}
