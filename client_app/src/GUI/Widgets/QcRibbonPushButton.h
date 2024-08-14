#ifndef QC_RIBBON_PUSH_BUTTON_H
#define QC_RIBBON_PUSH_BUTTON_H

#include <QPushButton>
#include <QSize>

enum RibBtnAdvMode
{
    RibBtnMode_NA = -1,
    RibBtnMode_Normal = 1,
    RibBtnMode_Context = 2,
};

class QLabel;
class QSpacerItem;


class QcRibbonPushButton : public QPushButton
{
public:
    QcRibbonPushButton(
        QWidget* parent = NULL);

    void init(const QString &mainIconPath,
        const QString &text,
        RibBtnAdvMode mode = RibBtnMode_Normal);

    void setAdvMode(RibBtnAdvMode mode = RibBtnMode_Normal);
    void setBtmText(const QString &text);
    void setMainIcon(const QString &mainIconPath);

public:
    virtual QSize minimumSizeHint () const;
    virtual QSize sizeHint () const;

private:
    void _layout();

private:
    RibBtnAdvMode m_advMode;
    QLabel* m_mainIconLabel;
    QLabel* m_textLabel;
    // if the button can trigger a context menu, then this will be set
    QLabel* m_advIconLabel;

    QSpacerItem *m_btmLeftSpacer;
    QSpacerItem *m_btmRightSpacer;

private: // helper
    QString m_mainIconPath;
    QString m_text;
};

#endif // QC_RIBBON_PUSH_BUTTON_H
