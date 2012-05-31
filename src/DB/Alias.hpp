#ifndef SCHEMA_ALIAS_HPP
#define SCHEMA_ALIAS_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "Group.hpp"

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
        dbo::field(a, ip,"ip");
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

#endif