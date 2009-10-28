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

	printf("WARNING!!!\n\n");
	printf("1) Environment variables must be set\n");
	printf("   eg.:\n");
	printf("    ssDir  = C:\\VSS\\MyVssDatabaseDir\n");
	printf("    ssUser = Admin\n");
	printf("    ssPwd  =\n");
	printf("   Currently set:\n\n");

	printf("    ");
	RUN("SET ssDir");
	printf("    ");
	RUN("SET ssUser");
	printf("    ");
	RUN("SET ssPwd");
	printf("\n");

	printf("2) Your locale date/time should be in one of these formats\n");
	printf("   '2010.10.10 10:10'  '2010-10-10 10:10'\n");
	printf("\n");

	printf("3) Create file '%s' with date+time string (see above)\n", config::szFromDate);
	printf("   to import/append from that date (e.g. import from another VSS database)\n");
	printf("\n");

	processor::Run();

	printf("\nDONE!   (press ENTER to exit)\n");
	getchar();
	return 0;
}


