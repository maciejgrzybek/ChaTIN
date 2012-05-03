#pragma once
#include <glibmm/ustring.h>
#include "types.hpp"
#include "SafeQueue.hpp"

/**
 * Class which should be used to add elements to waiting queue
 */
class QueueAdder
{
    ChaTIN::incomingMassage income;
    SafeQueue<ChaTIN::incomingMassage>& q;
public:
    /**
     * Create functor with given argumeters
     * @param const ChaTIN::incomingMassage& massage that came  from DialogManager
     * @param SafeQueue<ChaTIN::incomingMassage>& q queue to which we will add our income
     */
    QueueAdder( const ChaTIN::incomingMassage& income, SafeQueue<ChaTIN::incomingMassage>& q );
    
    /** 
     * Ads income to SafeQueue (designed to run in new thread).
     */
    void operator()();
};

/**
 * Parser of packages coming from inet
 */
class ToViewParser
{
public:
    /**
     * Put a masssage with incoming alias to the queue
     * @param cosnt Glib::ustring& alias of person who send you that massage
     * @param const Glib::ustring& msg directly what came from Socket::Socket 
     * @throw CannotParseMassageException if parser cannot understand that
     */
    void doCommand( const ChaTIN::Alias& alias, const Glib::ustring& msg );
    
    /**
     * Main loop of ToViewParser, build to run as a thread
     * Takes incoming massages from queque parse, and calls all method needed by context
     * and massage
     */
    void operator()();
};
