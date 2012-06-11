#ifndef SCHEMA_CONFERENCE_MEMBER_HPP
#define SCHEMA_CONFERENCE_MEMBER_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "Conference.hpp"

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{

class Conference;

class ConferenceMember
{
public:

    ConferenceMember() : ip_()
    {}

    ConferenceMember(std::string ip) : ip_(ip)
    {}

    std::string getIp() const
    {
        return ip_;
    }

    dbo::ptr<Conference> getConference() const
    {
        return conference_;
    }

    void setIp(std::string ip)
    {
        ip_ = ip;
    }

    void setConference(dbo::ptr<Conference> conference)
    {
        conference_ = conference;
    }

    void setConference(Conference* conference)
    {
        conference_ = dbo::ptr<Conference>(conference);
    }

    template<class Act>
    void persist(Act& a)
    {
        dbo::field(a, ip_, "ip");
        dbo::belongsTo(a, conference_, "conference");
    }
private:
    std::string ip_;
    dbo::ptr<Conference> conference_;

};

} // namespace Schema
} // namespace DB

#endif
