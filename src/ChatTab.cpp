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

EPtr ChatTabDialog::createEvent( const Glib::ustring& text ) const
{
    return EPtr( new EventDialog( alias, text ) );
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

EPtr ChatTabConference::createEvent( const Glib::ustring& text ) const
{
    return EPtr( new EventConference( name, text ) );
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

EPtr ChatTabLog::createEvent( const Glib::ustring& text ) const
{
    return EPtr( new EventLog( name,text ) );
}
