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
	RUN("SET ssDir");
	RUN("SET ssUser");
	RUN("SET ssPwd");
	printf("--------------------------\n");

	//printf("Press any key to start!\n");
	//getchar();

	processor::Run();

	printf("\nDone!\n");
	getchar();
	return 0;
}


