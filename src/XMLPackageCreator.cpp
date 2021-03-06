#include "XMLPackageCreator.hpp"
#include <glibmm/ustring.h>


XMLPackageCreator::XMLPackageCreator( const Glib::ustring& type, const Glib::ustring& text )
    : type(type), text(text)
{}
    
Glib::ustring& XMLPackageCreator::operator[]( const Glib::ustring& name )
{
    return (*other.insert(std::make_pair(name,""))).second;
}
    
Glib::ustring XMLPackageCreator::getXML()
{
    Glib::ustring xml = "<?xml version=\"1.0\"?>\n";
    xml+="<massage>\n";
    xml+="\t<type>"+type+"</type>\n";
    xml+="\t<text>\n";
    xml+=text;
    xml+="\n\t</text>\n";    
    for( auto& i : other )
    {
        xml+="\t<"+i.first+">";
        xml+=i.second;
        xml+="</"+i.first+">\n";
    }
    xml+="</massage>\n";
    return xml;
}
