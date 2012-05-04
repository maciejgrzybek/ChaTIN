#include "FromViewParser.hpp"

void FromViewParser::doCommand( const ChaTIN::Alias& alias, const Glib::ustring& input )
{
    if( isInputCommand( input ) )
    {
        //ANALYZE COMMAND
    }
    else
    {
        //Its simple massage package
    }
}

void FromViewParser::doCommand( const ChaTIN::ConferenceId& name, const Glib::ustring& input )
{
    if( isInputCommand( input ) )
    {
        //ANALYZE COMMAND
    }
    else
    {
        //Its simple massage package
    }
}

bool FromViewParser::isInputCommand( const Glib::ustring& input )
{
    return input[0]=='/';
}   
