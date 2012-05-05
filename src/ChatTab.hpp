#pragma once
#include "types.hpp"
#include "FromViewParser.hpp"
#include <gtkmm.h>
#include <glibmm/ustring.h>

/**
 * Enum used to recognize type in up cast
 */
enum ChatTabType{ DIALOG, CONFERENCE, LOG };

/**
 * Class for holding separation between view-controll and view
 * It contains informations needed to comunicate
 * with FromViewParser.
 * @author: Andrzej Fiedukowicz
 * @see FromViewParser::doCommand
 */
class ChatTab : public Gtk::TextView
{
    public:
        virtual const Glib::ustring& getFullAlias() const = 0;
        virtual ChatTabType getType() const = 0;
        virtual void doCommand( FromViewParser&, Glib::ustring ) const = 0;
};

/**
 * This class contains ChaTIN::Alias which can be use to
 * call doCommand on FromViewParser
 */
class ChatTabDialog : public ChatTab
{
        const ChaTIN::Alias& alias;
    public:
        ChatTabDialog( const ChaTIN::Alias& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
        virtual void doCommand( FromViewParser&, Glib::ustring ) const;
};

/**
 * This class contains ChaTIN::ConferenceId which can be use to
 * call doCommand on FromViewParser
 */
class ChatTabConference : public ChatTab
{
        const ChaTIN::ConferenceId& name;
    public:
        ChatTabConference( const ChaTIN::ConferenceId& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
        virtual void doCommand( FromViewParser&, Glib::ustring ) const;
};

/**
 * This class contains Glib::ustring which can be use to
 * call doCommand on FromViewParser
 */
class ChatTabLog : public ChatTab
{
        const Glib::ustring& name;
    public:
        ChatTabLog( const Glib::ustring& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
        virtual void doCommand( FromViewParser&, Glib::ustring ) const;
};
