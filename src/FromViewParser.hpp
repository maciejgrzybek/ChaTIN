#pragma once
#include <glibmm/ustring.h>
#include "types.hpp"
#include "DialogManager.hpp"
#include "SafeQueue.hpp"
#include "Event.hpp"
#include "ChatWindow.hpp"
#include "AliasManager.hpp"

class ChatWindow;
class DialogManager;
class ConferenceManager;
class AliasManager;

/**
 * Parser class converting user input into method calls
 */
class FromViewParser
{
    DialogManager& dialogManager;
    AliasManager& aliasManager;
    ConferenceManager& cm;
    SafeQueue<EPtr>& bq;
    SafeQueue<Action>& aq;
    ChatWindow* cw;
    
    public:

    /**
     * Simple constructor giving paser reference to DialogManager
     * @param DialogManager& reference to the dialogManager to use in parser.
     */
    FromViewParser( DialogManager&, AliasManager&, ConferenceManager&, SafeQueue<EPtr>&, SafeQueue<Action>& );

    void setView( ChatWindow* );

    /**
     * Parse and analize input from view and call appropriate methods 
     * (version for non conference chatBox)
     * @param const ChaTIN::Alias& alias - alias from which does input comes
     * @param const Glib::ustring& input - what thas user typed
     * @throw UnknownCommandExcepion when input begins with '/' and isnt parseable
     */
    void doCommand( const ChaTIN::Alias& alias, const Glib::ustring& input ); 

    /**
     * Parse and analize input from view and call appropriate methods 
     * (version for conference chatBox)
     * @param const ChaTIN::ConferenceId& name - identyfier of conference from which does input comes
     * @param const Glib::ustring& input - what thas user typed
     * @throw UnknownCommandExcepion when input begins with '/' and isnt parseable
     */
    void doCommand( const ChaTIN::ConferenceId& name, const Glib::ustring& input );

    /**
     * Parse and analize input from view and call appropriate methods 
     * (version for log chatBox)
     * @param const Glib::ustring& name - name of log tab from which does input comes
     * @param const Glib::ustring& input - what thas user typed
     * @throw UnknownCommandExcepion when input begins with '/' and isnt parseable
     */
    void doCommand( const ChaTIN::LogName& name, const Glib::ustring& input );

    /**
     * Does support all general commands like open/close/addfriend etc.
     * @return true if the input has without context
     */
    bool tryParseGeneral( const Glib::ustring& input );

    /**
     * Just handle all events from bq 
     */
    void operator()();

    static bool isInputCommand( const Glib::ustring& input );
};
