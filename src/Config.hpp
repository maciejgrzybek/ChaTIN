#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <unordered_map>
#include <boost/variant.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#define TIXML_USE_STL 
#include <tinyxml.h>
#include <tinystr.h>
#include "Exception.hpp"
#include "XMLException.hpp"
#include <string>

class Config
{
public:
    /**
     * Typedef for boost::variant of accepting types from XML.
     */
    typedef boost::variant<int,char,std::string> SupportedTypesVariant;
    /**
     * Typedef for map holding accepted types of keys and values corresponding to them.
     */
    typedef std::unordered_map<std::string,SupportedTypesVariant> OptionsMap;

    /**
     * Config constructor.
     * @param const std::string& Reference to name of file containing configuration in XML.
     * @throw FileOpenException Exception thrown when configuration file cannot be opened.
     */
    Config(const std::string& = "config.xml") throw(FileOpenException);

    /**
     * Loads configuration file.
     * @param const std::string& Reference to name of file containing configuration in XML.
     * @throw FileOpenException Exception thrown when configuration file can not be opened.
     * @throw DataParsingException Exception thrown when XML format from file does not correspond suitable format.
     */
    void loadFile(const std::string& = "config.xml") throw(FileOpenException,DataParsingException);

    /**
     * Method returns value of given key from config.
     * @param const std::string& Key name to get it's value.
     * @return T Value of given key of type as template requests.
     */
    template<class T>
    T getValue(const std::string& key) const
    {
        OptionsMap::const_iterator iter = config.find(key);
        if(iter == config.end())
            throw XML::ValueNotExistsException(key);
        return boost::get<T>(iter->second);
    }

    template<class T>
    T getValue(const char* key) const
    {
        std::string str(key);
        return getValue<T>(str);
    }

protected:
    /**
     * Function returns value of given expression after casting to one of supported types (boost::variant).
     */
    //typedef boost::function<SupportedTypesVariant (const std::string&)> CastFunction;
    typedef std::function<SupportedTypesVariant (const std::string&)> CastFunction;

    /**
     * Map of key=>value, where key is type to cast value given as parameter to value's function.
     * Value is boost::function callback which returns casted value, as key specifies. 
     * I.e. fromStringToTypeVariableMap["int"]("12"); will cast string "12" into int type, by given function (which uses boost::lexical_cast).
     * Consider moving this map static.
     */
    std::unordered_map<std::string, CastFunction> fromStringToTypeVariableMap;

    OptionsMap config;

};

#endif
