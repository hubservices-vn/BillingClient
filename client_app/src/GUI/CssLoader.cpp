#include "CssLoader.h"
#include "TxtLoader.h"

#include <QDir>
#include <QCoreApplication>
#include "log.h"

QString CssLoader::loadAllCssFiles()
{
    QString result;

    // in debug env, we load the css from disk directly (so if the css changed, there's no need to recompile the rcc file)
#ifndef NDEBUG
    {
        QString appFolder = QCoreApplication::applicationDirPath();
        result += TxtLoader::load(appFolder + "/css/Dialog.css");

        result += TxtLoader::load(appFolder + "/css/CheckBox.css");
        result += TxtLoader::load(appFolder + "/css/ComboBox.css");
        result += TxtLoader::load(appFolder + "/css/DateEdit.css");
        result += TxtLoader::load(appFolder + "/css/GroupBox.css");
        result += TxtLoader::load(appFolder + "/css/LinetEdit.css");
        result += TxtLoader::load(appFolder + "/css/MenuBar.css");
        result += TxtLoader::load(appFolder + "/css/PushButton.css");
        result += TxtLoader::load(appFolder + "/css/QWidget.css");
        result += TxtLoader::load(appFolder + "/css/RadioButton.css");
        result += TxtLoader::load(appFolder + "/css/RibbonButton.css");
        result += TxtLoader::load(appFolder + "/css/ScrollBar.css");
        result += TxtLoader::load(appFolder + "/css/SpinBox.css");
        result += TxtLoader::load(appFolder + "/css/StatusBar.css");
        result += TxtLoader::load(appFolder + "/css/TableWidget.css");
        result += TxtLoader::load(appFolder + "/css/TabWidget.css");
        result += TxtLoader::load(appFolder + "/css/TimeEdit.css");
        result += TxtLoader::load(appFolder + "/css/TitleBar-MainWindow.css");
        result += TxtLoader::load(appFolder + "/css/Toolbar.css");
        result += TxtLoader::load(appFolder + "/css/ToolButton.css");
    }
#else
    {
        result += TxtLoader::load(":/css/css/Dialog.css");

        result += TxtLoader::load(":/css/css/CheckBox.css");
        result += TxtLoader::load(":/css/css/ComboBox.css");
        result += TxtLoader::load(":/css/css/DateEdit.css");
        result += TxtLoader::load(":/css/css/GroupBox.css");
        result += TxtLoader::load(":/css/css/LineEdit.css");
        result += TxtLoader::load(":/css/css/MenuBar.css");
        result += TxtLoader::load(":/css/css/PushButton.css");
        result += TxtLoader::load(":/css/css/QWidget.css");
        result += TxtLoader::load(":/css/css/RadioButton.css");
        result += TxtLoader::load(":/css/css/RibbonButton.css");
        result += TxtLoader::load(":/css/css/ScrollBar.css");
        result += TxtLoader::load(":/css/css/SpinBox.css");
        result += TxtLoader::load(":/css/css/StatusBar.css");
        result += TxtLoader::load(":/css/css/TableWidget.css");
        result += TxtLoader::load(":/css/css/TabWidget.css");
        result += TxtLoader::load(":/css/css/TimeEdit.css");
        result += TxtLoader::load(":/css/css/TitleBar-MainWindow.css");
        result += TxtLoader::load(":/css/css/Toolbar.css");
        result += TxtLoader::load(":/css/css/ToolButton.css");
    }
#endif

    return result;
}
