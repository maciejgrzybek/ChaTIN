#include <glibmm/ustring.h>
namespace ChaTIN
{
/**
 * Class created to gain types controll from language
 * It is string but it can be used only in some contexts
 */
struct IPv6 : public Glib::ustring
{};

/**
 * Class created to gain types controll from language
 * It is string but it can be used only in some contexts
 * It can be "real" alias or IPv6 if user does not have alias
 *  but even if it has IPv6 it has to be translated via AliasManager
 *  to IPv6 type.
 */
struct Alias : public Glib::ustring
{};

/**
 * Class created to gain types controll from language
 * It holds name of the conference and ip of its owner
 *  which is enough data to identify conference by ConferenceManager
 */
struct ConferenceId : public Glib::ustring
{
    IPv6          ownerip;
    Glib::ustring name;
};

}
