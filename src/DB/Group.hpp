#ifndef SCHEMA_GROUP_HPP
#define SCHEMA_GROUP_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "Alias.hpp"

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{

class Alias;

class Group
{
public:

  std::string getName() const
  {
    return name_;
  }

  dbo::collection<dbo::ptr<Alias> > getAliases() const
  {
    return aliases_;
  }

  void setName(std::string name)
  {
    name_ = name;
  }

  template<class Act>
  void persist(Act& a)
  {
    //dbo::field(a, id, "id");
    dbo::field(a, name_, "name");
    dbo::hasMany(a, aliases_, dbo::ManyToOne, "groupId");
  }
private:
    //int id;
    std::string name_;
    dbo::collection<dbo::ptr<Alias> > aliases_;
};

} // namespace Schema
} // namespace DB

#endif
