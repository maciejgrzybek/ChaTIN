#pragma once
#include "types.hpp"
#include <memory>

class ChatTab;
typedef std::shared_ptr<ChatTab> TPtr;

#include "FromViewParser.hpp"
#include "Event.hpp"
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
        /**
         * Default constructor - set loaded history to false
         */
        ChatTab();
        
        /**
         * Destructor
         */
        virtual ~ChatTab();

        /**
         * Return sth that can be used to represent
         * tab by view
         */
        virtual const Glib::ustring& getFullAlias() const = 0;

        /**
         * return the ChatTabType enum (use from upcast)
         */
        virtual ChatTabType getType() const = 0;

        /**
         * calls doCommand with proper type argument
         * and Glib::ustring as input on FromViewParser
         * @param FromViewParser& parser to call doCommand on
         * @param Glib::ustring input from user
         * @see FromViewParser::doCommand
         */
        virtual EPtr createEvent( const Glib::ustring& ) const = 0;
        // TODO it can just return bind to doCommand prepared for FormViewParser
        //          with first argument binded. To have the call in ChatWindow:
        //      fromViewParser.(selectedTab->getDoCommand())(chatField.get_text());
        virtual bool operator==( const ChaTIN::TabId&        ) const = 0;
        
        bool getHistoryLoaded();
        void setHistoryLoaded();

    private:        
        bool historyLoaded;
};

/**
 * This class contains ChaTIN::Alias which can be use to
 * call doCommand on FromViewParser
 */
class ChatTabDialog : public ChatTab
{
        ChaTIN::Alias alias;
    public:
        ChatTabDialog( const ChaTIN::Alias& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
        virtual EPtr createEvent( const Glib::ustring&  ) const;
        virtual bool operator==( const ChaTIN::TabId&        ) const;
};

/**
 * This class contains ChaTIN::ConferenceId which can be use to
 * call doCommand on FromViewParser
 */
class ChatTabConference : public ChatTab
{
        ChaTIN::ConferenceId name;
    public:
        ChatTabConference( const ChaTIN::ConferenceId& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
        virtual EPtr createEvent( const Glib::ustring& ) const;
        virtual bool operator==( const ChaTIN::TabId&        ) const;
};

/**
 * This class contains Glib::ustring which can be use to
 * call doCommand on FromViewParser
 */
class ChatTabLog : public ChatTab
{
        ChaTIN::LogName name;
    public:
        ChatTabLog( const ChaTIN::LogName& );
        const Glib::ustring& getFullAlias() const;
        ChatTabType getType() const;
        virtual EPtr createEvent( const Glib::ustring& ) const;
        virtual bool operator==( const ChaTIN::TabId&        ) const;
};

