#ifndef UIFULLSCREENDIALOG2_H
#define UIFULLSCREENDIALOG2_H

#include <QDialog>
#include <ActiveQt/QAxWidget>

class QAxWidget;
class QVBoxLayout;
class uiFullScreenDialog2
{
public:
    explicit uiFullScreenDialog2() 
        : m_dialog(NULL), m_mainLayout(NULL), m_flashWidget(NULL)
    {
    }

public:
    void setupUi(QDialog* dialog);

    void retranslateUi();

    void initFlashWidget();

    void remakeFlashWidget();
private:
    void _makeNewFlashWidget();

public:
    QDialog* m_dialog;
    QVBoxLayout* m_mainLayout;
    QAxWidget* m_flashWidget;
};

#endif // UIFULLSCREENDIALOG2_H
