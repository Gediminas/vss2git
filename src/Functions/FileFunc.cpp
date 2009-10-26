#include "StdAfx.h"
#include "FileFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//static CString GetBasePath()
//{
//	CString sPath;
//	::GetCurrentDirectory(1000, sPath.GetBufferSetLength(1000));
//	return sPath;
//}

bool file::DoesFileExist(LPTSTR szFilePath)
{
	CFileFind ff;
	return TRUE == ff.FindFile(szFilePath);
}




