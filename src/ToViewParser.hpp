#pragma once
#include <glibmm/ustring.h>
#include "types.hpp"
#include "SafeQueue.hpp"

/**
 * Class which should be used to add elements to waiting queue
 */
class QueueAdder
{
    ChaTIN::IncomingMassage income;
    SafeQueue<ChaTIN::IncomingMassage>& q;
public:
    /**
     * Create functor with given argumeters
     * @param const ChaTIN::IncomingMassage& massage that came  from DialogManager
     * @param SafeQueue<ChaTIN::IncomingMassage>& q queue to which we will add our income
     */
    QueueAdder( const ChaTIN::IncomingMassage& income, SafeQueue<ChaTIN::IncomingMassage>& q );
    
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
    SafeQueue<ChaTIN::IncomingMassage>& q; //queue for all stuff
public:
    /**
     * Constructor taking queue reference to comunicate with rest of the world 
     * @param SafeQueue<ChaTIN::IncomingMassage>& q queue to communicate with world
     */
    ToViewParser( SafeQueue<ChaTIN::IncomingMassage>& q );

    /**
     * Put a masssage with Incoming alias to the queue
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
