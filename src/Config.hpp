#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <unordered_map>
#include <boost/variant.hpp>
#include <tinyxml.h>
#include <tinystr.h>
#include "Exception.hpp"

class Config
{
public:
    /**
     * Typedef for map holding accepted types of keys and values corresponding to them.
     */
    typedef std::unordered_map<boost::variant<int,char,std::string>,std::string> OptionsMap;

    /**
     * Config constructor.
     * @param const std::string& Reference to name of file containing configuration in XML.
     * @throw FileOpenException Exception thrown when configuration file cannot be opened.
     */
    Config(const std::string& = "config.xml") throw(FileOpenException);

    /**
     * Method returns value of given key from config.
     * @param const std::string& Key name to get it's value.
     * @return T Value of given key of type as template requests.
     */
    template<class T>
    T getValue(const std::string&);
};

#endif
