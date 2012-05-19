#include "FromViewParser.hpp"
#include "XMLPackageCreator.hpp"
#include "SafeQueue.hpp"
#include "ChatTab.hpp"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

FromViewParser::FromViewParser( DialogManager& dialogManager, SafeQueue<EPtr>& bq, SafeQueue<Action>& aq )
    : dialogManager(dialogManager), bq(bq), aq(aq)
{}

void FromViewParser::setView( ChatWindow* cw )
{
    this->cw = cw;
}


void FromViewParser::doCommand( const ChaTIN::Alias& alias, const Glib::ustring& input )
{
    if( isInputCommand( input ) )
    {
        if(tryParseGeneral(input))
            return;
        //ANALYZE COMMAND
    }
    else
    {
        XMLPackageCreator xml("msg", input);
        dialogManager.sendTo(alias, xml.getXML());
        aq.push( boost::bind(&ChatWindow::showIncomingMessageA, _1, alias, input, false ));

    }
}

void FromViewParser::doCommand( const ChaTIN::ConferenceId& name, const Glib::ustring& input )
{
    if( isInputCommand( input ) )
    {
        if(tryParseGeneral(input))
            return;
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

void FromViewParser::doCommand( const ChaTIN::LogName& name, const Glib::ustring& input )
{
    if( isInputCommand( input ) )
    {
        if(tryParseGeneral(input))
            return;
        //DO COMMNAD PARSING FOR Log tabs
    }
    else
    {
        //FIXME
        XMLPackageCreator xml("cmsg", input);
        std::cout << xml.getXML();
    }
} 

bool tryParseGeneral( const Glib::ustring& input )
{
    if( !isInputCommand( input ) )
        return false;

    if( input.substr(1,5) == "open " )
    {
        Glib::ustring toOpen = input.substr(6);
        //FIXME Check if alias is valid
        TPtr tab = TPtr( new  ChatTabDialog( ChaTIN::Alias( toOpen ) ) );
        //FIXME ALWAYS OPENING NEW ONE 
        aq.push( boost::bind(&ChatWindow::openDialogTab, _1, tab, true ) );    
        return true;
    }
    //ANALYZE COMMAND

    return false;
}

void FromViewParser::operator()()
{
    for(;;)
    {
        bq.front()->doCommand(*this);
        bq.pop();
    }
}
  
bool FromViewParser::isInputCommand( const Glib::ustring& input )
{ 
     return input[0]=='/';
}   
