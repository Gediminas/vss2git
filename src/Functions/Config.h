#ifndef __Config_H__
#define __Config_H__
#pragma once


namespace paths
{
	const LPCTSTR szTmpDir             = "../tmp";
	const LPCTSTR szWorkingDir         = "../tmp/Working";
	const LPCTSTR szDump               = "../tmp/dump.txt";
	const LPCTSTR szCounters           = "../tmp/Counters.txt";

	const LPCTSTR szStep1_VssDir       = "../tmp/Step1_VssDir.txt";
	const LPCTSTR szStep2_Paths        = "../tmp/Step2_Info.txt";
	const LPCTSTR szStep2_SkippedPaths = "../tmp/Step2_SkippedPaths.txt";
	const LPCTSTR szStep3_Grouped      = "../tmp/Step3_Grouped.txt";
	const LPCTSTR szStep4_Import       = "../tmp/Step4_Import.txt";
	const LPCTSTR szImportProgress     = "../tmp/ImportProgress.txt";
}




#endif //__Config_H__
