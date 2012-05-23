#include "FromViewParser.hpp"
#include "ConferenceManager.hpp"
#include "XMLPackageCreator.hpp"
#include "SafeQueue.hpp"
#include "ChatTab.hpp"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

FromViewParser::FromViewParser( DialogManager& dialogManager, ConferenceManager& cm, SafeQueue<EPtr>& bq, SafeQueue<Action>& aq )
    : dialogManager(dialogManager), cm(cm), bq(bq), aq(aq)
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
        TIPtr idWrite( new ChaTIN::Alias(alias) );
        aq.push( boost::bind(&ChatWindow::showIncomingMessage, _1, idWrite, "", input, false ));
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
        const std::vector< ChaTIN::IPv6 >& members = cm.getList( name );
        for( auto& member : members )
            xml["memberip"] = (Glib::ustring)member;
        std::cout << xml.getXML() << std::endl;
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

bool FromViewParser::tryParseGeneral( const Glib::ustring& input )
{
    if( !isInputCommand( input ) )
        return false;

    if( input.substr(1,5) == "open " )
    {
        Glib::ustring toOpen = input.substr(6);
        //FIXME Check if alias is valid
        //FIXME ALWAYS OPENING NEW ONE 
        TIPtr idOpen = TIPtr( new ChaTIN::Alias( toOpen ) );
        aq.push( boost::bind(&ChatWindow::openTab, _1, idOpen, true ) );    
        return true;
    }

    if( input.substr(1,5) == "close" )
    {
        aq.push( boost::bind(&ChatWindow::closeCurrentTab, _1) );
        return true;
    }
    //ANALYZE COMMAN
    
    if( input.substr(1,9) == "confopen " )
    {
        std::vector<Glib::ustring> params = Glib::Regex::split_simple(" ", input.substr(10));
        std::vector<ChaTIN::IPv6> members;
        if(params.size()<3)
        {
            //FIXME throw invalidCommandException
            return true;
        }
        TIPtr idOpen( new ChaTIN::ConferenceId( ChaTIN::IPv6(params[1]), params[0] )); //FIXME put valid myIp
        for( int i = 1; i < params.size(); ++i )
        {
            members.push_back(ChaTIN::IPv6(params[i]));
        }
        cm.addConference( (ChaTIN::ConferenceId&)*idOpen, members );
        aq.push( boost::bind( &ChatWindow::openTab, _1, idOpen, true ) );
    }

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
