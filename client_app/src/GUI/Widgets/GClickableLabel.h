#ifndef GCLICKABLELABEL_H
#define GCLICKABLELABEL_H

#include <QLabel>
#include <QVariant>
#include <QMap>

class GClickableLabel : public QLabel
{

    Q_OBJECT

public:
    explicit GClickableLabel(QWidget * parent = 0 );
    ~GClickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;
};

#endif // GCLICKABLELABEL_H
