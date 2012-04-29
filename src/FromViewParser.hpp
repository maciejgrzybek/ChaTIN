#pragma once
#include <glibmm/ustring.h>

/**
 * Parser class converting user input into method calls
 */
class FromViewParser
{
    /**
     * Parse and analize input from view and call appropriate methods
     * @param const Glib::ustring& alias - alias from which does input comes
     * @param const Glib::ustring& input - what thas user typed
     * @throw UnknownCommandExcepion when input begins with '/' and isnt parseable
     */
    void doCommand( const Glib::ustring& alias, const Glib::ustring& input );
};
