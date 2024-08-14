

#include <fstream>

#include "System/ModuleUtils.h"
#include "System/FileUtils.h"

#include "Crypto/XOREncryptHelper.h"

#include "XML/XMLAutoOpenMem.h"
#include "XML/XMLAutoSaveMem.h"

using SDK::System::ModuleUtils;
using SDK::System::FileUtils;

#include "LocalUpdateConfig.h"
#include "LocalConfig.h"

const unsigned int ENCRYPT_KEY = 20100901;

LocalUpdateConfig::LocalUpdateConfig() : version_(1.0f)
{

}

LocalUpdateConfig::~LocalUpdateConfig()
{

}

bool LocalUpdateConfig::Init()
{
	TCHAR directory[MAX_PATH] = {0};
	ModuleUtils::GetModuleDirectory(NULL, directory);
	
	std::wstring update_file(directory);
	update_file.append(L"\\update.dat");

	if (!FileUtils::Exists(update_file.c_str()))
	{
		return false;
	}

	LocalConfig config;
	
	try
	{
		std::ifstream file_stream;

		file_stream.open(update_file.c_str(), std::ios_base::in | std::ios_base::binary);
		if (file_stream.fail())
		{
			return false;
		}

		// Get file size
		file_stream.seekg(0, std::ios::end);
		size_t size = file_stream.tellg();
		file_stream.seekg(0);   

		// Read file data to data_buffer
		std::vector<char> data_buffer;
		data_buffer.resize(size);
		file_stream.read(&data_buffer[0], size);
		file_stream.close();

		// Encrypt file data
		std::string decrypt_data;
		decrypt_data.resize(data_buffer.size());

        SDK::Crypto::XOREncryptHelper::Encrypt(&data_buffer[0], data_buffer.size(), 
			&decrypt_data[0], ENCRYPT_KEY);

		XML_AUTO_OPEN_MEM(decrypt_data);
		XMLSerializer ss(XML_MEM_READ_ROOT_NODE("update"));
		config.Serialize(&ss, false);
	}
	catch(...)
	{
		return false;
	}

	version_ = config.version_;
    return true;
}

float LocalUpdateConfig::GetVersion()
{
	return version_;
}