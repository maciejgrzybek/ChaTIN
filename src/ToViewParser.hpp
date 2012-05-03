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
    QueueAdder( const ChaTIN::incomingMassage& income, SafeQueue<ChaTIN::incomingMassage>& q );
    void operator()();
};

/**
 * Parser of packages coming from inet
 */
class ToViewParser
{
public:
    /**
     * Put a message with incoming alias to the queue
     * Parse massage incoming from alias given by first argument
     * it invokes any method needed after analizying msg and context
     * @param const ChaTIN::Alias& alias of person who send you that massage
     * @param const Glib::ustring& message to be parsed
     * @throw CannotParseMassageException if parser cannot understand that
     */
    void doCommand( const ChaTIN::Alias&, const Glib::ustring&) const;
    
    /**
     * Main loop of ToViewParser, build to run as a thread
     * Takes incoming massages from queque parse, and calls all method needed by context
     * and massage
     */
    void operator()();
};
