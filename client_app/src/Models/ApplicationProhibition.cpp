#include "ApplicationProhibition.h"

ApplicationProhibition::ApplicationProhibition( 
    int id, AppProhibitionType prohibitType, const QString& fileName, const QString& param, bool enabled /* = true*/ )
    : m_id(id), m_prohibitType(prohibitType), m_fileName(fileName), m_params(param), m_enabled(enabled)
{
}

ApplicationProhibition::ApplicationProhibition( 
    int id, AppProhibitionType prohibitType, const QString& fileName, const QString& param, const QString& oldParam, bool enabled /* = true*/ )
    : m_id(id), m_prohibitType(prohibitType), m_fileName(fileName), m_params(param), m_oldParams(oldParam), m_enabled(enabled)
{
}

ApplicationProhibition::ApplicationProhibition( 
    int id, int prohibitTypeVal, const QString& fileName, const QString& param, const QString& oldParam, bool enabled /* = true*/ )
    : m_id(id), m_prohibitType((AppProhibitionType)prohibitTypeVal), m_fileName(fileName), m_params(param), m_oldParams(oldParam), m_enabled(enabled)
{
}

ApplicationProhibition::ApplicationProhibition( 
    const ApplicationProhibition& other )
    : m_id(other.id()), m_prohibitType(other.prohibitType()), m_fileName(other.fileName()),
    m_params(other.params()), m_oldParams(other.oldParams()), m_enabled(other.enabled())
{
}
