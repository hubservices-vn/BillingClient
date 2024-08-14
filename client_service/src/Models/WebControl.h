#ifndef WEBCONTROL_H
#define WEBCONTROL_H

#include <QObject>
#include "stdTypes.h"
#include "SmartPtrs.h"

enum HostAction
{
    HostAction_NA = -1,
    HostAction_Append = 1,
    HostAction_Update,
    HostAction_Delete,
};

// the change applying for hosts file
class WebControlTask
{
public:
    explicit WebControlTask(uint64_t id, const QString& oldUrl, 
        const QString& newUrl, HostAction action)
        : ControlId(id), OldUrl(oldUrl), NewUrl(newUrl), Action(action)
    {}

public:
    uint64_t ControlId;
    QString OldUrl;
    QString NewUrl;
    HostAction Action;
};

DECLARE_SHARED_PTR(WebControlTask);
DEFINE_NULL_PTRS(WebControlTask);

class WebControlNetworkPackItem
{
public:
    explicit WebControlNetworkPackItem(uint64_t id, const QString& url, 
        const QDateTime& updateDT, bool active, bool enabled)
        : Id(id), Url(url), DateTime(updateDT), Active(active), Enabled(enabled)
    {}

public:
    uint64_t Id;
    QString Url;
    QDateTime DateTime;
    bool Active;
    bool Enabled;
};

class WebControl
{
public:
    explicit WebControl(uint64_t controlId, const QString& url, const QDateTime& updateDT);

public: // accessors
    uint64_t controlId() const { return m_controlId; }
    void setControlId(uint64_t val) { m_controlId = val; }

    QString url() const { return m_url; }
    void setUrl(const QString& val) { m_url = val; }

    QDateTime updateDT() const { return m_updateDT; }
    void setUpdateDT(const QDateTime& val) { m_updateDT = val; }

private:
    uint64_t m_controlId;
    QString m_url;
    QDateTime m_updateDT;
};

#endif // WEBCONTROL_H
