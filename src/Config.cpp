#include "Config.hpp"

Config::Config(const std::string& fileName) throw(FileOpenException)
{
    // Initialization of fromStringToTypeVariableMap (lambda functions).
    fromStringToTypeVariableMap["int"] = [](const std::string& input){ return boost::lexical_cast<int>(input);};
    fromStringToTypeVariableMap["char"] = [](const std::string& input){ return boost::lexical_cast<char>(input);};
    fromStringToTypeVariableMap["string"] = [](const std::string& input){ return boost::lexical_cast<std::string>(input);};

    loadFile(fileName);
}

void Config::loadFile(const std::string& fileName) throw(FileOpenException)
{
// code mostly basis on TinyXML example
/*    TiXmlDocument doc(fileName.c_str());
    if (!doc.LoadFile())
        throw FileOpenException(fileName);

    TiXmlHandle hDoc(&doc);
    TiXmlElement* pElem;
    TiXmlHandle hRoot(0);

    std::string m_name; // check what for it is

	// block: name
	{
		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) return;
		m_name=pElem->Value();

		// save this for later
		hRoot=TiXmlHandle(pElem);
	}

    // block: string table
	{
		m_messages.clear(); // trash existing table

		pElem=hRoot.FirstChild().Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
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
                throw WrongTypeOfValueException(pKey,pText);
            }
			if (pKey.length() > 0 && pText.length() > 0)
			{
				m_messages[pAttr] = value;
			}
		}
	}

	// block: windows
	{
		m_windows.clear(); // trash existing list

		TiXmlElement* pWindowNode=hRoot.FirstChild( "Windows" ).FirstChild().Element();
		for( pWindowNode; pWindowNode; pWindowNode=pWindowNode->NextSiblingElement())
		{
			WindowSettings w;
			const char *pName=pWindowNode->Attribute("name");
			if (pName) w.name=pName;
			
			pWindowNode->QueryIntAttribute("x", &w.x); // If this fails, original value is left as-is
			pWindowNode->QueryIntAttribute("y", &w.y);
			pWindowNode->QueryIntAttribute("w", &w.w);
			pWindowNode->QueryIntAttribute("hh", &w.h);

			m_windows.push_back(w);
		}
	}

	// block: connection
	{
		pElem=hRoot.FirstChild("Connection").Element();
		if (pElem)
		{
			m_connection.ip=pElem->Attribute("ip");
			pElem->QueryDoubleAttribute("timeout",&m_connection.timeout);
		}
	}*/

}
