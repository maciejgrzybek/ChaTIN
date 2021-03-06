#include "FromViewParser.hpp"
#include "ConferenceManager.hpp"
#include "XMLPackageCreator.hpp"
#include "SafeQueue.hpp"
#include "ChatTab.hpp"
#include "Exception.hpp"
#include "types.hpp"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

FromViewParser::FromViewParser( DialogManager& dialogManager, 
                                AliasManager& aliasManager, 
                                ConferenceManager& cm, 
                                SafeQueue<EPtr>& bq, 
                                SafeQueue<Action>& aq )
    : dialogManager(dialogManager), 
      aliasManager(aliasManager), 
      cm(cm), 
      bq(bq), 
      aq(aq)
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
        if( input.substr(1,4) == "arch" )
        {
            ViewMsgV messages;
            DB::Messages m = DB::DBDriver::getInstance()->getMessages( aliasManager.getIP(alias) );
            for( auto& i : m )
            {
                messages.push_back( 
                    ViewMsg( 
                        aliasManager.getAlias(ChaTIN::IPv6(i->getIp())),
                        i->getContent(),
                        !i->getOutgoing()
                        ) 
                );
            }
            aq.push( 
                boost::bind(
                     &ChatWindow::loadHistory, 
                     _1, 
                     TIPtr( new ChaTIN::Alias(alias) ),
                     messages )
            ); 
        }
    }
    else
    {
        XMLPackageCreator xml("msg", input);
        dialogManager.sendTo(alias, xml.getXML());        
        TIPtr idWrite( new ChaTIN::Alias(alias) );
        aq.push( boost::bind(&ChatWindow::showIncomingMessage, _1, idWrite, "", input, false ));
        //From view parser

        //Save message in archive
        DB::DBDriver::getInstance()->store
                 (
                    new DB::Schema::Message(
                        std::string(aliasManager.getIP(alias)), 
                        std::string(input),
                        true )
                 );
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
        unsigned int limit = 256;
        Glib::ustring text;
        if( input.size() > limit )
            text = input.substr(0,limit);
        else
            text = input;

        XMLPackageCreator xml("cmsg", text);
        xml["name"] = name.name;
        xml["ownerip"] = name.ownerip;
        auto members = cm.getList(name);
        for(auto& member : members)
            xml["memberip"] = (Glib::ustring)member->getIp();
        std::cout << xml.getXML() << std::endl;
 
       dialogManager.sendTo(name, xml.getXML());
       //Save message in archive
       DB::Schema::Message* arch = 
                new DB::Schema::Message
                 (
                    std::string("::1"), 
                    std::string(input),
                    true 
                 );        
        arch->setConference( 
                new DB::Schema::Conference(
                        name.ownerip,
                        name.name
                    )            
        );
        DB::DBDriver::getInstance()->store(arch);
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
        TIPtr idOpen;
        //Choose if he put IP or ALIAS
        try
        {
            idOpen = TIPtr( new ChaTIN::Alias( aliasManager.getAlias( ChaTIN::IPv6(toOpen) ) ) );
        }
        catch( Socket::WrongAddressException& e )
        {
            idOpen = TIPtr( new ChaTIN::Alias( toOpen ) );
        }
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
            //FIXME throw invalidCommandParamsException
            return true;
        }
        TIPtr idOpen( new ChaTIN::ConferenceId( ChaTIN::IPv6(params[1]), params[0] )); //FIXME put valid myIp
        for(unsigned int i = 1; i < params.size(); ++i)
        {
            members.push_back(aliasManager.getIP(ChaTIN::Alias(params[i])));
        }
        cm.addConference( (ChaTIN::ConferenceId&)*idOpen, members );
        aq.push( boost::bind( &ChatWindow::openTab, _1, idOpen, true ) );
        return true;
    }

    if( input.substr(1,9) == "addalias ")
    {
        std::vector<Glib::ustring> params = Glib::Regex::split_simple(" ", input.substr(10));
        if( params.size() != 2 )
        {
            //FIXME throw invalicCommandParamsException
            return true;
        }
        ChaTIN::Alias alias(params[0]);
        ChaTIN::IPv6 ip(params[1]);
        aliasManager.registerAlias( alias, ip, false );
        aq.push( boost::bind( &ChatWindow::initializeFriends, _1, aliasManager.getAliasList() ) );
        return true;
    }

    if( input.substr(1,4) == "sub " )
    {        
        ChaTIN::IPv6 ip( aliasManager.getIP( ChaTIN::Alias( input.substr(5) ) ) );
        aliasManager.requestSub( ip );
        aq.push( boost::bind( &ChatWindow::initializeFriends, _1, aliasManager.getAliasList() ) );
        return true;
    }

    if( input.substr(1,7) == "subakc " )
    {
        ChaTIN::IPv6 ip( aliasManager.getIP( ChaTIN::Alias( input.substr(8) ) ) );
        aliasManager.acceptSub( ip );
        aq.push( boost::bind( &ChatWindow::initializeFriends, _1, aliasManager.getAliasList() ) );
        return true;
    }

    if( input.substr(1,7) == "subdec " )
    {
        ChaTIN::IPv6 ip( aliasManager.getIP( ChaTIN::Alias( input.substr(8) ) ) );
        aliasManager.rejectSub( ip );
        aq.push( boost::bind( &ChatWindow::initializeFriends, _1, aliasManager.getAliasList() ) );
        return true;
    }

    if( input.substr(1,4) == "load" )
    {
        aq.push( boost::bind( &ChatWindow::initializeFriends, _1, aliasManager.getAliasList() ) );
        return true;
    }
    
    if( input.substr(1,4) == "help" )
    {
        Glib::ustring helpmsg = "";
        helpmsg = "/open Alias/IP - otwiera okno rozmowy z pojedyńczym użytkownikiem";
        TIPtr logId = TIPtr( new ChaTIN::LogName("LOG") );
        aq.push( boost::bind(&ChatWindow::showIncomingMessage, _1, logId, "", helpmsg, true ));
        for(int i = 0; i<20; ++i )
        {
        helpmsg = "/confopen OwnerIP (IP)* - otwiera okno konferencji z zadanymi IP";
        TIPtr logId = TIPtr( new ChaTIN::LogName("LOG") );
        aq.push( boost::bind(&ChatWindow::showIncomingMessage, _1, logId, "", helpmsg, true ));
        }
        return true;
    }
    
    return false;
}

void FromViewParser::operator()()
{
    for(;;)
    {
        try
        {
            bq.front()->doCommand(*this);
        }
        catch( std::exception& e )
        {     
            TIPtr logId = TIPtr( new ChaTIN::LogName("LOG") );
            aq.push( boost::bind(&ChatWindow::showIncomingMessage, _1, logId, "ERROR: ", e.what(), true ));
        }
        bq.pop();
    }
}
  
bool FromViewParser::isInputCommand( const Glib::ustring& input )
{         
     return input.size() > 0 && input[0]=='/';
}   
