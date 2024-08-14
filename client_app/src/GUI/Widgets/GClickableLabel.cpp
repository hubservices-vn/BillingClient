#include "GClickableLabel.h"
#include <QKeyEvent>

GClickableLabel::GClickableLabel(QWidget * parent ) :
QLabel(parent)

{
}

GClickableLabel::~GClickableLabel()
{
}

void GClickableLabel::mousePressEvent ( QMouseEvent * event )

{
    emit clicked();
}