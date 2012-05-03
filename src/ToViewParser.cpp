#include "ToViewParser.hpp"

QueueAdder::QueueAdder( const ChaTIN::incomingMassage& income, SafeQueue<ChaTIN::incomingMassage>& )
    : q(q), income(income)
{}

void QueueAdder::operator()()
{
    q.push(income); //this may hang process for a while
}

void ToViewParser::doCommand( const ChaTIN::Alias& alias, const Glib::ustring& msg )
{
    
}

void ToViewParser::operator()()
{
    
}
