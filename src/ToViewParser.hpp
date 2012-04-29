#pragma once
#include <glibmm/ustring.h>

/**
 * Parser of packages coming from inet
 */
class ToViewParser
{
    /**
     * Parse massage incoming from alias given by fisrt argument
     * it invokes any method needed after analizying msg and context
     * @param cosnt Glib::ustring& alias of person who send you that massage
     * @param const Glib::ustring& msg directly what came from Socket::Socket 
     * @throw CannotParseMassageException if parser cannot understand that
     */
    void doCommand( const Glib::ustring& alias, const Glib::ustring& msg );
};
