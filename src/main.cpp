#include "stdafx.h"

#include "Functions/Processor.h"
#include "Functions/FileFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWinApp theApp;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	RUN("COLOR 0A");

	printf("--------------------------\n");
	printf("Environment variables must be set\n");
	printf("eg.:\n");
	printf(" ssDir  = C:\\VSS\\MyVssDatabaseDir\n");
	printf(" ssUser = Admin\n");
	printf(" ssPwd  =\n");
	printf("\nCurrently set:\n\n");
	RUN("SET ssDir");
	RUN("SET ssUser");
	RUN("SET ssPwd");
	printf("\n--------------------------\n\n");

	printf("Press ENTER to START IMPORT!\n");
	getchar();

	processor::Run();

	printf("\nDone!\n");
	getchar();
	return 0;
}


