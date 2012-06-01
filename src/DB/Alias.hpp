#ifndef SCHEMA_ALIAS_HPP
#define SCHEMA_ALIAS_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "Group.hpp"

#include "../types.hpp"

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{

class Alias
{
public:
    Alias() : ip(), alias(), group()
    {}

    Alias(ChaTIN::Alias alias_, ChaTIN::IPv6 ipv6) : ip(ipv6), alias(alias_)
    {}

    Alias(ChaTIN::Alias alias_, ChaTIN::IPv6 ipv6, Group group_) : ip(ipv6), alias(alias_), group(&group_)
    {}

    ChaTIN::Alias getAlias() const
    {
        return ChaTIN::Alias((Glib::ustring)alias);
    }
    
    ChaTIN::IPv6 getIP() const
    {
        return ChaTIN::IPv6(ip);
    }

    template<class Act>
    void persist(Act& a)
    {
        dbo::id(a, ip, "ip");
        dbo::field(a, alias, "alias");
        dbo::belongsTo(a, group, "groupId");
    }
private:
    std::string ip;
    std::string alias;
    dbo::ptr<Group> group;

};

} // namespace Schema
} // namespace DB

namespace Wt
{
    namespace Dbo
    {

        template<>
        struct dbo_traits<DB::Schema::Alias> : public dbo_default_traits
        {
            typedef std::string IdType;

            static IdType invalidId() {
                return std::string();
            }

            static const char *surrogateIdField() { return 0; }
        };

    }
}

#endif
