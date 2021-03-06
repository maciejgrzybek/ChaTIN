#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <glibmm/ustring.h>
#include "Exception.hpp"
#include <functional>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>
#include <memory>

class ChatWindow;
typedef boost::function<void(ChatWindow*)> Action;

typedef boost::tuple<Glib::ustring, Glib::ustring, bool> ViewMsg;
typedef std::vector<ViewMsg> ViewMsgV;

class ChatTab;
typedef std::shared_ptr<ChatTab> TPtr;

namespace ChaTIN
{
class TabId;
class Alias;
}

typedef std::shared_ptr<ChaTIN::TabId> TIPtr;

enum SubPhase { REQUESTED, ONE_SIDED, REJECTED, FULL, NONE };
enum Status { ONLINE, OFFLINE, DK };
typedef std::pair<ChaTIN::Alias, SubPhase> FriendRow;

namespace ChaTIN
{

enum TabIdType{ DIALOG, CONFERENCE, LOG };
class IPv6;

/*FIXME Implement before any other functionality*/
class TabId 
{
    public:
    virtual TabIdType getType()    const = 0;
    virtual TPtr createTab()  const = 0;
    virtual bool operator==( const TabId& ) const = 0;
};

class LogName : public Glib::ustring, public TabId
{
    public:
    LogName(const Glib::ustring&);
    bool operator==( const LogName& ) const;
    virtual TabIdType getType() const;
    virtual TPtr createTab()  const;
    virtual bool operator==( const TabId& ) const;
};

/**
 * Class created to gain types controll from language
 * It is string but it can be used only in some contexts
 * It can be "real" alias or IPv6 if user does not have alias
 *  but even if it has IPv6 it has to be translated via AliasManager
 *  to IPv6 type.
 */
class Alias : public Glib::ustring, public TabId
{
public:
    /**
     * Copy from IPv6 adress its valid alias anyway
     */
    Alias();
    Alias(const IPv6&);
    Alias(const Glib::ustring&);
    virtual ~Alias();
    bool operator==( const Alias& ) const;
    virtual TabIdType getType() const;
    virtual TPtr createTab() const;
    virtual bool operator==( const TabId& ) const;
};

/**
 * Class created to gain types controll from language
 * It is string but it can be used only in some contexts
 */
class IPv6 : public Glib::ustring
{
public:
    /**
     * Constructs IPv6 from given IPv6 string.
     * @param const std::string& Reference to string containing IP.
     * @throw WrongAddressException Exception thrown when alias is not valid IPv6 adress.
     */
    IPv6(const std::string&) throw(Socket::WrongAddressException);
};

/**
 * Class created to gain types controll from language
 * It holds name of the conference and ip of its owner
 *  which is enough data to identify conference by ConferenceManager
 */
struct ConferenceId : public TabId
{
    IPv6          ownerip;
    Glib::ustring name;
    ConferenceId( const IPv6&, const Glib::ustring& );
    bool operator==(const ConferenceId& r) const;
    virtual TabIdType getType() const;
    virtual TPtr createTab() const;
    virtual bool operator==( const TabId& ) const;
};

/**
 * This struct agregate information about incoming alias with 
 * the massage which came from him.
 */
struct IncomingMassage
{
    IPv6 ip;  
    Glib::ustring msg;
public:
    IncomingMassage( const IPv6& ip, const Glib::ustring& msg );    
};

} /* namespace ChaTIN */

namespace std
{

template <>
struct hash<ChaTIN::IPv6 const>
{
    size_t operator()(const ChaTIN::IPv6& v) const
    {
        hash<std::string> hashf;
        size_t resultHash;
        std::string ipv6String = v.c_str(); //MBO @see hash<ChaTIN::ConferenceId>::operator()
        resultHash = hashf(ipv6String);
        return resultHash;
    }
};

template <>
struct hash<ChaTIN::ConferenceId>
{
/*
 * std::hash specialization for ChaTIN::ConferenceId.
 * Produces hash by xoring hash of each ConferenceId's component.
 * MBO: Method uses strings which are created from ConferenceId's components. It could be more efficient if there is own implementation of hash function, without using hash<std::string> (which needs creatinion of std::string from Glib::ustring).
 * @param const ChaTIN::ConferenceId& Reference to ConferenceId to be hash evaluated of.
 * @return size_t Evaluated hash.
 */
size_t operator()(const ChaTIN::ConferenceId& v) const
{
    hash<std::string> hashf;
    size_t resultHash;
    std::string owneripString = v.ownerip;
    std::string nameString = v.name;
    resultHash = hashf(owneripString) ^ hashf(nameString);
    return resultHash;
}

};

} /* namespace std */



#endif
