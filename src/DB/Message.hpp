#ifndef SCHEMA_MESSAGE_HPP
#define SCHEMA_MESSAGE_HPP

#include <Wt/Dbo/Dbo>
#include <string>
#include <ctime>

#include "../types.hpp"
#include "Conference.hpp"

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{

class Conference;

class Message
{
public:
    Message( std::string ip, std::string content, bool isOutgoing )
        : ip_(ip), content_(content), isOutgoing_(isOutgoing), date_(time(NULL))
    {}

    Message()
    {}

    std::string getContent() const
    {
        return content_;
    }

    int getDate() const
    {
        return date_;
    }

    std::string getIp() const
    {
        return ip_;
    }

    int getOutgoing() const
    {
        return isOutgoing_;
    }

    dbo::ptr<Conference> getConference() const
    {
        return conference_;
    }

    void setContent(std::string content)
    {
        content_ = content;
    }

    void setDate(int date)
    {
        date_ = date;
    }

    void setIp(std::string ip)
    {
        ip_ = ip;
    }

    void setOutgoing(int outgoing)
    {
        isOutgoing_ = outgoing;
    }

    void setConference(dbo::ptr<Conference> conference)
    {
        conference_ = conference;
    }

    void setConference(Conference* conference)
    {
//        dbo::ptr<Conference> conf(conference);
        conference_ = dbo::ptr<Conference>(conference);
    }

    template<class Act>
    void persist(Act& a)
    {
//        dbo::field(a, id, "id");
        dbo::field(a, content_, "content");
        dbo::field(a, date_, "date");
        dbo::field(a, ip_, "ip");
        dbo::field(a, isOutgoing_, "isOutgoing");
        dbo::belongsTo(a, conference_, "conference");
    }
private:
//    int id;
    std::string content_;
    int date_;
    std::string ip_;
    int isOutgoing_;
    dbo::ptr<Conference> conference_;

};

} // namespace Schema
} // namespace DB

#endif
