#include "SoundManager.h"
#include <QSound>

void SoundManager::play( const QString& relativePath )
{
    if ( ! QSound::isAvailable())
    {
        return;
    }

    QSound::play(relativePath);
}
