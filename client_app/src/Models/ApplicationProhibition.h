#ifndef APPLICATIONPROHIBITION_H
#define APPLICATIONPROHIBITION_H

#include <QObject>

enum AppProhibitionType;
class ApplicationProhibition
{
public:
    explicit ApplicationProhibition(int id, AppProhibitionType prohibitType, const QString& fileName, 
        const QString& param, bool enabled = true);

    explicit ApplicationProhibition(int id, AppProhibitionType prohibitType, const QString& fileName, 
        const QString& param, const QString& oldParam, bool enabled = true);

    explicit ApplicationProhibition(int id, int prohibitTypeVal, const QString& fileName, 
        const QString& param, const QString& oldParam, bool enabled = true);

    explicit ApplicationProhibition(const ApplicationProhibition& other);

    int id() const { return m_id; }
    void setId(int val) { m_id = val; }

    AppProhibitionType prohibitType() const { return m_prohibitType; }
    void setProhibitType(AppProhibitionType val) { m_prohibitType = val; }

    QString fileName() const { return m_fileName; }
    void setFileName(const QString& val) { m_fileName = val; }

    QString params() const { return m_params; }
    void setParams(const QString& val) { m_params = val; }

    QString oldParams() const { return m_oldParams; }
    void setOldParams(const QString& val) { m_oldParams = val; }

    bool enabled() const { return m_enabled; }
    void setEnabled(bool val) { m_enabled = val; }

private:
    int m_id;
    AppProhibitionType m_prohibitType;
    QString m_fileName; // it can be file name or file path
    QString m_params; // separated with "|"
    QString m_oldParams;
    bool m_enabled;
};

#endif // APPLICATIONPROHIBITION_H
