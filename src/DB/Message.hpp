#ifndef SCHEMA_MESSAGE_HPP
#define SCHEMA_MESSAGE_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "Conference.hpp"
#include "../types.hpp"

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{

class Message
{
public:
    template<class Act>
    void persist(Act& a)
    {
        dbo::field(a, id, "id");
        dbo::field(a, content, "content");
        dbo::field(a, date, "date");
        dbo::field(a, ip, "ip");
        dbo::field(a, isOutgoing, "isOutgoing");
        dbo::belongsTo(a, conference, "conferenceId");
    }
private:
    int id;
    std::string content;
    int date;
    std::string ip;
    int isOutgoing;
    dbo::ptr<Conference> conference;

};

} // namespace Schema
} // namespace DB

#endif
