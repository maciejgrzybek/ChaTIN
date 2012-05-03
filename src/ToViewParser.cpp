#include "ToViewParser.hpp"
#include <boost/thread.hpp>

QueueAdder::QueueAdder( const ChaTIN::incomingMassage& income, SafeQueue<ChaTIN::incomingMassage>& q )
    : q(q), income(income)
{}

void QueueAdder::operator()()
{
    q.push(income); //this may hang process for a while
}

ToViewParser::ToViewParser( SafeQueue<ChaTIN::incomingMassage>& q )
    : q(q)
{}

void ToViewParser::doCommand( const ChaTIN::Alias& alias, const Glib::ustring& msg )
{
    //just invoke QueueAdder thread
    boost::thread adderThread(QueueAdder(ChaTIN::incomingMassage(alias, msg),q));
}

void ToViewParser::operator()()
{
    ChaTIN::incomingMassage currentMassage;
    for(;;)
    {
        currentMassage = q.front();
        q.pop();
        //FIXME do the stuff with currentMassage here
    }
}
