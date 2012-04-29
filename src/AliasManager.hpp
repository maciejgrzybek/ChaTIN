#pragma once
#include <glibmm/ustring.h>
#include <boost/bimap.hpp>
#include <iostream>
#include <map>
#include "DBDriver.hpp"

enum SubPhase { REQUESTED, ONE_SIDED, REJECTED, FULL };

/**
 * Class used to two things:
 * 1. Translate alias->ip ip->alias
 * 2. Colecting informations about subscription
 *    especially state of subscription request and
 *    provide those information for view
 */
class AliasManager
{
    typedef boost::bimap<Glib::ustring/*alias*/, Glib::ustring /*ipv6*/> BiStringMap;

    /* aliases bimap (1) */
    BiStringMap dictionary;
    /* subscriptions data (2) */
    std::map< Glib::ustring /*alias*/, SubPhase> subscriptions;
    const DBDriver& db;

public:
    /**
     * Default construtctor reading existing aliases from given DBDriver
     */
    AliasManager( const DBDriver& );

    /**
     * Gets alias from given ip
     * @param const Glib::ustring& IPv6 as string
     * @return alias if any found or argument if doesn't.
     */
    Glib::ustring getAlias( const Glib::ustring& );

    /**
     * Gets ipv6 adress by alias if it exists in db     
     * If it isnt it returns argument if its valid ipv6 adress or throw exception otherwise
     * @param const Glib::ustring& reference to string containing alias
     * @return IPv6 adress as string
     * @throw AliasDoesNotExistsException - when there is no such alias
     *                                      and argument isnt valid ipv6 adress
     */
    Glib::ustring getIP( const Glib::ustring& );
    
    /**
     * @throw AliasAlreadyExistsException - when alias name is in use
     * @throw IPAlreadyHasAliasException - when ip has alias    
     */
    void registerAlias( const Glib::ustring& alias, const Glib::ustring ip, bool trySubscribe = true );
    
    /**
     * Removes alias given by ip adress
     * @param const Glib::ustring& IPv6 adress to delete from alias list
     * @throw AliasDoesNotExistsException - when alias wansnt find
     */
    void deleteAliasByIp( const Glib::ustring& ip );
    
    /**
     * Removes alias given by its name
     * @param const Glib::ustring& alias to find
     * @throw AliasdoesNotExistsException - when alias wansnt find
     */
    void deleteAliasByAlias( const Glib::ustring& alias );

    /**
     * Send information to adress that you want to subscribe him
     *  if you are in REQUESTED then just call acceptSub
     * @param const Glib::ustring& alias to request
     * @throw AliasNotConnected - if alias client ist connected right now
     * @throw AliasAlreadyFullSubscribed - if alias is on FULL phase with you
     */
    void requestSub( const Glib::ustring& alias ); 

    /**
     * If you are in REQUESTED then send Accept and go to FULL
     * @param const Glib::ustring& alias to accept
     * @throw YouAreNotRequested - when alias client didnt send you request
     * @throw AliasNotConnected - if alias client ist connected right now
     */
    void acceptSub( const Glib::ustring& alias );

    /**
     * If you are in REQUESTED then send Reject and go to REJECTED
     * @param const Glib::ustring& alias to reject
     * @throw YouAreNotRequested - when alias client didnt send you request
     * @throw AliasNotConnected - if alias client ist connected right now
     */
    void rejectSub( const Glib::ustring& alias );

    /**
     * Call if reject package came from alias
     * goes to REJECTED phase
     * @param const Glib::ustring& alias which rejected you
     */
    void wasRejected( const Glib::ustring& alias );

    /**
     * Call if accept package came from alias
     * if you was in ONE_SIDED go to FULL
     * otherwise dont do anything
     * @param const Glib::ustring& alias which send accept to you 
     */
    void wasAccepted( const Glib::ustring& alias );
};
