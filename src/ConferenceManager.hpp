#include <map>
#include <vector>
#include <glibmm/ustring.h>


/**
 * Class being able to create / remove conference container holding name of conference 
 * with list of its members.
 */
class ConferenceManager
{
    std::map< std::pair<Glib::ustring /*name*/, Glib::ustring /*owner ipv6*/>, std::vector< Glib::ustring /*members*/> conferences;

    /**
     * Method that allows to create confrence but only if name isnt already registred name 
     *  or other conference name and owner ::1.
     * @throw NameDoesNotExistsException when you try to add conference with unexisting name
     * @throw NameAlreadyInUse if name was already usede
     */
    void addConference( const Glib::ustring& name, const std::vector< Glib::ustring >& members );

    /**
     * Method that can be used to find list of ipv6 in conference given by name
     * @param cosnt Glib::ustring& name of conference
     * @throw ConferenceNotFoundException when there is no conference with given name
     * @return reference to list of ipv6 in conference given by name in argument
     */
    const std::vector<Glib::ustring>& getList( const Glib::ustring& name );

    /**
     * Remove conference with given name from list
     * @param const Glib::ustring& name of conference
     * @throw ConferenceNotFoundException when there is no conference with given name
     */
    void removeConfrence( const Glib::ustring& name );

    /**
     * Find if incoming name+ipv6 pair is already in use
     * If it isnt just do addConferece
     * otherwise - create new name for that and try again
     * If you find conference with same name+ipv6 pair
     *  compare vectors of members to find out if its the same conference
     *  if it is do nothing, otherwise find new name (~1,~2,~3...) for new one and add it
     */
    void registerIncomingConference( 
            const Glib::ustring& name, const Glib::ustring& owner, const std::vector< Glib::ustring >& members );
    
};
