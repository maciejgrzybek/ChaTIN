#ifndef SCHEMA_GROUP_HPP
#define SCHEMA_GROUP_HPP

#include <Wt/Dbo/Dbo>
#include <string>

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{
class Alias;

class Group
{
public:

  template<class Act>
  void persist(Act& a)
  {
    dbo::field(a, id, "id");
    dbo::field(a, name, "name");
    dbo::hasMany(a, aliases, dbo::ManyToOne, "groupId");
  }
private:
    int id;
    std::string name;
    dbo::collection< dbo::ptr<Alias> > aliases;
};

} // namespace Schema
} // namespace DB

#endif
