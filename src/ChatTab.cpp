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

bool ChatTabDialog::operator==( const ChaTIN::LogName&     ) const
{
    return false;
}
bool ChatTabDialog::operator==( const ChaTIN::Alias& alias ) const
{
    return this->alias == alias;
}
bool ChatTabDialog::operator==( const ChaTIN::ConferenceId& ) const
{
   return false;
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

bool ChatTabConference::operator==( const ChaTIN::LogName&        ) const
{
    return false;
}
bool ChatTabConference::operator==( const ChaTIN::Alias&        ) const
{
    return false;
}
bool ChatTabConference::operator==( const ChaTIN::ConferenceId& name ) const
{
    return this->name == name;
}



ChatTabLog::ChatTabLog( const ChaTIN::LogName& name )
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

bool ChatTabLog::operator==( const ChaTIN::LogName& name) const
{
    return this->name == name;
}
bool ChatTabLog::operator==( const ChaTIN::Alias&        ) const
{
    return false;
}
bool ChatTabLog::operator==( const ChaTIN::ConferenceId& ) const
{
    return false;
}


