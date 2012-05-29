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
    template<class Act>
    void persist(Act& a)
    {
        dbo::field(a, ip, "ip");
        dbo::belongsTo(a, conference, "conferenceId");
    }
private:
    std::string ip;
    dbo::ptr<Conference> conference;

};

} // namespace Schema
} // namespace DB

#endif
