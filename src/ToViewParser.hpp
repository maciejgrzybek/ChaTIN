#pragma once
#include <glibmm/ustring.h>
#include <map>
#include <boost/function.hpp>
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
    
    typedef boost::function<void ( const ChaTIN::Alias&, const Glib::ustring& msgText ) > inActionFun;

    
    // map msg_type -> callback (boost::function)
    std::map< Glib::ustring, inActionFun > actions;

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

private:
    /**
     * It do all parsing using tinyxml and then call function based on massage type
     * @throw CannotParseMassageExcpeion if parser cannot understand income
     */
    void parse( const ChaTIN::IncomingMassage& msg );
};
