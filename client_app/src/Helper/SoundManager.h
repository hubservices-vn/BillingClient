#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class SoundManager : public QObject, public Singleton_T<SoundManager>
{
    Q_OBJECT
public:
    static void play(const QString& relativePath);
};

#endif // SOUNDMANAGER_H
