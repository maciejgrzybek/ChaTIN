#include "types.hpp"
#include "Socket.hpp"

namespace ChaTIN
{
    Alias::Alias( const IPv6& ip ) : Glib::ustring( static_cast<Glib::ustring>(ip) )
    {}

    IPv6::IPv6( const Alias& alias ) throw(Socket::WrongAddressException) : Glib::ustring( static_cast<Glib::ustring>(alias) )
    {
        if(!Socket::Socket::isValidIP(static_cast<Glib::ustring>(alias)))
            throw Socket::WrongAddressException(static_cast<std::string>(static_cast<Glib::ustring>(alias)),0);
    }

    IPv6::IPv6(const std::string& ip) throw(Socket::WrongAddressException) : Glib::ustring(static_cast<Glib::ustring>(ip))
    {
        if(!Socket::Socket::isValidIP(ip))
            throw Socket::WrongAddressException(ip,0);
    }

    bool ConferenceId::operator==( const ConferenceId& r ) const
    {
        return ( ownerip == r.ownerip ) && (name == r.name );
    }

    IncomingMassage::IncomingMassage( const ChaTIN::Alias& alias, const Glib::ustring& msg )
        : alias(alias), msg(msg)
    {}

}; /*namespace ChaTIN*/
