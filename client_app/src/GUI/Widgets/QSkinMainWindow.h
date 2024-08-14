/********************************************************************************************************
 * PROGRAM      : childform
 * DATE - TIME  : Samstag 30 Dezember 2006 - 12h04
 * AUTHOR       :  (  )
 * FILENAME     : QSkinMainWindow.h
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef QSkinMainWindow_H
#define QSkinMainWindow_H
#include "qwidgetresizehandler_p.h"
#include <QtGui>

class QSkinMainWindow;
class QSkinMainWindowResizeHandler : public QWidgetResizeHandler
{
Q_OBJECT
public:
	QSkinMainWindowResizeHandler(QSkinMainWindow *parent, QWidget *cw =0);
protected:
	bool eventFilter(QObject *o, QEvent *e);
private:
	QSkinMainWindow *skinWidget;
};
class  QSkinMainWindow : public QMainWindow
{
    Q_OBJECT
    friend class QSkinMainWindowResizeHandler;
public:
    QSkinMainWindow(QWidget* wgtParent = 0);
	~QSkinMainWindow(){}
	void setSkinPath(const QString & skinpath);
	QString getSkinPath();
public slots:
	void updateStyle();
signals:
	void aboutToClose();
protected:
	void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *e);
	void closeEvent(QCloseEvent *e);
	void loadSkinIni();
	void manageRegions();
	void getSelectedFrame(QPoint p);
private:
	bool resizeFrame;
    QPoint dragPosition;
	QPixmap widgetMask;//the pixmap, in which the ready frame is stored on pressed? 
	QString skinPath;
	double alpha;
	QFont titleFont;
	QColor titleColor;
	QColor backgroundColor;
	// Resize implementation
	enum edgePosition {
		None,
		Left,
		TopLeft,
		Top,
		TopRight,
		Right,
		BottomRight,
		Bottom,
		BottomLeft
	};
	
	QPoint p;
    QRect r;
    int d;
    int dxMax;
    int dyMax;
    bool gotMousePress;	
	int resizePosition;
	QRegion leftFrame;
	QRegion topFrame;
	QRegion rightFrame;
	QRegion bottomFrame;
	QRegion topLeftFrame;
	QRegion bottomLeftFrame;
	QRegion topRightFrame;
	QRegion bottomRightFrame;
	QSkinMainWindowResizeHandler * resizeHandler;
};
#endif

