#include "stdafx.h"

#include "Functions/Processor.h"
#include "Functions/FileFunc.h"
#include "Functions/Config.h"

#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWinApp theApp;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	RUN("COLOR 0A");

	printf("WARNING!!!\n");
	printf("\n");

	printf("1) These VSS environment variables must be set (eg.):\n");
	printf("    ssDir  = C:\\VSS\\MyVssDatabaseDir\n");
	printf("    ssUser = Admin\n");
	printf("    ssPwd  =\n");
	printf("\n");

	printf("   Currently:\n");
	printf("    ");
	RUN("SET ssDir");
	printf("    ");
	RUN("SET ssUser");
	printf("    ");
	RUN("SET ssPwd");
	printf("\n");

	printf("2) Your locale date-time should be in one of these formats\n");
	printf("   '2010.10.10 10:10'  '2010-10-10 10:10'\n");
	printf("\n");

	printf("3) Create file '%s' with date-time string (see above)\n", config::szFromDate);
	printf("   to import/append only from specified date (e.g. update imported database)\n");
	printf("\n");

//	printf("4) Hardcoded\n", config::szFromDate);
//	szEmail
//		szTimeZone
//	printf("   to import/append from that date (e.g. import from another VSS database)\n");
//	printf("\n");

	processor::Run();

	printf("\nDONE!   (press ENTER to exit)\n");
	getchar();
	return 0;
}


