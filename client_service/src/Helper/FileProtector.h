//#ifndef FILEPROTECTOR_H
//#define FILEPROTECTOR_H
//
//#include <QObject>
//#include <QStringList>
//
//
//class CFile;
//class FileProtector : public QObject, public CcSingleton<FileProtector>
//{
//    Q_OBJECT
//public:
//    FileProtector();
//
//    ~FileProtector();
//
//public:
//    // protect the apps by opening them in "sharedExclusive" mode
//    bool protect(const QStringList& appPathList);
//
//    bool unprotect();
//private:
//    QList<CFile*> m_protectList;
//};
//
//#endif // FILEPROTECTOR_H
