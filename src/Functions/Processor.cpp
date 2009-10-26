#include "StdAfx.h"
#include "Processor.h"

#include "Data.h"
#include "VssFunc.h"
#include "GitFunc.h"
#include "FileFunc.h"
#include "Config.h"

//#include "Tools/FileUtil.h"
//#include "Tools/FileUtilEx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static void Initialize()
{
	printf(">> create folder '%s'\n", paths::szWorkingDir);

	::CreateDirectory(paths::szTmpDir, NULL);
	::CreateDirectory(paths::szWorkingDir, NULL);
	printf("\n");

	vss::init_root_workfolder(paths::szWorkingDir);

	//printf(">> generate - 'history.txt' (for StdAfx.h)\n");
	//system(ss + " History $/Wood/MatrixKozijn/StdAfx.h >>..\\tmp\\history.txt");
}

static void CollectPaths(SDataVect &vect, LPCTSTR szInputFile, LPCTSTR szOutputFile)
{
	if (!file::StartJob(szOutputFile))
	{
		CStdioFile file;
		CFileException fe;
		if (!file.Open(szInputFile, CFile::modeRead | CFile::shareDenyWrite, &fe))
		{
			printf("file error\n");
			exit(1);
		
		}

		const DWORD dwFileLength = file.GetLength();

		CString sLine;
		CString sCurrentFolder;

		while (file.ReadString(sLine))
		{
			if (sLine.IsEmpty())
			{
				continue;
			}

			if (-1 != sLine.Find("$/"))
			{
				//FOLDER 'CHANGED'
				sCurrentFolder = sLine;

				while (':' != sCurrentFolder[sCurrentFolder.GetLength()-1])
				{
					if (!file.ReadString(sLine))
					{
						ASSERT(FALSE);
						return;
					}

					sCurrentFolder += sLine;
				}

				sCurrentFolder.Delete(sCurrentFolder.GetLength()-1); //delete ':'

				printf("\r>> %d%%", 100 * file.GetPosition() / dwFileLength);
			}
			else if (-1 != sLine.Find(".cpp") ||
					(-1 != sLine.Find(".h"))   )
			{
				//ADD FILE
				system(CString("ECHO ") + sCurrentFolder + "/" + sLine + " >> " + szOutputFile);
			}
		}

		file.Close();
		file::MarkJobDone(szOutputFile);
	}
};


void processor::Run()
{
	printf("\nINIT\n");
	Initialize();

	printf("\nSTEP1\n");
	vss::list_all_files(paths::szOutput1_Vss);

	printf("\nSTEP2\n");
	SDataVect vect;
	CollectPaths(vect, paths::szOutput1_Vss, paths::szOutput2_Paths);

}



