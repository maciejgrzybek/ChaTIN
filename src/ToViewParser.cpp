#include "ToViewParser.hpp"
#include <boost/thread.hpp>
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include <tinyxml.h>

ToViewParser::ToViewParser( SafeQueue<ChaTIN::IncomingMassage>& q, SafeQueue<Action>& aq )
    : q(q), aq(aq)
{
   actions["msg"]=&ToViewParser::incomingDialogMsg;
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

void ToViewParser::incomingDialogMsg( const ChaTIN::IPv6& ip, const Glib::ustring& msg, const otherAttributes&)
{
    TIPtr incomeAlias(new ChaTIN::Alias(ip));
    aq.push( boost::bind(&ChatWindow::showIncomingMessage, _1, incomeAlias, msg, false ));
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
                while( current )
                {
                    otherAtt[current->Value()] = current->GetText();
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

