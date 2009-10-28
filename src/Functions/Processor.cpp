#include "StdAfx.h"
#include "Processor.h"

#include "Data.h"
#include "VssFunc.h"
#include "GitFunc.h"
#include "FileFunc.h"
#include "Config.h"

//#include "Tools/FileUtil.h"
//#include "Tools/FileUtilEx.h"

#include <conio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


template <class _InputIter, class _Function>
inline void for_each2(_InputIter __first, _InputIter __last, _Function &__f)
{
  for ( ; __first != __last; ++__first)
    __f(*__first);
}



class CImportGroupData
{
public:
	CImportGroupData()
	: m_nCurrentLine(-1),
	  m_nItem(-1),
	  m_nCount(-1)
	{

	}

	~CImportGroupData()
	{

	}

	void Init(int nCount, LPCTSTR szWorkingDir, LPCTSTR szOutputFile)
	{
		m_nCurrentLine = 0,
		m_nItem        = 0,
		m_nCount       = nCount,
		m_sWorkingDir  = szWorkingDir;
		m_sOutputFile  = szOutputFile;
		
		m_pFileProgress = new CStdioFile;
		
		if (m_pFileProgress->Open(config::szImportProgress, CFile::modeRead | CFile::shareDenyNone, NULL))
		{
			CString sLine;
			if (m_pFileProgress->ReadString(sLine))
			{
				m_nCurrentLine = atoi(sLine);
			}
			else
			{
				//ASSERT(FALSE); //Corrupted file???
			}
			m_pFileProgress->Close();
		}

		VERIFY(m_pFileProgress->Open(config::szImportProgress, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, NULL));

		//GetCurrentDirectory(2000, m_sSysWorkingDir.GetBufferSetLength(2000));
		//SetCurrentDirectory(config::szWorkingDir);
		//GetCurrentDirectory(2000, m_sVssWorkingDir.GetBufferSetLength(2000));
	}

	void operator () (SGroupData* pGroupData)
	{
		++ m_nItem;

		if (m_nItem <= m_nCurrentLine)
		{
			return; //already imported
		}
		++ m_nCurrentLine;

		printf("\r>> %d%%  processing %d/%d  %s  %s          ",
			100 * m_nCurrentLine / m_nCount,
			m_nCurrentLine,
			m_nCount,
			pGroupData->time,
			pGroupData->user);

		//SetCurrentDirectory(m_sSysWorkingDir);
		RUN(FormatStr("ECHO ************************************************************ >> %s", m_sOutputFile));
		RUN(FormatStr("ECHO.>> %s", m_sOutputFile));
		
		RUN(FormatStr("ECHO Processing %d>> %s", m_nCurrentLine, m_sOutputFile));
		RUN(FormatStr("ECHO %s  %s>> %s", pGroupData->time, pGroupData->user, m_sOutputFile));
		
		RUN(FormatStr("ECHO.>> %s", m_sOutputFile));
		//RUN(FormatStr("ECHO       *** VSS ***>> %s", m_sOutputFile));

		for (SDataVect::iterator it = pGroupData->data_vect->begin(); pGroupData->data_vect->end() != it; ++ it)
		{
			SData *&data = (*it);
			vss::get_file(data->file, data->version, m_sWorkingDir, m_sOutputFile);

			if (_kbhit())
			{
				if (27 == getch())
				{
					m_nCurrentLine = INT_MAX;
					return;
				}
			}
		}

		RUN(FormatStr("ECHO.>> %s", m_sOutputFile));
		//RUN(FormatStr("ECHO       *** GIT ***>> %s", m_sOutputFile));

		git::Commit(m_sOutputFile, m_sWorkingDir, pGroupData->time, pGroupData->user, config::szEmail, m_nCurrentLine);
		
		RUN(FormatStr("ECHO.>> %s", m_sOutputFile));

		m_pFileProgress->SetLength(0);
		m_pFileProgress->WriteString(FormatStr("%d\n", m_nCurrentLine));
		m_pFileProgress->Flush();
	}

	void Destroy()
	{
		if (INT_MAX == m_nCurrentLine)
		{
			printf("\n>> ABORTED", 100 * m_nCurrentLine / m_nCount, m_nCurrentLine);
		}
		else
		{
			printf("\r>> FINISHED", 100 * m_nCurrentLine / m_nCount, m_nCurrentLine);
		}

		printf("                    ");//20
		printf("                    ");//40
		printf("                    ");//60
		printf("\n");

		m_pFileProgress->Close();
		delete m_pFileProgress;
		m_pFileProgress = NULL;

		//SetCurrentDirectory(m_sSysWorkingDir);
	}

private:
	int m_nCurrentLine;
	int m_nItem;
	int m_nCount;
	CStdioFile *m_pFileProgress;
	CString m_sOutputFile;
	CString m_sWorkingDir;

	//CString m_sSysWorkingDir;
	//CString m_sVssWorkingDir;
};




static void AddFileVersions(LPCTSTR szFilePath, CStdioFile &output_file)
{
	CString sFilePath = szFilePath;

	if (-1 != sFilePath.Find("MatrixKozijnTools"))
	{
		ASSERT(FALSE);
		sFilePath.Replace("MatrixKozijnTools", "MatrixKozijn Tools");
	}

	vss::list_file_versions(sFilePath, config::szDump);

	CStdioFile fileDump;
	CFileException fe;

	if (!fileDump.Open(config::szDump, CFile::modeRead | CFile::shareDenyWrite, &fe))
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

//			if (-1 != sFilePath.Find("BrandViewFrameAll.h"))
//			{
//				ASSERT(FALSE);
//			}
			
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
			else if ("**********************" == sLine)
			{
				do
				{
					output_file.WriteString("TRASH\n");
					output_file.WriteString(sLine);
					output_file.WriteString("\n");
				}
				while(fileDump.ReadString(sLine) && !sLine.IsEmpty());
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

static bool BuildDataVect(SDataVect &vect, LPCTSTR szInputFile)
{
	CStdioFile fileInput, fileOutput;
	CFileException fe;

	if (!fileInput.Open(szInputFile, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		printf(">> input file error\n");
		getchar();
		exit(1);
	}

	try
	{
		const DWORD dwFileLength = fileInput.GetLength();

		bool bAdd = false;
		CString sKey, sParam;
		SData *pData = NULL;

		while (fileInput.ReadString(sKey))
		{
			if (sKey.IsEmpty())
			{
				continue;
			}

			if ("**********" == sKey)
			{
				if (bAdd)
				{
					ASSERT_POINTER(pData, SData);
					ASSERT(0 < pData->version);

					vect.push_back(pData);
					
					bAdd = false;

					if (1 == pData->version)
					{
						printf("\r>> %d%%", 100 * fileInput.GetPosition() / dwFileLength);
					}
				}
				else
				{
					delete pData;
				}

				pData = new SData;
			}
			else if (0 == sKey.Find("ACTION_"))
			{
				ASSERT(!bAdd);
				ASSERT_POINTER(pData, SData);
				ASSERT(0 < pData->version);
				ASSERT(INT_MAX != pData->version);

				if ("ACTION_COMMITED" == sKey)
				{
					ASSERT(1 < pData->version);
					bAdd = true;
				}
				else if ("ACTION_CREATED" == sKey)
				{
					ASSERT(1 == pData->version);
					bAdd = true;
				}
				else if ("ACTION_BRANCHED" == sKey)
				{
					//skip
				}
				else if ("ACTION_LABELED" == sKey)
				{
					//skip
				}
				else
				{
					//skip
					printf(">> ERROR: Unrecognized action '%s'\n", sKey);
					ASSERT(FALSE);
				}
			}
			else
			{
				ASSERT_POINTER(pData, SData);
				fileInput.ReadString(sParam);
				ASSERT(!sParam.IsEmpty());

				if ("FILE" == sKey)
				{
					pData->file = sParam;
				}
				else if ("VERSION" == sKey)
				{
					ASSERT(0 == pData->version);
					pData->version = atoi(sParam);
					ASSERT(0 < pData->version);
				}
				else if ("USER" == sKey)
				{
					pData->user = sParam;
				}
				else if ("TIME" == sKey)
				{
					pData->time = sParam;
				}
				else if ("LABEL" == sKey)
				{
					pData->label = sParam;
				}
				else if ("LABEL_COMMENT" == sKey)
				{
					pData->label_comment = sParam;
				}
				else if ("COMMENT" == sKey)
				{
					pData->comment = sParam;
				}
				else if ("TRASH" == sKey)
				{
					//skip
				}
				else
				{
					//skip
					printf(">> ERROR: Unrecognized token '%s' -> '%s'\n", sKey, sParam);
					ASSERT(FALSE);
				}
			}
		}

		if (bAdd)
		{
			ASSERT_POINTER(pData, SData);
			ASSERT(0 < pData->version);

			vect.push_back(pData);
		}
	}
	catch(CFileException *e)
	{
		e->ReportError();
		ASSERT(FALSE);
		e->Delete();

		fileInput.Close();
		return false;
	}

	fileInput.Close();
	printf("\r>> finished\n");
	return true;
}

static bool StoreDataVect(const SGroupDataVect &group_vect, LPCTSTR szOutputFile)
{
	CStdioFile file;
	CFileException fe;

	if (!file.Open(szOutputFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &fe))
	{
		printf(">> input file error\n");
		getchar();
		exit(1);
	}

	try
	{
		CStoreGroupData store(file, group_vect.size());
		for_each2(group_vect.begin(), group_vect.end(), store);
	}
	catch(CFileException *e)
	{
		e->ReportError();
		ASSERT(FALSE);
		e->Delete();

		file.Close();
		return false;
	}

	file.Close();
	printf("\r>> finished\n");
	return true;
}

static void GroupDataVect(SDataVect &vect, SGroupDataVect &group_vect)
{
	CDataVectGrouping group(group_vect);
	for_each2(vect.begin(), vect.end(), group);
	group.ValidateLastGroup();
}


static bool BuildGroupDataVect(SDataVect &vect_for_auto_delete, SGroupDataVect &group_vect, LPCTSTR szInputFile)
{
	CStdioFile fileInput;
	CFileException fe;

	if (!fileInput.Open(szInputFile, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		printf(">> input file error\n");
		getchar();
		exit(1);
	}

	try
	{
//		const DWORD dwFileLength = fileInput.GetLength();
//
//		bool bAdd = false;
		CString sLine;
		SGroupData *pGroupData = NULL;
		SData *pData = NULL;

		while (fileInput.ReadString(sLine))
		{
			if (sLine.IsEmpty())
			{
				continue;
			}

			if ("**********" == sLine)
			{
				group_vect.push_back(pGroupData = new SGroupData);
				
				fileInput.ReadString(sLine);
				ASSERT(group_vect.size() == atoi(sLine));

				fileInput.ReadString(sLine);
				pGroupData->time = sLine;

				fileInput.ReadString(sLine);
				pGroupData->user = sLine;

				fileInput.ReadString(sLine);
				while (!sLine.IsEmpty())
				{
					vect_for_auto_delete.push_back(pData = new SData);
					pGroupData->data_vect->push_back(pData);
					pData->file = sLine;

					fileInput.ReadString(sLine);
					pData->version = atoi(sLine);
					ASSERT(0 < pData->version);

					fileInput.ReadString(sLine);
				}
			}
			else
			{
				printf(">> ERROR: Unknown line - " + sLine);
			}
		}
	}
	catch(CFileException *e)
	{
		e->ReportError();
		ASSERT(FALSE);
		e->Delete();

		fileInput.Close();
		return false;
	}

	fileInput.Close();
	printf("\r>> finished\n");
	return true;
}

static bool Import(SGroupDataVect &group_vect, LPCTSTR szWorkingDir, LPCTSTR szOutputFile)
{
	CImportGroupData import;
	import.Init(group_vect.size(), szWorkingDir, szOutputFile);
	for_each2(group_vect.begin(), group_vect.end(), import);
	import.Destroy();
	return true;
}


inline bool CheckExt(const CString &sLine, LPCTSTR szExt)
{
	return ((sLine.GetLength() - strlen(szExt)) == sLine.Find(szExt));

}



static void Initialize(LPCTSTR szTmpDir, LPCTSTR szWorkingDir)
{
	printf("\nINIT");
	printf("\n>> ");
	RUN("TIME/T");

	printf(">> create folder '%s'\n", szWorkingDir);
	::CreateDirectory(szTmpDir, NULL);
	file::CreateDirectoryRecursive(szTmpDir, NULL);
	::CreateDirectory(szWorkingDir, NULL);
	file::CreateDirectoryRecursive(szWorkingDir, NULL);
	printf("\n");

	CString sGitIgnore(szWorkingDir);
	sGitIgnore += "/.gitignore";
	::DeleteFile(sGitIgnore);
	RUN(FormatStr("ECHO *.scc>> %s", sGitIgnore));
	RUN(FormatStr("ECHO *.obj>> %s", sGitIgnore));

	git::CreateDB(config::szInit, szWorkingDir, config::szEmail);
	vss::init_root_workfolder(szWorkingDir);
}

static void Step1_VssPaths(LPCTSTR szOutputFile)
{
	printf("\nSTEP1");
	printf("\n>> ");
	RUN("TIME/T");
	vss::list_all_files(szOutputFile);
}

static void Step2_CollectInfo(LPCTSTR szInputFile, LPCTSTR szOutputFile, LPCTSTR szOutputSkippedFile)
{
	printf("\nSTEP2");
	printf("\n>> ");
	RUN("TIME/T");

	if (file::StartJob(szOutputFile))
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
		int nSkippedFileCount = 0;

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
				if (CheckExt(sLine, ".h") || CheckExt(sLine, ".cpp"))
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
					if (-1 != sLine.Find(" item(s)"))
					{
						fileSkipped.WriteString(FormatStr("\n\n>> SKIPPED FILES: %d\n", nSkippedFileCount));
						fileSkipped.WriteString("TOTAL FILES: " + sLine + "\n");
					}
					else
					{
						fileSkipped.WriteString(sCurrentFolder + "/" + sLine + "\n");
						++ nSkippedFileCount;
					}
				}
			}
		}

		fileInput.Close();
		fileOutput.Close();
		fileSkipped.Close();
		::DeleteFile(config::szDump);
		file::MarkJobDone(szOutputFile);
	}
};


static void Step3_GroupInfo(LPCTSTR szInputFile, LPCTSTR szOutputFile)
{
	printf("\nSTEP3");
	printf("\n>> ");
	RUN("TIME/T");

	if (file::StartJob(szOutputFile))
	{
		::DeleteFile(config::szCounters);

		SDataVect vect;
		SGroupDataVect group_vect;

		printf(">> building data vector\n");
		if (!BuildDataVect(vect, szInputFile))
		{
			printf(">> failed\n");
			getchar();
			exit(1);
		}

		printf(FormatStr(">> file+version count: %d\n", vect.size()));
		RUN(FormatStr("ECHO Step3 FILE+VERSION count: %7d >> %s", vect.size(), config::szCounters));

		printf(">> sorting vector by date+user\n");
		std::sort(vect.begin(), vect.end(), data::compare_by_time_user);

		printf(">> grouping vector by date+user\n");
		GroupDataVect(vect, group_vect);

		RUN(FormatStr("ECHO Step3 estimated VSS GET    count: %7d >> %s", vect.size(), config::szCounters));
		RUN(FormatStr("ECHO Step3 estimated GIT COMMIT count: %7d >> %s", group_vect.size(),           config::szCounters));

		printf(">> storing data vector\n");
		if (!StoreDataVect(group_vect, szOutputFile))
		{
			printf(">> store vector failed\n");
			getchar();
			exit(1);
		}

		file::MarkJobDone(szOutputFile);
	}
};


static void Step4_Import(LPCTSTR szInputFile, LPCTSTR szOutputFile, LPCTSTR szWorkingDir)
{
	printf("\nSTEP4");
	printf("\n>> ");
	RUN("TIME/T");

	if (file::StartJob(szOutputFile, true))
	{
		SDataVect vect_for_auto_delete;
		SGroupDataVect group_vect;

		printf(">> building grouped data vector\n");
		if (!BuildGroupDataVect(vect_for_auto_delete, group_vect, szInputFile))
		{
			printf(">> failed\n");
			getchar();
			exit(1);
		}

		printf(FormatStr(">> estimated VSS GET    count: %7d\n",  vect_for_auto_delete.size()));
		printf(FormatStr(">> estimated GIT COMMIT count: %7d\n",  group_vect.size()));
		RUN(FormatStr("ECHO Step4 estimated VSS GET    count: %7d >> %s", vect_for_auto_delete.size(), config::szCounters));
		RUN(FormatStr("ECHO Step4 estimated GIT COMMIT count: %7d >> %s", group_vect.size(),           config::szCounters));

		printf(">> IMPORTING\n");
		if (!Import(group_vect, szWorkingDir, szOutputFile))
		{
			printf(">> IMPORT FAILED\n");
			getchar();
			exit(1);
		}

		//file::MarkJobDone(szOutputFile);
	}
};


void processor::Run()
{
	CString sOriginalFolder, sVssWorkingDir;
	GetCurrentDirectory(2000, sOriginalFolder.GetBufferSetLength(2000));
	
	Initialize(config::szTmpDir, config::szWorkingDir);

	SetCurrentDirectory(config::szWorkingDir);
	GetCurrentDirectory(2000, sVssWorkingDir.GetBufferSetLength(2000));
	SetCurrentDirectory(sOriginalFolder);

	Step1_VssPaths    (config::szStep1_VssDir);
	Step2_CollectInfo (config::szStep1_VssDir,  config::szStep2_Paths, config::szStep2_SkippedPaths);
	Step3_GroupInfo   (config::szStep2_Paths,   config::szStep3_Grouped);
	Step4_Import      (config::szStep3_Grouped, config::szStep4_Import, sVssWorkingDir);


	printf("\n>> ");
	RUN("TIME/T");
}



