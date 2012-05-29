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
    template<class Act>
    void persist(Act& a)
    {
        dbo::belongsTo(a, group, "groupId");
    }
private:
    ChaTIN::IPv6 ip;
    ChaTIN::Alias alias;
    dbo::ptr<Group> group;

};

} // namespace Schema
} // namespace DB

#endif
