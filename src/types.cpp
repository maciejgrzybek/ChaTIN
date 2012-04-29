#include "types.hpp"
#include "Socket.hpp"

namespace ChaTIN
{
    Alias::Alias( const IPv6& ip ) : Glib::ustring( static_cast<Glib::ustring>(ip) )
    {}

    IPv6::IPv6( const Alias& alias ) : Glib::ustring( static_cast<Glib::ustring>(alias) )
    {
        if( !Socket::Socket::isValidIP(static_cast<Glib::ustring>(alias)) )
            ;//FIXME throw CannotCastToIPv6Exception
    }
}; /*namespace ChaTIN*/
