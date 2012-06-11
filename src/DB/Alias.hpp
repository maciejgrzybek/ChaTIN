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

class Group;

class Alias
{
public:
    Alias() : ip_(), alias_(), group_()
    {}

    Alias(ChaTIN::Alias alias, ChaTIN::IPv6 ipv6) : ip_(ipv6), alias_(alias)
    {}

    Alias(ChaTIN::Alias alias, ChaTIN::IPv6 ipv6, Group* group) : ip_(ipv6), alias_(alias), group_(group)
    {}

    ChaTIN::Alias getAlias() const
    {
        return ChaTIN::Alias((Glib::ustring)alias_);
    }
    
    ChaTIN::IPv6 getIP() const
    {
        return ChaTIN::IPv6(ip_);
    }

    template<class Act>
    void persist(Act& a)
    {
        dbo::id(a, ip_, "ip");
        dbo::field(a, alias_, "alias");
        dbo::belongsTo(a, group_, "groupId");
    }
private:
    std::string ip_;
    std::string alias_;
    dbo::ptr<Group> group_;

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

            static IdType invalidId()
            {
                return std::string();
            }

            static const char* surrogateIdField() { return 0; }
        };

    }
}

#endif
