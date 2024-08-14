#include "WebControl.h"


WebControl::WebControl( uint64_t controlId, const QString& url, const QDateTime& updateDT )
: m_controlId(controlId), m_url(url), m_updateDT(updateDT)
{
}
