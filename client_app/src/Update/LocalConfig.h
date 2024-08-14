#ifndef AUTOUPDATE_LOCALCONFIG_H__
#define AUTOUPDATE_LOCALCONFIG_H__

#include <vector>

#include "LocalFileItem.h"

class LocalConfig
{
public:
	BEGIN_XML_SERIALIZER(LocalConfig)
		XML_ATTRIBUTE(version_, version)
		XML_ELEMENT_OBJECTS(file_items_, file)
	END_XML_SERIALIZER()
	
public:
	float version_;
	std::vector<LocalFileItem> file_items_;
};

#endif // AUTOUPDATE_LOCALCONFIG_H__