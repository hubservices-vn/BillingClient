//#include "FileProtector.h"
//#include "Utils.h"
//
//namespace
//{
//    CString QStringToCString( const QString &str )
//    {
//#ifdef _UNICODE
//        return CString(str.utf16());
//#else
//        return CString(str.toAscii());
//#endif
//    }
//}
//
//
//
//FileProtector::FileProtector()
//{
//}
//
//FileProtector::~FileProtector()
//{
//    if (! m_protectList.isEmpty())
//    {
//        unprotect();
//    }
//}
//
//bool FileProtector::protect(const QStringList &appPathList)
//{
//    foreach (const QString &appPath, appPathList)
//    {
//        CFile* source = new CFile();
//        CString appPathCStr = QStringToCString(appPath);
//        source->Open(appPathCStr, CFile::modeRead | CFile::shareExclusive);
//        m_protectList.append(source);
//    }
//
//    return true;
//}
//
//bool FileProtector::unprotect()
//{
//    foreach (CFile* cfile, m_protectList)
//    {
//        if (cfile == NULL)
//        {
//            continue;
//        }
//
//        cfile->Close();
//    }
//
//    qDeleteAll(m_protectList);
//    return true;
//}
