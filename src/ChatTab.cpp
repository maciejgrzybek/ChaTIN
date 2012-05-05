#include "ChatTab.hpp"

ChatTabDialog::ChatTabDialog( const ChaTIN::Alias& alias )
    : alias(alias)
{}

const Glib::ustring& ChatTabDialog::getFullAlias() const
{
    return alias;
}

ChatTabType ChatTabDialog::getType() const
{
    return DIALOG;
}


ChatTabConference::ChatTabConference( const ChaTIN::ConferenceId& name )
    : name(name)
{}

const Glib::ustring& ChatTabConference::getFullAlias() const
{
    return name.name;
}

ChatTabType ChatTabConference::getType() const
{
    return CONFERENCE;
}

ChatTabLog::ChatTabLog( const Glib::ustring& name )
    : name(name)
{}

const Glib::ustring& ChatTabLog::getFullAlias() const
{
    return name;
}

ChatTabType ChatTabLog::getType() const
{
    return LOG;
}
