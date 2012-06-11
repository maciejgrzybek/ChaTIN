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

    Conference() : ownerIp_(), name_(), date_(0)
    {}

    Conference(std::string ownerIp, std::string name, int date = 0)
    {
        if(date == 0)
            date = time(NULL);

        ownerIp_ = ownerIp;
        name_ = name;
        date_ = date;
    }

    Conference(const Conference& toCopy) : ownerIp_(toCopy.ownerIp_),
                                          name_(toCopy.name_),
                                          date_(toCopy.date_)
    {}

    std::string getOwnerIp() const
    {
        return ownerIp_;
    }

    std::string getName() const
    {
        return name_;
    }

    int getDate() const
    {
        return date_;
    }

    dbo::collection< dbo::ptr<Message> > getMessages() const
    {
        return messages_;
    }

    dbo::collection< dbo::ptr<ConferenceMember> > getMembers() const
    {
        return members_;
    }

    void setOwnerIp(std::string ownerIp)
    {
        ownerIp_ = ownerIp;
    }

    void setName(std::string name)
    {
        name_ = name;
    }

    void setDate(int date)
    {
        date_ = date;
    }

    void addConferenceMember(ConferenceMember& conferenceMember)
    {
        conferenceMember.setConference(this);
//        members_.insert(conferenceMember);
    }

    void addMessage(dbo::ptr<Message> message)
    {
        messages_.insert(message);
    }

    template<class Act>
    void persist(Act& a)
    {
        //dbo::field(a, id, "id");
        dbo::field(a, ownerIp_, "ownerIp");
        dbo::field(a, name_, "name");
        dbo::field(a, date_, "date");
        dbo::hasMany(a, messages_, dbo::ManyToOne, "conference");
        dbo::hasMany(a, members_, dbo::ManyToOne, "conference");
    }
private:
    //int id;
    std::string ownerIp_;
    std::string name_;
    int date_;
    dbo::collection< dbo::ptr<Message> > messages_;
    dbo::collection< dbo::ptr<ConferenceMember> > members_;

};

} // namespace Schema
} // namespace DB

#endif
