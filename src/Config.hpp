#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

class Config
{
public:
    /**
     * Typedef for map holding accepted types of keys and values corresponding to them.
     */
    typedef std::unordered_map<boost::variant<int,char,std::string>,std::string> OptionsMap;

    /**
     * Singleton instance getter.
     * @param const std::string& Reference to name of file containing configuration in XML.
     * @return Config* Pointer to instance of Config class.
     */
    static Config* getInstance(const std::string& = "config.xml");

    /**
     * Method returns value of given key from config.
     * @param const std::string& Key name to get it's value.
     * @return T Value of given key of type as template requests.
     */
    template<class T>
    T getValue(const std::string&);
private:
    /**
     * Config constructors are private, because we don't want anyone instantiating this class without getInstance() method.
     * @param const std::string& Reference to name of file containing configuration in XML.
     */
    Config(const std::string& = "config.xml");
    Config(const Config&);
    Config& operator=(Config&);
};

#endif
