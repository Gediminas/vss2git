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



static void AddFileVersions(LPCTSTR szFilePath, CStdioFile &output_file)
{
	CString sFilePath = szFilePath;

	if (-1 != sFilePath.Find("MatrixKozijnTools"))
	{
		ASSERT(FALSE);
		sFilePath.Replace("MatrixKozijnTools", "MatrixKozijn Tools");
	}

	vss::list_file_versions(sFilePath, paths::szDump);

	CStdioFile fileDump;
	CFileException fe;

	if (!fileDump.Open(paths::szDump, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		printf(">> dump file error\n");
		getchar();
		exit(1);
	}

	CString sLine, sToken;
	int nToken, nPos;
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
			output_file.WriteString(sFilePath);
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
				nPos = sLine.Find("Date: ") + 6;
				sToken = sLine.Mid(nPos, 8);
				//sToken.TrimLeft();
				//sToken.TrimRight();
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

				nPos = sLine.Find("Time: ") + 6;
				sToken = sLine.Mid(nPos, 5);
				sToken.TrimLeft();
				if (4 == sToken.GetLength())
				{
					sToken = "0" + sToken;
				}

				ASSERT(5 == sToken.GetLength());
				ASSERT(':' == sToken[2]);

				output_file.WriteString(sToken);
				output_file.WriteString("\n");
			}
			else if (0 == sLine.Find("Label: "))
			{
				sToken = sLine.Mid(8, sLine.GetLength() - 9);
				output_file.WriteString("LABEL\n");
				output_file.WriteString(sToken);
				output_file.WriteString("\n");
			}
			else if (0 == sLine.Find("Label comment: "))
			{
				if (15 < sLine.GetLength())
				{
					sToken = sLine.Right(sLine.GetLength() - 15);
					output_file.WriteString("LABEL_COMMENT\n");
					output_file.WriteString(sToken);
					output_file.WriteString("\n");
				}
			}
			else if ("Branched" == sLine)
			{
				output_file.WriteString("ACTION_BRANCHED\n");
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
			else if ("Created" == sLine)
			{
				ASSERT(1 == nVersion);
				output_file.WriteString("ACTION_CREATED\n");
			}
			else if (0 == sLine.Find("Checked in $/"))
			{
				output_file.WriteString("ACTION_COMMITED\n");
			}
			else if (0 == sLine.Find("Labeled"))
			{
				output_file.WriteString("ACTION_LABELED\n");
			}
			else
			{
				output_file.WriteString("TRASH\n");
				output_file.WriteString(sLine);
				output_file.WriteString("\n");
			}
				
		}

	}

	//output_file.Flush();
	fileDump.Close();
}

static void AddSkippedFile(LPCTSTR szFilePath, CStdioFile &output_file)
{
	output_file.WriteString(szFilePath);
	output_file.WriteString("\n");
}

inline bool CheckExt(const CString &sLine, LPCTSTR szExt)
{
	return ((sLine.GetLength() - strlen(szExt)) == sLine.Find(szExt));

}



static void Initialize()
{
	printf("\nINIT");
	printf("\n>> ");
	system("TIME/T");

	printf(">> create folder '%s'\n", paths::szWorkingDir);

	::CreateDirectory(paths::szTmpDir, NULL);
	::CreateDirectory(paths::szWorkingDir, NULL);
	printf("\n");

	vss::init_root_workfolder(paths::szWorkingDir);

	//printf(">> generate - 'history.txt' (for StdAfx.h)\n");
	//system(ss + " History $/Wood/MatrixKozijn/StdAfx.h >>..\\tmp\\history.txt");
}

static void Step1_VssPaths(LPCTSTR szOutputFile)
{
	printf("\nSTEP1");
	printf("\n>> ");
	system("TIME/T");
	vss::list_all_files(szOutputFile);
}

static void Step2_CollectInfo(LPCTSTR szInputFile, LPCTSTR szOutputFile, LPCTSTR szOutputSkippedFile)
{
	printf("\nSTEP2");
	printf("\n>> ");
	system("TIME/T");

	if (!file::StartJob(szOutputFile))
	{
		::DeleteFile(szOutputSkippedFile);

		CStdioFile fileInput, fileOutput, fileSkipped;
		CFileException fe;

		if (!fileInput.Open(szInputFile, CFile::modeRead | CFile::shareDenyWrite, &fe))
		{
			printf(">> input file error\n");
			getchar();
			exit(1);
		}

		if (!fileOutput.Open(szOutputFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &fe))
		{
			printf(">> output file error\n");
			getchar();
			exit(1);
		
		}

		if (!fileSkipped.Open(szOutputSkippedFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &fe))
		{
			printf(">> output 'skipped' file error\n");
			getchar();
			exit(1);
		
		}

		const DWORD dwFileLength = fileInput.GetLength();
		
		CString sClearText(' ', 70);
		sClearText = "\r" + sClearText;

		CString sLine, sCurrentFolder, sLogDir;

		while (fileInput.ReadString(sLine))
		{
			if (sLine.IsEmpty())
			{
				continue;
			}

			if (0 == sLine.Find("$/"))
			{
				//FOLDER 'CHANGED'
				sCurrentFolder = sLine;

				while (':' != sCurrentFolder[sCurrentFolder.GetLength()-1] && fileInput.ReadString(sLine))
				{
					ASSERT(-1 == sLine.Find("No items found under $/"));

					if (sCurrentFolder.GetLength() < 79)
					{
						sCurrentFolder += " ";
					}

					sCurrentFolder += sLine;
				}

				ASSERT(':' == sCurrentFolder[sCurrentFolder.GetLength()-1]);
				sCurrentFolder.Delete(sCurrentFolder.GetLength()-1); //delete ':'

				fileOutput.Flush();
				fileSkipped.Flush();

				sLogDir.Format("...%s", sCurrentFolder.Right(min(sCurrentFolder.GetLength(), 50)));

				printf(sClearText);
				printf("\r>> %.1f%% (%s)", 100.0 * fileInput.GetPosition() / dwFileLength, sLogDir);
			}
			else if ('$' != sLine[0])
			{
				if (CheckExt(sLine, ".h")   ||
					CheckExt(sLine, ".cpp") ||
					CheckExt(sLine, ".c")   ||
					CheckExt(sLine, ".hpp")  )
				{
					ASSERT(-1 == sLine.Find("No items found under $/"));
					AddFileVersions(sCurrentFolder + "/" + sLine, fileOutput);
				}
				else
				{
					if (-1 != sLine.Find("No items found under $/"))
					{
						sCurrentFolder.Empty();
					}
					else
					{
						AddSkippedFile(sCurrentFolder + "/" + sLine, fileSkipped);
					}
				}
			}
		}

		fileInput.Close();
		fileOutput.Close();
		fileSkipped.Close();
		::DeleteFile(paths::szDump);
		file::MarkJobDone(szOutputFile);
	}
};

static void Step3_GroupInfo(LPCTSTR szInputFile, LPCTSTR szOutputFile)
{
	printf("\nSTEP3");
	printf("\n>> ");
	system("TIME/T");

	if (!file::StartJob(szOutputFile))
	{
		CStdioFile fileInput, fileOutput, fileSkipped;
		CFileException fe;

		if (!fileInput.Open(szInputFile, CFile::modeRead | CFile::shareDenyWrite, &fe))
		{
			printf(">> input file error\n");
			getchar();
			exit(1);
		}

		if (!fileOutput.Open(szOutputFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &fe))
		{
			printf(">> output file error\n");
			getchar();
			exit(1);
		
		}

		const DWORD dwFileLength = fileInput.GetLength();

		CString sLine;
//		SData *pData = NULL;

		while (fileInput.ReadString(sLine))
		{
			if (sLine.IsEmpty())
			{
				continue;
			}

			if ("**********" == sLine)
			{
				//ASSERT(NULL == pData); //is asserts, previous was not processed
				//pData = new SData;

				fileOutput.Flush();

				printf("\r>> %d%%", 100 * fileInput.GetPosition() / dwFileLength);
			}
			else if ("FILE" == sLine)
			{
				fileOutput.WriteString("\n");
				fileOutput.WriteString("F:");
				fileInput.ReadString(sLine);
				fileOutput.WriteString(sLine + "\n");
			}
			else if ("VERSION" == sLine)
			{
				fileOutput.WriteString("V:");
				fileInput.ReadString(sLine);
				fileOutput.WriteString(sLine + "\n");
			}
			else if ("USER" == sLine)
			{
				fileOutput.WriteString("U:");
				fileInput.ReadString(sLine);
				fileOutput.WriteString(sLine + "\n");
			}
			else if ("TIME" == sLine)
			{
				fileOutput.WriteString("T:");
				fileInput.ReadString(sLine);
				fileOutput.WriteString(sLine + "\n");
			}
			else if ("TAG" == sLine)
			{
				fileOutput.WriteString("TAG:");
				fileInput.ReadString(sLine);
				fileOutput.WriteString(sLine + "\n");
			}
			else if ("COMMENT" == sLine)
			{
				fileOutput.WriteString("C:");
				fileInput.ReadString(sLine);
				fileOutput.WriteString(sLine + "\n");
			}
			else if ("COMMIT" == sLine)
			{
				//fileOutput.WriteString("F:");
				//fileInput.ReadString(sLine);
				//fileOutput.WriteString(sLine + "\n");
			}
			else if ("CREATED" == sLine)
			{
				//fileOutput.WriteString("F:");
				//fileInput.ReadString(sLine);
				//fileOutput.WriteString(sLine + "\n");
			}
			else if ("BRANCHED" == sLine)
			{
				//fileOutput.WriteString("F:");
				//fileInput.ReadString(sLine);
				//fileOutput.WriteString(sLine + "\n");
			}
			else if ("TRASH" == sLine)
			{
				fileOutput.WriteString("X:");
				fileInput.ReadString(sLine);
				fileOutput.WriteString(sLine + "\n");
			}
			else
			{
				fileOutput.WriteString(">> ERROR:");
				fileOutput.WriteString(sLine + "\n");
				printf(">> ERROR: Unrecognized token '%s'\n", sLine);

			}
		}

		fileInput.Close();
		fileOutput.Close();
		file::MarkJobDone(szOutputFile);
	}
};


void processor::Run()
{
	Initialize();
	Step1_VssPaths    (paths::szStep1_VssDir);
	Step2_CollectInfo (paths::szStep1_VssDir, paths::szStep2_Paths, paths::szStep2_SkippedPaths);
	Step3_GroupInfo   (paths::szStep2_Paths,  paths::szStep3_Grouped);

	//SDataVect vect;

	printf("\n>> ");
	system("TIME/T");
}



