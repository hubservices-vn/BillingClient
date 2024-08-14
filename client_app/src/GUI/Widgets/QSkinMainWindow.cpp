/********************************************************************************************************
 * PROGRAM      : childform
 * DATE - TIME  : Samstag 30 Dezember 2006 - 12h04
 * AUTHOR       :  (  )
 * FILENAME     : QSkinMainWindow.cpp
 * LICENSE      :
 * COMMENTARY   :
 ********************************************************************************************************/
#include <QtGui>
#include "QSkinMainWindow.h"
QSkinMainWindowResizeHandler::QSkinMainWindowResizeHandler(QSkinMainWindow *parent, QWidget *cw) : QWidgetResizeHandler(parent, cw)
{
    skinWidget = parent;
}
bool QSkinMainWindowResizeHandler::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
        skinWidget->mousePressEvent(dynamic_cast<QMouseEvent*>(e));
    return QWidgetResizeHandler::eventFilter(o,e);
}

QSkinMainWindow::QSkinMainWindow( QWidget* wgtParent)
    : QMainWindow(wgtParent)

{
    //no windows frame
    setWindowFlags(Qt::FramelessWindowHint);
    //Minimum width and height else our application will disappear by resizing
    setWindowTitle("QSkinMainWindow");
    setMinimumSize (100, 32);
    alpha = 255;
    widgetMask = QPixmap(width(), height());
    //let the window repaint every 400ms (is needed for the close-button)
    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(400);
    //mouse tracking on, because the close-button should glow if the cursor comes over it
    setMouseTracking(true);
    loadSkinIni();
    //content marginst because of the own Frame
    setContentsMargins ( QPixmap(skinPath + "sl.png").width()+5, QPixmap(skinPath + "so.png").height()+5, QPixmap(skinPath + "sr.png").width()+5, QPixmap(skinPath + "su.png").height()+5);
    resizeHandler =  new QSkinMainWindowResizeHandler(this);
    resizeFrame = true;

}
void QSkinMainWindow::manageRegions()
{
    leftFrame 			= QRegion( QRect(0, 5, 5, height()-10) );
    topFrame 			= QRegion( QRect(5, 0, width()-10, 5) );
    rightFrame 			= QRegion( QRect(width()-5, 5, 5, height()-10) );
    bottomFrame 		= QRegion( QRect(5, height()-5, width()-10, 5) );
    topLeftFrame 		= QRegion( QRect(0, 0, 5, 5) );
    bottomLeftFrame 	= QRegion( QRect(0, height() - 5, 5, 5) );
    topRightFrame 		= QRegion( QRect(width()-5, 0, 5, 5) );
    bottomRightFrame 	= QRegion( QRect(width()-5, height()-5, 5, 5) );
}
void QSkinMainWindow::getSelectedFrame(QPoint p)
{
    if(leftFrame.contains(p))
        resizePosition = Left;
    else if(topFrame.contains(p))
        resizePosition = Top;
    else if(rightFrame.contains(p))
        resizePosition = Right;
    else if(bottomFrame.contains(p))
        resizePosition = Bottom;
    else if(topLeftFrame.contains(p))
        resizePosition = TopLeft;
    else if(bottomLeftFrame.contains(p))
        resizePosition = BottomLeft;
    else if(topRightFrame.contains(p))
        resizePosition = TopRight;
    else if(bottomRightFrame.contains(p))
        resizePosition = BottomRight;
    else
        resizePosition = None;

}
void QSkinMainWindow::closeEvent(QCloseEvent *e)
{
    emit aboutToClose();
}
void QSkinMainWindow::resizeEvent(QResizeEvent *e)
{
    manageRegions();
    resizeFrame = true;
}
void QSkinMainWindow::mousePressEvent(QMouseEvent *e)
 {
    // winwow Mover

    QPoint pos = e->pos();
    QRect rect = this->rect();
    if (e->button() == Qt::LeftButton)
    {

        dragPosition = e->globalPos() - frameGeometry().topLeft();
        // buttons
        const int buttonRight = 10;
        const int buttonWidth = QPixmap(skinPath + "quiButton.png").width();
        int butPos = (QPixmap(skinPath + "so.png").height() - QPixmap(skinPath + "quiButton.png").height()+6);
        int right = rect.right() - pos.x();
        int button = (right - buttonRight)/buttonWidth;
        if(pos.x() < width() - 5)
            if(pos.y() < QPixmap(skinPath + "so.png").height() + butPos && pos.y() > 6)
            {
                switch (button) {
                    case 0:
                     close();
                     e->accept();
                     break;
                    case 1:
                     if(isMaximized())
                        showNormal();
                     else
                        showMaximized();
                     e->accept();
                     break;
                    case 2:
                     showMinimized();
                     e->accept();
                     break;
                }
            }
            else
                e->accept();

    }

 }
void QSkinMainWindow::mouseReleaseEvent(QMouseEvent *mouseEvent)
{

}
void QSkinMainWindow::mouseMoveEvent(QMouseEvent *e)
{

}





void QSkinMainWindow::setSkinPath(const QString & skinpath)
{
    skinPath = skinpath;
    update();

}
QString QSkinMainWindow::getSkinPath()
{
    return skinPath;
}

void QSkinMainWindow::updateStyle()
{

    //this makes the alpha blending work. just gdi funktions and a conversation from QPixmap to HBITMAP. not really interresting
    QPixmap so = QPixmap(skinPath + "so.png");
    QPixmap sl = QPixmap(skinPath + "sl.png");
    QPixmap sr = QPixmap(skinPath + "sr.png");
    QPixmap su = QPixmap(skinPath + "su.png");
    QPixmap min = QPixmap(skinPath + "minButton.png");
    QPixmap max = QPixmap(skinPath + "maxButton.png");
    QPixmap qui = QPixmap(skinPath + "quiButton.png");

    widgetMask = QPixmap(width(), height());
    widgetMask.fill(Qt::transparent);
    QPainter p(&widgetMask);


    p.drawPixmap(QRect(width()-(so.width()-68), 0,so.width()-68, so.height()), so, QRect(68, 0 , so.width()-68, so.height()));
    p.drawPixmap(QRect(0, 0, 64, so.height()), so, QRect(0, 0 , 64, so.height()));
    p.drawPixmap(QRect(64, 0, width()-(so.width()-4), so.height()), so, QRect(64, 0, 4, so.height()));


    p.drawPixmap(QRect(sl.width(), height()-su.height(), width(), su.height()), su);

    p.drawPixmap(QRect(0, height() - su.height(), sl.width(), su.height()),
                su,
                QRect(0, 0 , 4, su.height()));

    p.drawPixmap(QRect(sl.width(), height() - su.height(), width()-sl.width()-sr.width(), su.height()),
                su,
                QRect(4, 0, 2, su.height()));
    p.drawPixmap(QRect(width()-sr.width(), height() - su.height(), sr.width(), su.height()),
                su,
                QRect(su.width()-4, 0 , 4, su.height()));


    p.drawPixmap(QRect(width() - qui.width()-8, 6, qui.width(), qui.height()),
    qui,
    QRect(0,0, qui.width(), qui.height()));

    p.drawPixmap(QRect(width() - qui.width() - max.width()-8, 6, max.width(), max.height()),
    max,
    QRect(0,0, max.width(), max.height()));

    p.drawPixmap(QRect(width() - qui.width()-max.width() - min.width()-8, 6, min.width(), min.height()),
    min,
    QRect(0,0, min.width(), min.height()));

    p.drawPixmap(QRect(width()-sr.width(), so.height(), sr.width(), height()-so.height()-su.height()), sr);


    p.drawPixmap(QRect(0, so.height(), sl.width(), height()-su.height()-so.height()), sl);

    p.setFont(titleFont);
    p.setPen(QPen(titleColor, 2));
    p.drawText(QRect(10, 0, width()-qui.width()*3-8-sl.width()-sr.width(), so.height()), Qt::AlignVCenter, windowTitle());
    QRect contentsRect(sl.width(), so.height(), width() - sr.width() - sl.width(), height() - so.height() - su.height());
    p.setBrush(backgroundColor);
    p.setPen(QPen(Qt::black, 1));
    p.drawRect(contentsRect);
    p.end();
    setMask(widgetMask.mask());
    resizeFrame = false;
}
void QSkinMainWindow::loadSkinIni()
{
    QSettings s(QApplication::applicationDirPath() + "/skin/skin.dat", QSettings::IniFormat);
    s.beginGroup("Skin");
    backgroundColor = QColor(s.value("Hintergrundfarbe", Qt::white).toString());
    titleColor = QColor(s.value("Titelfarbe", QColor(255,100,0)).toString());
    QFont font;
    font.fromString(s.value("Titelschrift", QFont("Comic Sans MS")).toString());
    skinPath = QApplication::applicationDirPath() + "/" + s.value("Skinpfad", QApplication::applicationDirPath() + "skin/blau/").toString();
    titleFont = font;
    s.endGroup();
}
void QSkinMainWindow::paintEvent(QPaintEvent *event)
{
    if(resizeFrame)
        updateStyle();
    QPainter p(this);
    p.drawPixmap(rect(), widgetMask);
    p.end();

}
