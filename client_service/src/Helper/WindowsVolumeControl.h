#ifndef WINDOWSVOLUMECONTROL_H
#define WINDOWSVOLUMECONTROL_H

// disable the "bool <--> BOOL" conversion warning
#pragma warning(disable: 4800)

enum WinOsVersion;
class WindowsVolumeControl
{
public:
    // return value in range [0, 100]
    static int getMasterVolume();

    // ensure "volValue" is in range [0, 100]
    static bool setMasterVolume(int volValue);

    static bool isMuted();
    static bool setMute(bool mute);

private:
    static WinOsVersion _getWinOsVer();
};

#endif // WINDOWSVOLUMECONTROL_H
