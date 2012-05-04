#pragma once
#include <glibmm/ustring.h>
#include "types.hpp"

/**
 * Parser class converting user input into method calls
 */
class FromViewParser
{
    public:
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
     * (version for non conference chatBox)
     * @param const ChaTIN::ConferenceId& name - identyfier of conference from which does input comes
     * @param const Glib::ustring& input - what thas user typed
     * @throw UnknownCommandExcepion when input begins with '/' and isnt parseable
     */
    void doCommand( const ChaTIN::ConferenceId& name, const Glib::ustring& input );

    static bool isInputCommand( const Glib::ustring& input );
};
