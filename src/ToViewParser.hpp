#pragma once
#include <glibmm/ustring.h>
#include <map>
#include <boost/function.hpp>
#include "types.hpp"
#include "ChatWindow.hpp"
#include "SafeQueue.hpp"
#include "QueueAdder.hpp"
#include "XMLPackageCreator.hpp"
#include "AliasManager.hpp"

class DialogManager;
class ConferenceManager;

/**
 * Parser of packages coming from inet
 */
class ToViewParser
{
    AliasManager& am;
    ConferenceManager& cm;
    SafeQueue<ChaTIN::IncomingMassage>& q; //queue for all stuff
    SafeQueue<Action>& aq;
    

public:
    //FIXME i should use full version everywhere, but its only for now
    typedef XMLPackageCreator::OtherAttributes otherAttributes;
 
    /**
     * This is boost::function refering to ToViewParser method.
     * Correct method (action) should:
     *      - return void
     *      - gets const ChaTIN::Alias& as first  argument (from who is the massage comming)
     *      - gets const Glib::ustring& as second argument (massage text)
     *      - gets const otherAtributes as third  argument (additional information map: name->str_value)
     */
    typedef boost::function<void ( ToViewParser*, ChaTIN::IPv6, Glib::ustring, otherAttributes ) > InActionFun;

private:    
    // map msg_type -> callback (boost::function)
    std::map< Glib::ustring, InActionFun > actions;

public:
    /**
     * Constructor taking queue reference to comunicate with rest of the world 
     * @param SafeQueue<ChaTIN::IncomingMassage>& q queue to communicate with world
     */
    ToViewParser( AliasManager&, ConferenceManager&, SafeQueue<ChaTIN::IncomingMassage>& q, SafeQueue<Action>& aq );

    /**
     * Put a masssage with Incoming alias to the queue
     * @param cosnt Glib::ustring& alias of person who send you that massage
     * @param const Glib::ustring& msg directly what came from Socket::Socket 
     * @throw CannotParseMassageException if parser cannot understand that
     */
    void doCommand( const ChaTIN::IPv6& ip, const Glib::ustring& msg );
    
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


    /**COMMNADS**/
    void incomingDialogMsg( ChaTIN::IPv6, Glib::ustring, otherAttributes );
    void incomingConfMsg( ChaTIN::IPv6, Glib::ustring, otherAttributes );
    void incomingSubRequest( ChaTIN::IPv6, Glib::ustring, otherAttributes );
    void incomingAcceptSub( ChaTIN::IPv6, Glib::ustring, otherAttributes );
    void incomingRejectSub( ChaTIN::IPv6, Glib::ustring, otherAttributes );
};
