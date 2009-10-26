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

static void AddFile(LPCTSTR szFilePath, CStdioFile &output_file)
{
	vss::list_file_versions(szFilePath, paths::szDump);

	CStdioFile fileDump;
	CFileException fe;

	if (!fileDump.Open(paths::szDump, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		printf(">> dump file error\n");
		getchar();
		exit(1);
	}

	CString sLine, sToken;
	int nToken;
	int nVersion = INT_MAX;

	while (fileDump.ReadString(sLine))
	{
		if (sLine.IsEmpty())
		{
			continue;
		}

		
		if (0 == sLine.Find("*****************  Version "))
		{
			sToken = sLine.Mid(27, 4);
			sToken.TrimRight();

			nToken = atoi(sToken);

			ASSERT(0 < nToken);
			ASSERT(nToken < nVersion);

			nVersion = nToken;

			output_file.WriteString("**********\n");

			output_file.WriteString("FILE\n");
			output_file.WriteString(szFilePath);
			output_file.WriteString("\n");
			
			output_file.WriteString("VERSION\n");
			output_file.WriteString(sToken);
			output_file.WriteString("\n");
		}
		else if (nVersion < INT_MAX)
		{
			if (0 == sLine.Find("User: "))
			{
				//User
				sToken = sLine.Mid(6, 13);
				sToken.TrimRight();

				output_file.WriteString("USER\n");
				output_file.WriteString(sToken);
				output_file.WriteString("\n");

				//Time
				sToken = sLine.Mid(25, 9);
				sToken.TrimLeft();
				sToken.TrimRight();
				sToken.Replace('.', '-');
				ASSERT('-' == sToken[2]);
				ASSERT('-' == sToken[5]);

				if ('9' == sToken[0])
				{
					ASSERT('8' == sToken[1] || '9' == sToken[1]);// =1998 || 1999
					sToken = "19" + sToken;
				}
				else
				{
					ASSERT('0' == sToken[0]);// =200x
					sToken = "20" + sToken;
				}

				output_file.WriteString("TIME\n");
				output_file.WriteString(sToken);
				output_file.WriteString(" ");

				sToken = sLine.Mid(43, 5);
				sToken.TrimRight();
				if (4 == sToken.GetLength())
				{
					sToken = "0" + sToken;
				}

				ASSERT(5 == sToken.GetLength());
				ASSERT(':' == sToken[2]);

				output_file.WriteString(sToken);
				output_file.WriteString("\n");
			}
			else if ("Branched" == sLine)
			{
				output_file.WriteString("BRANCHED\n");
			}
			else if ("Created" == sLine)
			{
				ASSERT(1 == nVersion);
				output_file.WriteString("CREATED\n");
			}
			else if (0 == sLine.Find("Comment: "))
			{
				if (9 < sLine.GetLength())
				{
					sToken = sLine.Right(sLine.GetLength() - 9);

					output_file.WriteString("COMMENT\n");
					output_file.WriteString(sToken);
					output_file.WriteString("\n");
				}
			}
			else if (0 == sLine.Find("Checked in $/"))
			{
				output_file.WriteString("COMMIT\n");
			}
			else if (0 == sLine.Find("Labeled"))
			{
				output_file.WriteString("TAG\n");
			}
			else
			{
				output_file.WriteString("UNKNOWN\n");
				output_file.WriteString(sLine);
				output_file.WriteString("\n");
			}
				
		}

	}

	output_file.Flush();
	fileDump.Close();
}

static void Step1_VssPaths(LPCTSTR szOutputFile)
{
	printf("\nSTEP1\n");
	vss::list_all_files(szOutputFile);
}

static void Step2_CollectPaths(LPCTSTR szInputFile, LPCTSTR szOutputFile)
{
	printf("\nSTEP2\n");

	if (!file::StartJob(szOutputFile))
	{
		CStdioFile fileI, fileO;
		CFileException fe;

		if (!fileI.Open(szInputFile, CFile::modeRead | CFile::shareDenyWrite, &fe))
		{
			printf(">> input file error\n");
			getchar();
			exit(1);
		}

		if (!fileO.Open(szOutputFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &fe))
		{
			printf(">> output file error\n");
			exit(1);
		
		}

		const DWORD dwFileLength = fileI.GetLength();

		CString sLine;
		CString sCurrentFolder;

		while (fileI.ReadString(sLine))
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
					if (!fileI.ReadString(sLine))
					{
						ASSERT(FALSE);
						return;
					}

					sCurrentFolder += sLine;
				}

				sCurrentFolder.Delete(sCurrentFolder.GetLength()-1); //delete ':'

				//fileO.Flush();
				printf("\r>> %d%%", 100 * fileI.GetPosition() / dwFileLength);
			}
			else if (-1 != sLine.Find(".cpp") ||
					(-1 != sLine.Find(".h"))   )
			{
				AddFile(sCurrentFolder + "/" + sLine, fileO);
			}
		}

		fileI.Close();
		fileO.Close();
		file::MarkJobDone(szOutputFile);
	}
};

static void Step3_CollectAllInfo(LPCTSTR szInputFile, LPCTSTR szOutputFile)
{
	printf("\nSTEP3\n");
//	if (!file::StartJob(szOutputFile))
//	{
//		CStdioFile file;
//		CFileException fe;
//		if (!file.Open(szInputFile, CFile::modeRead | CFile::shareDenyWrite, &fe))
//		{
//			printf("file error\n");
//			exit(1);
//		
//		}
//
//		const DWORD dwFileLength = file.GetLength();
//
//		CString sLine;
//		CString sCurrentFolder;
//
//		while (file.ReadString(sLine))
//		{
//			if (sLine.IsEmpty())
//			{
//				continue;
//			}
//
//			if (-1 != sLine.Find("$/"))
//			{
//				//FOLDER 'CHANGED'
//				sCurrentFolder = sLine;
//
//				while (':' != sCurrentFolder[sCurrentFolder.GetLength()-1])
//				{
//					if (!file.ReadString(sLine))
//					{
//						ASSERT(FALSE);
//						return;
//					}
//
//					sCurrentFolder += sLine;
//				}
//
//				sCurrentFolder.Delete(sCurrentFolder.GetLength()-1); //delete ':'
//
//				printf("\r>> %d%%", 100 * file.GetPosition() / dwFileLength);
//			}
//			else if (-1 != sLine.Find(".cpp") ||
//					(-1 != sLine.Find(".h"))   )
//			{
//				//ADD FILE
//				system(CString("ECHO ") + sCurrentFolder + "/" + sLine + " >> " + szOutputFile);
//			}
//		}
//
//		file.Close();
//		file::MarkJobDone(szOutputFile);
//	}
};


void processor::Run()
{
	printf("\nINIT\n");
	Initialize();

	Step1_VssPaths      (paths::szStep1_VssDir);
	Step2_CollectPaths  (paths::szStep1_VssDir, paths::szStep2_Paths);
	Step3_CollectAllInfo(paths::szStep2_Paths,  paths::szStep3_Info);

	//SDataVect vect;
}



