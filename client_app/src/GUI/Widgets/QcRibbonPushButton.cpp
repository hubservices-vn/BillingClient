#include "QcRibbonPushButton.h"
#include <QtGui>

QcRibbonPushButton::QcRibbonPushButton(QWidget* parent) 
: QPushButton(parent)
{
    m_advMode = RibBtnMode_Normal;

    m_mainIconLabel = NULL;
    m_textLabel = NULL;
    m_advIconLabel = NULL;
    m_btmLeftSpacer = NULL;
    m_btmRightSpacer = NULL;
}

QSize QcRibbonPushButton::minimumSizeHint() const
{
    return QSize(68, 60);
}

QSize QcRibbonPushButton::sizeHint() const
{
    return QSize(68, 60);
}

void QcRibbonPushButton::init
(const QString &mainIconPath, 
 const QString &text, 
 RibBtnAdvMode mode /* = RibBtnMode_Normal*/ )
{
    setMainIcon(mainIconPath);
    setBtmText(text);
    setAdvMode(mode);

    _layout();
}

void QcRibbonPushButton::setMainIcon( const QString &mainIconPath )
{
    if (m_mainIconLabel == NULL)
    {
        m_mainIconLabel = new QLabel(this);
        m_mainIconLabel->setAlignment(Qt::AlignCenter);
    }

    m_mainIconPath = mainIconPath;
    m_mainIconLabel->setPixmap(QPixmap(mainIconPath));
}

void QcRibbonPushButton::setBtmText( const QString &text )
{
    if (m_textLabel == NULL)
    {
        m_textLabel = new QLabel(this);
        m_textLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    }

    m_text = text;
    m_textLabel->setText(m_text);
}

void QcRibbonPushButton::setAdvMode( RibBtnAdvMode mode /* = RibBtnMode_Normal*/ )
{
    m_advMode = mode;

    if (mode == RibBtnMode_Normal)
    {
        if (m_advIconLabel != NULL)
        {
            m_advIconLabel->deleteLater();
            m_advIconLabel = NULL;
        }
        if (m_btmLeftSpacer != NULL)
        {
            delete m_btmLeftSpacer;
            m_btmLeftSpacer = NULL;
        }
        if (m_btmRightSpacer != NULL)
        {
            delete m_btmRightSpacer;
            m_btmRightSpacer = NULL;
        }
    }
    // create the ">" indicator icon
    else if (mode == RibBtnMode_Context)
    {
        if (m_advIconLabel == NULL)
        {
            m_advIconLabel = new QLabel(this);
        }
        
        m_advIconLabel->setPixmap(QPixmap(":/styles/styles/RibbonButton/ribbon-menu-indicator.png"));
        m_advIconLabel->setAlignment(Qt::AlignCenter);

        if (m_btmLeftSpacer == NULL)
        {
            m_btmLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        }
        
        if (m_btmRightSpacer == NULL)
        {
            m_btmRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
        }
    }
}

void QcRibbonPushButton::_layout()
{
    QVBoxLayout *mainVLayout = new QVBoxLayout();
    mainVLayout->setMargin(0);
    mainVLayout->setSpacing(0);

    mainVLayout->addWidget(m_mainIconLabel);

    QString text = m_textLabel->text();
    if (m_advIconLabel == NULL)
    {
        mainVLayout->addWidget(m_textLabel);
    }
    else
    {
        // add bottom: the text (and optional adv icon)
        QHBoxLayout* btmHLayout = new QHBoxLayout();
        btmHLayout->setMargin(0);
        btmHLayout->setSpacing(0);
        {
            btmHLayout->addItem(m_btmLeftSpacer);

            btmHLayout->addWidget(m_textLabel);
            btmHLayout->addWidget(m_advIconLabel);

            btmHLayout->addItem(m_btmRightSpacer);
        }

        mainVLayout->addLayout(btmHLayout);
    }

    mainVLayout->setStretch(0, 3);
    mainVLayout->setStretch(1, 1);

    this->setLayout(mainVLayout);
}
