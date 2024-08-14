#include "WallpaperSetter.h"

#include <Windows.h>
#include "wininet.h"
#include <Shlobj.h>

const int DEFAULT_WALLPAPER_OPTION = WPSTYLE_STRETCH;

WallpaperSetter::WallpaperSetter()
{
    _desktop = NULL;

    _init();
}

WallpaperSetter::~WallpaperSetter()
{
    if (_desktop != NULL)
    {
        _desktop->Release();
    }

    CoUninitialize();
}

void WallpaperSetter::setWallpaper( const QString& imageFilePath, int wallpaperOption )
{
    int realWallpaperOption = _getProperImageOption(wallpaperOption);
    _setWallpaperImp(imageFilePath.toStdWString(), realWallpaperOption);
}

bool WallpaperSetter::_init()
{
    CoInitializeEx(0, COINIT_APARTMENTTHREADED);

    HRESULT status = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void**)&_desktop);

    return (SUCCEEDED(status));
}


void WallpaperSetter::_setWallpaperImp( const std::wstring& filePath, int wallpaperOption  )
{
    if ( _desktop == NULL )
    {
        return;
    }

    WALLPAPEROPT wOption;
    ZeroMemory(&wOption, sizeof(WALLPAPEROPT));
    wOption.dwSize = sizeof(WALLPAPEROPT);
    wOption.dwStyle = wallpaperOption;

    HRESULT status = _desktop->SetWallpaper(filePath.c_str(), 0);
    if (FAILED(status))
    {
        //LOG_ERROR("Fail to set wallpaper.");
        return;
    }

    status = _desktop->SetWallpaperOptions(&wOption, 0);
    if (FAILED(status))
    {
        //LOG_ERROR("Fail to set wallpaper options.");
        return;
    }

    status = _desktop->ApplyChanges(AD_APPLY_ALL);
    if (FAILED(status))
    {
        //LOG_ERROR("Fail to apply wallpaper changes.");
        return;
    }
}

int WallpaperSetter::_getProperImageOption( int rawOption )
{
    if (rawOption < 0
        || rawOption >= WPSTYLE_MAX)
    {
        return DEFAULT_WALLPAPER_OPTION;
    }

    return rawOption;
}
