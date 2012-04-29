#include <map>
#include <vector>
#include <glibmm/ustring.h>


/**
 * NOTE: Can we hold aliases? Isnt that bad when we call registerIncomingConfeorence in some case
 *       Should names has other namespace then aliases? 
 *        Or we only create lock(alias) unlock(alias) method in  aliasManager?
 */

/**
 * Class being able to create / remove conference container holding name of conference 
 * with list of its members.
 */
class ConferenceManager
{
    std::map< Glib::ustring /*name*/, std::vector< Glib::ustring /*members*/> conferences;

    /**
     * Method that allows to create confrence but only if name isnt already registred alias
     *  or other conference name.
     * @throw AliasDoesNotExistsException when you try to add conference with unexisting alias
     * @throw AliasAlreadyInUse if name was already used as alias or other conference name
     */
    void addConference( const Glib::ustring& name, const std::vector< Glib::ustring >& members );

    /**
     * Method that can be used to find list of aliases in conference name of which we know
     * @param cosnt Glib::ustring& name of conference
     * @throw ConferenceNotFoundException when there is no conference with given name
     * @return reference to list of aliases in conference given by name in argument
     */
    const std::vector<Glib::ustring>& getList( const Glib::ustring& name );

    /**
     * Remove conference with given name from list
     * @param const Glib::ustring& name of conference
     * @throw ConferenceNotFoundException when there is no conference with given name
     */
    void removeConfrence( const Glib::ustring& name );

    /**
     * Find if incoming name is already in use
     * If it isnt just do addConferece
     * otherwise - create new name for that and try again
     * If you find conference with same name (on one can be generated from it)
     *  it compares vectors of members to find out if its the same conference
     *  if it is does nothing, other wise finds good name for new one and addit
     * note: it can be easliy implemented using recursion
     * members to find out if its the same conference.
     */
    void registerIncomingConference( 
            const Glib::ustring& name, const std::vector< Glib::ustring >& members );
    
    
    

};
