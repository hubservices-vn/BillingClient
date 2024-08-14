#ifndef GSKINMAINWINDOW_H
#define GSKINMAINWINDOW_H

#include <QMainWindow>

class GSkinMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GSkinMainWindow(QWidget *parent = 0);

signals:

public slots:

protected: // event handler
    
    void paintEvent(QPaintEvent* event);
    //void resizeEvent ( QResizeEvent * event );
    void mousePressEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );

private:
    bool isPointInCloseButton( const QPoint& point );
    bool isPointInMinButton( const QPoint& point );
    bool isPointInMaxButton( const QPoint& point );
    bool isPointInIconArea( const QPoint& point );
    bool isPointInTitleArea( const QPoint& point );
};

#endif // GSKINMAINWINDOW_H
