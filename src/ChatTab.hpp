#pragma once
#include "types.hpp"
#include <glibmm/ustring.h>

enum ChatTabType{ DIALOG, CONFERENCE, LOG };

/**
 * Class for holding separation between view-controll and view
 * It contains  fullAlias of tab as well as it is Widget
 * @author: Andrzej Fiedukowicz
 */
class ChatTab : public Gtk::TextView
{
    public:
        virtual const Glib::ustring& getFullAlias() const = 0;
        virtual ChatTabType getType() const = 0;
};

class ChatTabDialog : public ChatTab
{
        const ChaTIN::Alias& alias;
    public:
        ChatTabDialog( const ChaTIN::Alias& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
};

class ChatTabConference : public ChatTab
{
        const ChaTIN::ConferenceId& name;
    public:
        ChatTabConference( const ChaTIN::ConferenceId& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
};

class ChatTabLog : public ChatTab
{
        const Glib::ustring& name;
    public:
        ChatTabLog( const Glib::ustring& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
};
