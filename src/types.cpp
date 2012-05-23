#include "types.hpp"
#include "Socket.hpp"
#include "ChatTab.hpp"

namespace ChaTIN
{
    LogName::LogName( const Glib::ustring& name ) : Glib::ustring(name)
    {}    

    TabIdType LogName::getType() const
    {
        return ChaTIN::LOG;
    }

    TPtr LogName::createTab() const
    {
        return TPtr( new ChatTabLog(*this) );
    }

    bool LogName::operator==( const LogName& toCmp ) const
    {
        return std::string(toCmp)==std::string(*this);
    }

    bool LogName::operator==( const TabId& toCmp ) const
    {
        return getType()==toCmp.getType() && *this==(const LogName&)toCmp;
    }

    Alias::Alias( const IPv6& ip ) : Glib::ustring( static_cast<Glib::ustring>(ip) )
    {}
    Alias::Alias( const Glib::ustring& alias ) : Glib::ustring(alias)
    {}

    TabIdType Alias::getType() const
    {
        return ChaTIN::DIALOG;
    }

    TPtr Alias::createTab() const
    {
        return TPtr( new ChatTabDialog(*this) );
    }

    bool Alias::operator==( const Alias& toCmp ) const
    {
        return std::string(toCmp)==std::string(*this);
    }

    bool Alias::operator==( const TabId& toCmp ) const
    {
        return getType()==toCmp.getType() && *this==(const Alias&)toCmp;
    }

    IPv6::IPv6(const std::string& ip) throw(Socket::WrongAddressException) : Glib::ustring(static_cast<Glib::ustring>(ip))
    {
        if(!Socket::Socket::isValidIP(ip))
            throw Socket::WrongAddressException(ip,0);
    }

    ConferenceId::ConferenceId( const IPv6& ownerip, const Glib::ustring& name )
        : ownerip(ownerip), name(name)
    {}

    bool ConferenceId::operator==( const ConferenceId& r ) const
    {
        return ( ownerip == r.ownerip ) && (name == r.name );
    }

    TabIdType ConferenceId::getType() const
    {
        return ChaTIN::CONFERENCE;
    }

    TPtr ConferenceId::createTab() const
    {
        return TPtr( new ChatTabConference(*this) );
    }

    bool ConferenceId::operator==( const TabId& toCmp ) const
    {
        return getType()==toCmp.getType() && *this==(const ConferenceId&)toCmp;
    }



    IncomingMassage::IncomingMassage( const ChaTIN::IPv6& ip, const Glib::ustring& msg )
        : ip(ip), msg(msg)
    {}

}; /*namespace ChaTIN*/
