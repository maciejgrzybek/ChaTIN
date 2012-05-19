#include "Config.hpp"

Config::Config(const std::string& fileName) throw(FileOpenException)
{
    // Initialization of fromStringToTypeVariableMap (lambda functions).
    fromStringToTypeVariableMap[std::string("int")] = [](const std::string& input){ return SupportedTypesVariant(boost::lexical_cast<int>(input));};
    fromStringToTypeVariableMap[std::string("char")] = [](const std::string& input){ return SupportedTypesVariant(boost::lexical_cast<char>(input));};
    fromStringToTypeVariableMap[std::string("string")] = [](const std::string& input){ return SupportedTypesVariant(boost::lexical_cast<std::string>(input));};

    loadFile(fileName);
}

void Config::loadFile(const std::string& fileName) throw(FileOpenException,DataParsingException)
{
// code mostly basis on TinyXML example
    TiXmlDocument doc(fileName.c_str());
    if (!doc.LoadFile())
        throw FileOpenException(fileName);

    TiXmlHandle hDoc(&doc);
    TiXmlElement* pElem;
    TiXmlHandle hRoot(0);

    std::string m_name; // check what for it is

	// block: name
	{
		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root. If does not - throw exception.
		if (!pElem)
            throw XML::XMLParsingException(fileName);

		m_name=pElem->Value();

		// save this for later
		hRoot=TiXmlHandle(pElem);
	}

    // <configuration>
	{
		config.clear(); // trash existing table

		pElem=hRoot.FirstChild().Element();
		for(; pElem; pElem=pElem->NextSiblingElement())
		{
			std::string pKey = pElem->Value();
			std::string pText = pElem->GetText();
            std::string pAttr = pElem->Attribute("name");
            SupportedTypesVariant value;
            try
            {
                value = fromStringToTypeVariableMap[pKey](pText);
            }
            catch(boost::bad_lexical_cast&)
            {
                throw XML::WrongTypeOfValueException(pKey,pText);
            }
			if (pKey.length() > 0 && pText.length() > 0)
			{
			    config[pAttr] = value;
			}
        }
	}

}
