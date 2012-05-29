#ifndef SCHEMA_SUBSCRIPTION_HPP
#define SCHEMA_SUBSCRIPTION_HPP

#include <Wt/Dbo/Dbo>
#include <string>

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{

class Subscription
{
public:
    template<class Act>
    void persist(Act& a)
    {
        dbo::field(a, ip, "ip");
        dbo::field(a, state, "state");
        dbo::field(a, date, "date");
    }
private:
    std::string ip;
    int state;
    int date;

};

} // namespace Schema
} // namespace DB

#endif
