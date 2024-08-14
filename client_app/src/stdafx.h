#pragma once


#pragma warning(push)
#pragma warning(disable: 4482) // enum-with-prefix warning
#pragma warning(disable:4805) // comparison operations between bool and int
#pragma warning(disable:4996) // a function that was marked with deprecated
#pragma warning(disable: 4189) // 'identifier' : local variable is initialized but not referenced

//#include <stdint.h>
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;

// c library header files
#include <math.h>
#include <vector>
#include <map>

// QT related header files
// general
#include <QApplication>
#include <QObject>
#include <QDateTime>
#include <QRegExp>
#include <QScopedPointer>
#include <QSharedPointer>

// os / platform
#include <QProcess>
#include <QSettings>

// container
#include <QStringList>
#include <QList>
#include <QMap>

// regexp
#include <QRegExp>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExpValidator>

// UI
#include <QtGui>
#include <QDesktopWidget>
#include <QMessageBox>

// thread or msgBox
#include <QTimer>
#include <QThread>

// IO
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QIODevice>
#include <QBuffer>

// network
#include <QTcpSocket>

// logger
#include "log.h"

// utils
#include "DateTimeUtils.h"
#include "Utils.h"

// qt types
#include "QtTypePtrs.h"