#ifndef SCHEMA_CONFERENCE_HPP
#define SCHEMA_CONFERENCE_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "../types.hpp"
#include "ConferenceMember.hpp"
#include "Message.hpp"

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{

class ConferenceMember;
class Message;

class Conference
{
public:

    std::string getName() const
    {
        return name;
    }

    dbo::collection< dbo::ptr<Message> > getMessages() const
    {
        return messages;
    }

    template<class Act>
    void persist(Act& a)
    {
        //dbo::field(a, id, "id");
        dbo::field(a, ownerIp, "ownerIp");
        dbo::field(a, name, "name");
        dbo::field(a, date, "date");
        dbo::hasMany(a, messages, dbo::ManyToOne, "conferenceId");
        dbo::hasMany(a, members, dbo::ManyToOne, "conferenceId");
    }
private:
    //int id;
    std::string ownerIp;
    std::string name;
    int date;
    dbo::collection< dbo::ptr<Message> > messages;
    dbo::collection< dbo::ptr<ConferenceMember> > members;

};

} // namespace Schema
} // namespace DB

#endif
