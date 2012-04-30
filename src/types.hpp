#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <glibmm/ustring.h>
#include "Exception.hpp"
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
};

}/*namespace ChaTIN*/

#endif
