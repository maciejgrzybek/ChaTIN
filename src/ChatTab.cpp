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

void ChatTabDialog::doCommand( FromViewParser& fromViewParser, Glib::ustring text ) const
{
    fromViewParser.doCommand(alias, text);
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

void ChatTabConference::doCommand( FromViewParser& fromViewParser, Glib::ustring text ) const
{
    fromViewParser.doCommand(name, text);
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

void ChatTabLog::doCommand( FromViewParser& fromViewParser, Glib::ustring text ) const
{
    fromViewParser.doCommand(name, text);
}
