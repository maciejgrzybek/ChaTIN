#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <glibmm/ustring.h>
#include "Exception.hpp"
#include <functional>
#include <string>
namespace ChaTIN
{
class IPv6;
/**
 * Class created to gain types controll from language
 * It is string but it can be used only in some contexts
 * It can be "real" alias or IPv6 if user does not have alias
 *  but even if it has IPv6 it has to be translated via AliasManager
 *  to IPv6 type.
 */
class Alias : public Glib::ustring
{
public:
    /**
     * Copy from IPv6 adress its valid alias anyway
     */
    Alias(const IPv6&);
};

/**
 * Class created to gain types controll from language
 * It is string but it can be used only in some contexts
 */
class IPv6 : public Glib::ustring
{
    /**
     * Just cast to IPv6 if Alias is IP adress
     * @throw WrongAddressException Exception thronw when alias is not valid IPv6 adress.
     */
    IPv6(const Alias&) throw(Socket::WrongAddressException);
};

/**
 * Class created to gain types controll from language
 * It holds name of the conference and ip of its owner
 *  which is enough data to identify conference by ConferenceManager
 */
struct ConferenceId
{
    IPv6          ownerip;
    Glib::ustring name;
    bool operator==(const ConferenceId& r) const
    {
        return (ownerip == r.ownerip) && (name == r.name);
    }
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
struct equal_to<ChaTIN::ConferenceId>
{
    bool operator()(const ChaTIN::ConferenceId& l,const ChaTIN::ConferenceId& r) const
    {
        return (l.ownerip == r.ownerip) && (l.name == r.name);
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
    std::string owneripString = v.ownerip.c_str();
    std::string nameString = v.name.c_str();
    resultHash = hashf(owneripString) ^ hashf(nameString);
    return resultHash;
}

}; /* namespace std */

}


#endif
