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

  dbo::collection< dbo::ptr<Alias> > aliases;

  template<class Act>
  void persist(Act& a)
  {
    dbo::hasMany(a, aliases, dbo::ManyToOne, "groupId");
  }
private:
    int id;
    std::string name;
};

} // namespace Schema
} // namespace DB

#endif
