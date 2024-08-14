#include "GSkinMainWindow.h"
#include <QPainter>
#include <QtGui>
#include <QApplication>


GSkinMainWindow::GSkinMainWindow(QWidget *parent) : QMainWindow(parent, Qt::FramelessWindowHint)
{
    this->setStyleSheet(" \
                        QMainWindow::title { \
                        border-top-left-radius: 4px; \
                        border-top-right-radius: 4px; \
                        background-image: url(:/styles/styles/TitleBar/bg-header-white.png); \
                        height: 25px; \
                        padding: 0 0 0 20px; \
                        font-weight: bold; \
                        color: white; \
                        ");
}

// the parts of a titlebar

////////////////////////////////////////////////////////////////////////////////////////
//iconArea//----------------titleArea--------------//minButton//maxButton//closeButton//
////////////////////////////////////////////////////////////////////////////////////////

QRect minimizeButtonRect;
QRect maximizeButtonRect;
QRect closeButtonRect;
QRect titleAreaRec;
QRect iconAreaRec(0, 0, 0, 0); // leave blank for GarenaIcon
bool dragged( false );
QPoint dragPosition;

void GSkinMainWindow::paintEvent( QPaintEvent* event )
{
    QMainWindow::paintEvent(event);

    QStylePainter p(this);
    QStyle* style = this->style();
    QRect active_area = this->rect();
    int titlebar_height = 0;

    // Titlebar.
    QStyleOptionTitleBar t_opt;
    t_opt.initFrom(this);
    t_opt.subControls = QStyle::SC_TitleBarLabel
        | QStyle::SC_TitleBarSysMenu
        | QStyle::SC_TitleBarNormalButton
        | QStyle::SC_TitleBarContextHelpButton
        | QStyle::SC_TitleBarMinButton
        | QStyle::SC_TitleBarMaxButton
        | QStyle::SC_TitleBarCloseButton;

    titlebar_height = style->pixelMetric(QStyle::PM_TitleBarHeight, &t_opt, this);

    t_opt.rect = QRect(0, 0, this->width(), titlebar_height);
    t_opt.titleBarState = this->windowState();
    t_opt.text = t_opt.fontMetrics.elidedText(this->windowTitle(), Qt::ElideRight, t_opt.rect.width());
    t_opt.titleBarState = (int)Qt::WindowActive | ~Qt::WindowMinimized;
    style->drawComplexControl(QStyle::CC_TitleBar, &t_opt, &p, this);

    // app icon
    QIcon appIcon = QIcon(":/icons/icons/Common/server.ico");
    QRect iconR = QRect( 4, 0, 32, titlebar_height );
    iconAreaRec = QRect( 4, 0, 32, titlebar_height );
    p.drawItemPixmap( iconR, (int)Qt::AlignLeft | Qt::AlignVCenter, appIcon.pixmap( 32, 32 ) );

    // TODO: use another palette other than the default "t_opt.paletee" to make more customized app title
    // windows title
    titleAreaRec = QRect( 40, 0, rect().width() - 30 - 80, titlebar_height );
    style->drawItemText(&p, titleAreaRec, Qt::AlignLeft | Qt::AlignVCenter, 
        t_opt.palette, true, t_opt.text, QPalette::WindowText );

    // minimize button
    QIcon minIcon = style->standardIcon( QStyle::SP_TitleBarMinButton );
    QRect minR = QRect( 0, 0, rect().width() - 60, titlebar_height );
    minimizeButtonRect = QRect( rect().width() - 80, 0, 20, titlebar_height );
    p.drawItemPixmap( minR, (int)Qt::AlignRight | Qt::AlignVCenter, minIcon.pixmap( 19, 19 ) );

    // maximize button (or normal button), will be switched between these 2 status
    if (this->isMaximized())
    {
        QIcon maxIcon = style->standardIcon( QStyle::SP_TitleBarMaxButton );
        QRect maxR = QRect( 0, 0, rect().width() - 40, titlebar_height );
        maximizeButtonRect = QRect(rect().width() - 60, 0, 20, titlebar_height );
        p.drawItemPixmap( maxR, (int)Qt::AlignRight | Qt::AlignVCenter, maxIcon.pixmap( 19, 19 ) );
    }
    else
    {
        QIcon maxIcon = style->standardIcon( QStyle::SP_TitleBarNormalButton );
        QRect maxR = QRect( 0, 0, rect().width() - 40, titlebar_height );
        maximizeButtonRect = QRect(rect().width() - 60, 0, 20, titlebar_height );
        p.drawItemPixmap( maxR, (int)Qt::AlignRight | Qt::AlignVCenter, maxIcon.pixmap( 19, 19 ) );
    }

    // close button
    QIcon closeIcon = style->standardIcon( QStyle::SP_TitleBarCloseButton );
    QRect closeR = QRect( 0, 0, rect().width() - 20, titlebar_height );
    closeButtonRect = QRect( rect().width() - 40, 0, 20, titlebar_height );
    p.drawItemPixmap( closeR, (int)Qt::AlignRight | Qt::AlignVCenter, closeIcon.pixmap( 19, 19 ) );

    // Background widget.
    active_area.setTopLeft(QPoint(0, titlebar_height));
    this->setContentsMargins(0, titlebar_height, 0, 0);

    QStyleOption w_opt;
    w_opt.initFrom(this);
    w_opt.rect = active_area;
    style->drawPrimitive(QStyle::PE_Widget, &w_opt, &p, this);
}


//void GSkinMainWindow::resizeEvent( QResizeEvent * event )
//{
//}


bool GSkinMainWindow::isPointInMinButton( const QPoint& point )
{
    return minimizeButtonRect.contains( point );
}

bool GSkinMainWindow::isPointInMaxButton( const QPoint& point )
{
    return maximizeButtonRect.contains( point );
}

bool GSkinMainWindow::isPointInCloseButton( const QPoint& point )
{
    return closeButtonRect.contains( point );
}

bool GSkinMainWindow::isPointInIconArea( const QPoint& point )
{
    return iconAreaRec.contains( point );
}

bool GSkinMainWindow::isPointInTitleArea( const QPoint& point )
{
    return titleAreaRec.contains( point ) ;
}

void GSkinMainWindow::mousePressEvent( QMouseEvent* event )
{
    if( event->button() == Qt::LeftButton )
    {
        dragged = false;
        QPoint gPos = event->globalPos();
        if ( isPointInTitleArea( event->pos() ) )
        {
            dragged = true;
            dragPosition = gPos - frameGeometry().topLeft();
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        QPoint point = event->pos();
        if (isPointInTitleArea(point) ||
            isPointInMinButton(point) ||
            isPointInMaxButton(point) ||
            isPointInIconArea(point))
        {
            // TODO: pop up a system context menu to move/maximize/minimize/etc

            // after poping up the context menu, return directly
            // don't go to the default QWidget::mousePressEvent( event );
            event->ignore();
            return;
        }
        // when right-clicking on the close button, do nothing
        else if (isPointInCloseButton(point))
        {
            // don't go to the default QWidget::mousePressEvent( event );
            event->ignore();
            return;
        }
    }
    QWidget::mousePressEvent( event );
}

void GSkinMainWindow::mouseReleaseEvent( QMouseEvent* event )
{
    if (event->button() == Qt::RightButton)
    {
        QPoint point = event->pos();
        if (isPointInTitleArea(point) ||
            isPointInMinButton(point) ||
            isPointInMaxButton(point) ||
            isPointInIconArea(point))
        {
            QMenu *mainMenu = new QMenu;
            QAction* appTitleAction = mainMenu->addAction(this->windowTitle(), this, SLOT(doNothing()));
            appTitleAction->setEnabled(false);
            mainMenu->addSeparator();

            QAction* sizeAction = mainMenu->addAction(tr("Size"), this, SLOT(doNothing()));
            sizeAction->setEnabled(false);
            mainMenu->addSeparator();

            //mainMenu->addAction(tr("Close"), this, SLOT(close()));
            //mainMenu->addSeparator();
            mainMenu->addAction(tr("maxmize"), this, SLOT(doNothing()));
            mainMenu->addAction(tr("minimize"), this, SLOT(doNothing()));
            mainMenu->exec(event->globalPos());
        }
        return;
    }

    if( !dragged && event->button() == Qt::LeftButton )
    {
        const QPoint& clickPoint = event->pos();
        if (isPointInMinButton(event->pos()))
        {
            dragged = false;
            this->showMinimized();
        }
        else if (isPointInMaxButton(event->pos()))
        {
            dragged = false;
            if (this->isMaximized())
            {
                this->showNormal();
            }
            else
            {
                this->showMaximized();
            }
        }
        else if( isPointInCloseButton( event->pos() ) )
        {
            close();
        }
        else if( isPointInTitleArea(event->pos()) )
        {
            dragPosition = event->pos();
        }
        else if (isPointInIconArea(event->pos()))
        {
            dragged = false;
            // pop up a context menu showing the system menus
            QMessageBox::information(this, tr("msg"), tr("Not implemented"));
        }
    }

    QWidget::mouseReleaseEvent( event );
}

void GSkinMainWindow::mouseMoveEvent( QMouseEvent* event )
{
    QWidget::mouseMoveEvent( event );

    // doesn't allow to move window when maxmized
    if (!this->isMaximized())
    {
        if ( dragged && event->buttons() & Qt::LeftButton )
        {
            move( event->globalPos() - dragPosition );
            event->accept();
        }
    }
}
