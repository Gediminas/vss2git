#include "StdAfx.h"
#include "Processor.h"

#include "Data.h"
#include "VssFunc.h"
#include "GitFunc.h"
#include "FileFunc.h"

//#include "Tools/FileUtil.h"
//#include "Tools/FileUtilEx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const LPTSTR s_sz_list_all_files = "..\\tmp\\all_files.txt";

static void Initialize()
{
	printf(">> create folder '..\\tmp\\Working'\n");
	::CreateDirectory("..\\tmp", NULL);
	::CreateDirectory("..\\tmp\\Working", NULL);
	printf("\n");

	vss::init_root_workfolder("..\\tmp\\Working");

	//printf(">> generate - 'history.txt' (for StdAfx.h)\n");
	//system(ss + " History $/Wood/MatrixKozijn/StdAfx.h >>..\\tmp\\history.txt");
}

static void Collect(SDataVect &vect)
{

	vss::list_all_files(s_sz_list_all_files);

	CStdioFile file;
	CFileException fe;
	if (!file.Open(s_sz_list_all_files, CFile::modeRead, &fe))
	{
		printf("file error\n");
		exit(1);
	
	}

	CString sLogFile = "..\\tmp\\tree.log";
	::DeleteFile(sLogFile);

	CString sLine;
	while (file.ReadString(sLine))
	{
		if (sLine.IsEmpty())
		{
			continue;
		}

		if (-1 != sLine.Find("$/"))
		{
			system(CString("ECHO FOLDER ") + sLine +" >> " + sLogFile);
		}
		else if (-1 != sLine.Find(".cpp") ||
				(-1 != sLine.Find(".h"))   )
		{
			system(CString("ECHO ADD ") + sLine +" >> " + sLogFile);
		}
		else
		{
			system(CString("ECHO skip ") + sLine +" >> " + sLogFile);
		}
	}

	file.Close();
};


void processor::Run()
{
	Initialize();

	SDataVect vect;
	Collect(vect);

}



