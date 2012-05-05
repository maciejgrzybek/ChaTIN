#pragma once
#include "ChatTabFactory.hpp"

static std::shared_ptr<ChatTabDialog> ChatTabFactory::create( const ChaTIN::Alias& alias )
{
    return std::shared_ptr<ChatTabDialog>(new ChatTabDialog(alias));
}

static std::shared_ptr<ChatTabConference> ChatTabFactory::create( const ChaTIN::ConferenceId& name )
{
    return std::shared_ptr<ChatTabConference>(new ChatTabConference(name));
}

static std::shared_ptr<ChatTabLog> ChatTabFactory::create( const Glib::ustring& name )
{
    return std::shared_ptr<ChatTabLog>(new ChatTabLog(name));
}
