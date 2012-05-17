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

void FromViewParser::doCommand( const Glib::ustring& name, const Glib::ustring& input )
{
    if( isInputCommand( input ) )
    {
        if( input.substr(1,5) == "open " )
        {
            TPtr tab = TPtr( new  ChatTabDialog( ChaTIN::Alias( input.substr(6) ) ) );
            //FIXME ALWAYS OPENING NEW ONE 
            aq.push( boost::bind(&ChatWindow::openDialogTab, _1, tab) );    
        }
        //ANALYZE COMMAND
    }
    else
    {
        //FIXME
        XMLPackageCreator xml("cmsg", input);
        std::cout << xml.getXML();
    }
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
