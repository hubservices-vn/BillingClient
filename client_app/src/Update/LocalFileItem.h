#ifndef AUTOUPDATE_LOCALFILEITEM_H__
#define AUTOUPDATE_LOCALFILEITEM_H__

#include "XML/XMLSerializer.h"
#include "XML/XMLHelper.h"

using namespace SDK::XML;

class LocalFileItem
{
public:
	LocalFileItem() : version_(0.0f)
	{

	}

	LocalFileItem(const LocalFileItem & item)
	{
		path_ = item.path_;
		version_ = item.version_;
	}

public:
	BEGIN_XML_SERIALIZER(LocalFileItem)
		XML_ATTRIBUTE(path_, path)
		XML_ATTRIBUTE(version_, version)
	END_XML_SERIALIZER()

public:
	std::wstring path_;
	float version_;
};

#endif // AUTOUPDATE_LOCALFILEITEM_H__
