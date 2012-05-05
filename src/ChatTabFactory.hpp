#pragma once
#include "ChatTab.hpp"
#include "types.hpp"
#include <memory>
#include <glibmm/ustring.h>

/**
 * This is static factory creating 
 * ChatTab object based on input infomations
 */
class ChatTabFactory
{    
    public:

    /**
     * Creates dialog tab
     */
    static std::shared_ptr<ChatTabDialog> create( const ChaTIN::Alias& alias );

    /**
     * Creates conference tab
     */
    static std::shared_ptr<ChatTabConference> create( const ChaTIN::ConferenceId& name );

    /**
     * Creates log tab
     */
    static std::shared_ptr<ChatTabLog> create( const Glib::ustring& name );
}; 
