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
        dbo::id(a, ip, "ip");
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
