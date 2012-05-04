#include "FromViewParser.hpp"
#include "XMLPackageCreator.hpp"

FromViewParser::FromViewParser( DialogManager& dialogManager )
    : dialogManager(dialogManager)
{}

void FromViewParser::doCommand( const ChaTIN::Alias& alias, const Glib::ustring& input )
{
    if( isInputCommand( input ) )
    {
        //ANALYZE COMMAND
    }
    else
    {
        XMLPackageCreator xml("msg", input);
        dialogManager.sendTo(alias, xml.getXML());
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
        XMLPackageCreator xml("cmsg", input);
        xml["name"] = name.name;
        xml["ownerip"] = name.ownerip;
        dialogManager.sendTo(name, xml.getXML());
    }
}

bool FromViewParser::isInputCommand( const Glib::ustring& input )
{
    return input[0]=='/';
}   
