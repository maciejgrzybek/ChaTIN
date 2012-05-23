#pragma once
#include <glibmm/ustring.h>
#include <map>

/**
 * This class is used to build ready to send XML packages based on given informations
 * There is huge possibility that simple static function in FromViewParser
 *  would be not only simpler but also better solution. Well I like trains...
 *  ekhm... I mean classes ;-)
 */
class XMLPackageCreator
{
    public:
    /** 
     * This is the map name->value for other information
     *  which can be find in xml and can be usefull in some contexts
     *  (eg. conference name for conference income)
     */
    typedef std::multimap<Glib::ustring, Glib::ustring> OtherAttributes;

    private:
    Glib::ustring type;
    Glib::ustring text;    
    OtherAttributes other;

    public:

    /**
     * Constructor taking two obligatory elements of every XML package in this system
     * @param const Glib::ustring& type - type of package just created
     * @param const Glib::ustring& text - text to include to XML package
     */
    XMLPackageCreator( const Glib::ustring& type, const Glib::ustring& text );
    
    /**
     * Accesor to additional field in xml massage
     * @param const Glib::ustring& name - name of field you want access to
     * @return Glib::ustring& reference to field to modyfi
     * FIXME it works in not obvious way
     */
    Glib::ustring& operator[]( const Glib::ustring& name );
    
    /**
     * Generate ready to send xml package with informatations from object
     *  as string.
     * @return Glib::ustring ready to send with DialogManager string with XML package
     */
    Glib::ustring getXML();
};
