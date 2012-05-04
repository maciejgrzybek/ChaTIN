#pragma once
#include <glibmm/ustring.h>
#include <map>
#include <boost/function.hpp>
#include "types.hpp"
#include "SafeQueue.hpp"
#include "QueueAdder.hpp"

/**
 * Parser of packages coming from inet
 */
class ToViewParser
{
    SafeQueue<ChaTIN::IncomingMassage>& q; //queue for all stuff
   
    
    /**
     * This is the map name->value for other information
     *  which can be find in xml and can be usefull in some cotext (eg. conference name for conference income)
     */
    typedef std::map<Glib::ustring, Glib::ustring> otherAttributes;
//    typedef std::string otherAttributes;
 
    /**
     * This is boost::function refering to ToViewParser method.
     * Correct method (action) should:
     *      - return void
     *      - gets const ChaTIN::Alias& as first  argument (from who is the massage comming)
     *      - gets const Glib::ustring& as second argument (massage text)
     *      - gets const otherAtributes as third  argument (additional information map: name->str_value)
     */
    typedef boost::function<void ( ToViewParser*, const ChaTIN::Alias&, const Glib::ustring&, const otherAttributes& ) > inActionFun;

    
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
