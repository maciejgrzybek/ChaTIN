#include "ToViewParser.hpp"
#include <boost/thread.hpp>
#define TIXML_USE_STL
#include <tinyxml.h>


QueueAdder::QueueAdder( const ChaTIN::IncomingMassage& income, SafeQueue<ChaTIN::IncomingMassage>& q )
    : q(q), income(income)
{}

void QueueAdder::operator()()
{
    q.push(income); //this may hang process for a while
}

ToViewParser::ToViewParser( SafeQueue<ChaTIN::IncomingMassage>& q )
    : q(q)
{}

void ToViewParser::doCommand( const ChaTIN::Alias& alias, const Glib::ustring& msg )
{
    //just invoke QueueAdder thread
    boost::thread adderThread(QueueAdder(ChaTIN::IncomingMassage(alias, msg),q));
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


void ToViewParser::parse( const ChaTIN::IncomingMassage& income )
{
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
            if( iter != actions.end() )
            {
                iter->second(income.alias, text->GetText() );
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
     
    //DO PARSING (no gramar yet)
}

