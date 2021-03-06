#pragma once
#ifndef _ALIAS_MANAGER_HPP_
#define _ALIAS_MANAGER_HPP_

#include <boost/bimap.hpp>
#include <boost/optional.hpp>
#include <iostream>
#include <map>
#include <set>
#include "DB/DBDriver.hpp"
#include "DialogManager.hpp"
#include "types.hpp"

class DialogManager;


/**
 * Class used to two things:
 * 1. Translate alias->ip ip->alias
 * 2. Colecting informations about subscription
 *    especially state of subscription request and
 *    provide those information for view
 */
class AliasManager
{
    typedef boost::bimap<ChaTIN::Alias, ChaTIN::IPv6> BiStringMap;
    typedef dbo::ptr<DB::Schema::Subscription> SubDB;

    /* aliases bimap (1) */
    BiStringMap dictionary;
    /* subscriptions data (2) */
    std::map<ChaTIN::IPv6, SubDB> subscriptions;
    DB::DBDriver& db;
    bool sendReady;
    boost::optional<DialogManager&> sender;

public:
    /**
     * Default construtctor reading existing aliases from given DBDriver
     */
    AliasManager( DB::DBDriver& );

    /**
     * Allow to set dialog manager after some actions.
     * Before calling this method AliasManager cannot do many things
     * (everything that need to send sth)
     */
    void setDialogManager( DialogManager& sender_ );

    /**
     * Return list of registred aliases.
     * The list does not contains any kind of knowledge about IPs
     * @return std::set<friendRow> list of registred aliases
     */
    std::set<FriendRow> getAliasList();

    /**
     * Gets alias from given ip
     * @param const ChaTIN::IP& IPv6
     * @return alias if any found or argument if doesn't.
     */
    ChaTIN::Alias getAlias( const ChaTIN::IPv6& ) const;

    /**
     * Gets ipv6 adress by alias if it exists in db     
     * If it isnt methos returns argument if its valid ipv6 adress or throw exception otherwise
     * @param const ChaTIN::Alias& Reference to alias
     * @return IPv6 adress as ChaTIN::IPv6
     * @throw AliasDoesNotExistsException - when there is no such alias
     *                                      and argument isnt valid ipv6 adress
     */
    ChaTIN::IPv6 getIP(const ChaTIN::Alias&) const;
    
    /**
     * @param const ChaTIN::Alias& Reference to alias to be registered.
     * @param const ChaTIN::IPv6& Reference to IPv6 to be mapped on given alias.
     * @param bool trySubscribe Parameter decides whether we want send subscription request to given IP on registering. Default value is true.
     * @throw AliasAlreadyExistsException - when alias name is in use
     * @throw IPAlreadyHasAliasException - when ip has alias    
     */
    void registerAlias( const ChaTIN::Alias&, const ChaTIN::IPv6&, bool trySubscribe = true );
    
    /**
     * Removes alias given by ip adress
     * @param const ChaTIN::Alias& IPv6 adress to delete from alias list
     * @throw AliasDoesNotExistsException - when alias wansnt find
     */
    void deleteAliasByIP(const ChaTIN::IPv6&);
    
    /*
     * Removes alias given by its name
     * @param const ChaTIN::Alias& alias to find
     * @throw AliasDoesNotExistsException - when alias wansnt find
     */
    void deleteAliasByAlias(const ChaTIN::Alias&);

    /**
     * Send information to adress that you want to subscribe 
     * If you are in REQUESTED state then just call acceptSub
     * @param const ChaTIN::IPv6& Alias to request
     * @throw AliasNotConnected - if alias client ist connected right now
     * @throw AliasAlreadyFullSubscribed - if alias is on FULL phase with you
     * @throw NoDialogManagerGivenException - if it try to send sth but dialogManager isnt set yet
     */
    void requestSub(const ChaTIN::IPv6&); 

    /**
     * If you are in REQUESTED state then send Accept and go to FULL
     * @param const ChaTIN::IPv6& Alias to accept
     * @throw YouAreNotRequested - when alias client didnt send you request
     * @throw AliasNotConnected - if alias client ist connected right now
     * @throw NoDialogManagerGivenException - if it try to send sth but dialogManager isnt set yet
     */
    void acceptSub(const ChaTIN::IPv6&);
    
    /**
     * If you are in REQUESTED then send Reject and go to REJECTED
     * @param const ChaTIN::IPv6& alias to reject
     * @throw YouAreNotRequested - when alias client didnt send you request
     * @throw AliasNotConnected - if alias client ist connected right now
     * @throw NoDialogManagerGivenException - if it try to send sth but dialogManager isnt set yet
     */
    void rejectSub(const ChaTIN::IPv6&);


    /**
     * Call if request package came from ip
     * if not in REJECTED goto FULL
     */
    void wasRequested(const ChaTIN::IPv6&);

    /**
     * Call if reject package came from alias
     * goes to REJECTED phase
     * @param const ChaTIN::IPv6& alias which rejected you
     */
    void wasRejected(const ChaTIN::IPv6&);

    /**
     * Call if accept package came from alias
     * if you was in REQUESTED go to FULL
     * otherwise dont do anything
     * @param const ChaTIN::IPv6& alias which send accept to you 
     */
    void wasAccepted(const ChaTIN::IPv6&);

    private:
    /**
     * loads subscriptions vector from DB using DBDriver
     */
    void loadSubscriptionsFromDB();

    /**
     * saves subscriptions vector to DB using DBDriver
     */
    void saveSubscriptionsToDB();
};

#endif
