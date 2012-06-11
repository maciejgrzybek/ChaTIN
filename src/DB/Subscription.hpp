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
    Subscription() : ip_(), state_(0)
    {}

    Subscription(std::string ip, int state) : ip_(ip), state_(state)
    {}

    std::string getIP() const
    {
        return ip_;
    }

    int getState() const
    {
        return state_;
    }

    int getDate() const
    {
        return date_;
    }

    void setIP(std::string& ip)
    {
        ip_ = ip;
    }

    void setState(int state)
    {
        state_ = state;
    }

    void setDate(int date)
    {
        date_ = date;
    }

    template<class Act>
    void persist(Act& a)
    {
        dbo::id(a, ip_, "ip");
        dbo::field(a, state_, "state");
        dbo::field(a, date_, "date");
    }
private:
    std::string ip_;
    int state_;
    int date_;

};

} // namespace Schema
} // namespace DB

namespace Wt
{
    namespace Dbo
    {

        template<>
        struct dbo_traits<DB::Schema::Subscription> : public dbo_default_traits
        {
            typedef std::string IdType;

            static IdType invalidId() {
                return std::string();
            }

            static const char *surrogateIdField() { return 0; }
        };

    }
}


#endif
