#include "ToViewParser.hpp"
#include "ConferenceManager.hpp"
#include <boost/thread.hpp>
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include <tinyxml.h>

ToViewParser::ToViewParser( AliasManager& am, ConferenceManager& cm, SafeQueue<ChaTIN::IncomingMassage>& q, SafeQueue<Action>& aq )
    : am(am), cm(cm), q(q), aq(aq)
{
    actions[ "msg"  ] = &ToViewParser::incomingDialogMsg;
    actions[ "cmsg" ] = &ToViewParser::incomingConfMsg;
    actions[ "iky"  ] = &ToViewParser::incomingSubRequest;
    actions[ "ikya" ] = &ToViewParser::incomingAcceptSub;
    actions[ "idky" ] = &ToViewParser::incomingRejectSub;
}

void ToViewParser::doCommand( const ChaTIN::IPv6& ip, const Glib::ustring& msg )
{
    q.push(ChaTIN::IncomingMassage(ip,msg)); // FIXME: can be vulnerable of out of memory when SafeQueue is full and we try to push to it.
    // In this case, thread which called doCommand will hang up.
}

void ToViewParser::operator()()
{
    for(;;)
    {
        ChaTIN::IncomingMassage currentMassage = q.front();
        q.pop();
        parse( currentMassage );
    }
}

void ToViewParser::incomingDialogMsg( ChaTIN::IPv6 ip, Glib::ustring msg, otherAttributes)
{
    TIPtr incomeAlias(new ChaTIN::Alias(ip)); //FIXME aliasManager usage here
    Glib::ustring alias = (ChaTIN::Alias&)*incomeAlias;
    aq.push( boost::bind(&ChatWindow::showIncomingMessage, _1, incomeAlias,
                                     alias, msg, true ));
}

void ToViewParser::incomingConfMsg( ChaTIN::IPv6 ip, Glib::ustring msg, otherAttributes attr)
{
    TIPtr incomeId(new ChaTIN::ConferenceId(
                                        ChaTIN::IPv6(attr.lower_bound("ownerip")->second), 
                                        attr.lower_bound("name")->second )); //FIXME dialogManager
    std::vector< ChaTIN::IPv6 > members;
    otherAttributes::iterator i;
    for( i = attr.lower_bound("memberip"); i!=attr.upper_bound("memberip"); ++i )
    {
        members.push_back(ChaTIN::IPv6(i->second));
    }
    cm.registerIncomingConference( (ChaTIN::ConferenceId&)(*incomeId), members );
    Glib::ustring alias = ChaTIN::Alias(ip);
    aq.push( boost::bind(&ChatWindow::showIncomingMessage, _1, incomeId, 
                                    alias, msg, true ));
}

void ToViewParser::incomingSubRequest( ChaTIN::IPv6 ip, Glib::ustring, otherAttributes )
{
    am.wasRequested( ip );
}

void ToViewParser::incomingAcceptSub( ChaTIN::IPv6 ip, Glib::ustring, otherAttributes )
{
    am.wasAccepted( ip );
}

void ToViewParser::incomingRejectSub( ChaTIN::IPv6 ip, Glib::ustring, otherAttributes )
{
    am.wasRejected( ip );
}

void ToViewParser::parse( const ChaTIN::IncomingMassage& income )
{
    if(income.msg == "")
        return;

    Glib::ustring msgText;
    Glib::ustring typeName;
    otherAttributes otherAtt;    

    TiXmlDocument massage;
    massage.Parse( income.msg.c_str() );
    TiXmlElement* msgNode = massage.FirstChildElement("massage");
    if( msgNode )
    {
        TiXmlElement* type = msgNode->FirstChildElement("type");
        TiXmlElement* text = msgNode->FirstChildElement("text");
        if( type && text )
        {
            auto iter = actions.find( type->GetText() );
            msgText = text->GetText();
            typeName = type->GetText();

            if( iter != actions.end() )
            {
                msgNode->RemoveChild(type); //we remove those in order to not 
                msgNode->RemoveChild(text); //find them again in loop

                //now find and save every other children of this massage
                TiXmlElement* current = msgNode->FirstChildElement();
                while( current!=NULL )
                {
                    otherAtt.insert(std::make_pair(current->Value(),current->GetText()));
                    if( msgNode->IterateChildren(current) == NULL )
                        break; //ugly but simple TODO fix
                    current = msgNode->IterateChildren(current)->ToElement(); //Wow! Just like in NWScript                    
                }
                iter->second(this, income.ip, msgText, otherAtt);
            }
            else
            {
                //FIXME
                //THROW CannotParseMassageException
            }
        }            
    }
    else
    {
        //FIXME
        //THROW CannotParseMassageException
    }
}

