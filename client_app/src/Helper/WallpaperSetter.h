#ifndef WALLPAPER_SETTER_H
#define WALLPAPER_SETTER_H

#include <QString>
#include <string>

extern const int DEFAULT_WALLPAPER_OPTION;

struct IActiveDesktop;
class WallpaperSetter
{
public:
    explicit WallpaperSetter();
    ~WallpaperSetter();

    // "imageOption" : center / title / stretch (value of WPSTYLE_STRETCH, etc)
    void setWallpaper(const QString& imageFilePath, int imageOption = -1);

private:
    bool _init();

    void _setWallpaperImp(const std::wstring& filePath, int wallpaperOption );

    int _getProperImageOption(int rawOption);
private:
    IActiveDesktop* _desktop;
};

#endif // WALLPAPER_SETTER_H