#pragma once

class RevertAPI
{
public:
	RevertAPI();
	~RevertAPI();

public:
    bool canAutoUpdate() const;
	//// query the mount point of C,E, volume D is inaccessible
	//const wchar_t* QueryMountPoint(wchar_t volume);
	//// query the protection status of C/E
	//bool IsProtected(wchar_t volume);
	//// query the new protection setting of C/E
	//bool WillBeProtected(wchar_t volume);
	//// config the new protection setting
	//bool SetProtect(wchar_t volume,bool is_protect);

protected:
	// open dr0
	bool __OpenDevice();
	// fetch the installation key
	bool __Init32BytesKey();
	// fetch the control key
	bool __Init4BytesKey();

protected:
	bool __SendIOCtrl(uint32_t code, void* in,uint32_t inlen,void* out,uint32_t* outlen);

protected:
	enum
	{
		MAX_SIGNATURE_LENGTH = 5,
		MAX_MOUNTPONT_LENGTH = 256,
		MAX_INSTALL_KEY_LENGTH = 0x20,
		SECTOR_SIZE = 512,

		CONFIG_OFFSET = 0x6400,

		VOLUME_C	= 0,
		VOLUME_E	= 1,
		VOLUME_MAX	= 2,
	};

protected:
#include <pshpack1.h>
	struct ConfigArea_t 
	{
		uint8_t sign[MAX_SIGNATURE_LENGTH];
		uint8_t status[VOLUME_MAX];
		uint8_t reserved[9];
		char install_key[MAX_INSTALL_KEY_LENGTH];
	};

	// IOCTL_REVERT_1_QUERY_MOUNT_POINT
	struct QueryMountPoint_t 
	{
		uint32_t length;
		uint32_t key;
		uint8_t volume;
		wchar_t mountpoint[MAX_MOUNTPONT_LENGTH+1];
	};

	// IOCTL_REVERT_4_SET_STATUS,IOCTL_REVERT_5_QUERY_STATUS,IOCTL_REVERT_6_QUERY_STATUS
	struct VolumeStatus_t 
	{
		uint32_t length;
		uint32_t key;
		char install_key[MAX_INSTALL_KEY_LENGTH+1];
		uint8_t volume;
		uint8_t status;
	};

	// IOCTL_REVERT_7_UNKNOWN
	struct Unknown_7_t 
	{
		uint32_t length;
		uint32_t key;
		char install_key[MAX_INSTALL_KEY_LENGTH+1];
	};

	// IOCTL_REVERT_2_UNKNOWN , may be used to switch the protection status of volume D:
	struct Unknown_2_t 
	{
		char unknown[8];
	};
#include <poppack.h>

protected:
	HANDLE dr0_device_;
	char installation_key_[MAX_INSTALL_KEY_LENGTH];// ReadFile DR0 0x6400
	uint32_t control_key_;
	uint32_t control_key_hash_;
	std::wstring mountpoints_[VOLUME_MAX];
	bool ready_;
};