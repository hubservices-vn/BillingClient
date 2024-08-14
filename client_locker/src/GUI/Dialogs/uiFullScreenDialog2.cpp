#include "uiFullScreenDialog2.h"
#include <Qtgui>
#include <QtWebKit/QWebView>

void uiFullScreenDialog2::setupUi(QDialog *dialog)
{
    m_dialog = dialog;

#ifdef NDEBUG
    //Qt::WindowFlags flags = m_dialog->windowFlags();
    m_dialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
#endif
}

void uiFullScreenDialog2::retranslateUi()
{
    // nothing to translate
}

void uiFullScreenDialog2::initFlashWidget()
{
    _makeNewFlashWidget();
}

void uiFullScreenDialog2::remakeFlashWidget()
{
    if (m_mainLayout != NULL)
    {
        delete m_mainLayout;
        m_mainLayout = NULL;
    }

    if (m_flashWidget != NULL)
    {
        QTimer::singleShot(2000, m_flashWidget, SLOT(deleteLater));
    }

    _makeNewFlashWidget();
}

void uiFullScreenDialog2::_makeNewFlashWidget()
{
    m_mainLayout = new QVBoxLayout();
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

    m_flashWidget = new QAxWidget();
    m_flashWidget->setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}"));

    m_mainLayout->addWidget(m_flashWidget);

    m_dialog->setLayout(m_mainLayout);
}
