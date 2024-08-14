#ifndef OSAGENTWIN_H
#define OSAGENTWIN_H

#pragma warning (disable: 4805 4995)

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QList>
#include <QScopedPointer>


class RemoteSubClass;
class OSAgentWin 
{
public:
    OSAgentWin();
    virtual ~OSAgentWin();

    virtual bool disableSystemHotKey( void );
    virtual bool enableSystemHotKey( void );

    virtual bool hideTaskBar( void );
    virtual bool showTaskBar( void );

    virtual bool hideStartMenu( void );
    virtual bool showStartMenu( void );

    virtual bool disableCAD( void );
    virtual bool enableCAD( void );

protected:
    bool m_taskbarHidden;
    bool m_startmenuHidden;
    bool m_hotkeyDisabled;
    bool m_cadDisabled;

    QScopedPointer<RemoteSubClass> m_subClass; // used to disable CAD
};

#endif // OSAGENTWIN_H
