#include "stdafx.h"

#include "Functions/Processor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWinApp theApp;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	printf("--------------------------\n");
	system("SET ssDir");
	system("SET ssUser");
	system("SET ssPwd");
	printf("--------------------------\n");

	//printf("Press any key to start!\n");
	//getchar();

	processor::Run();

	printf("\nDone!\n");
	getchar();
	return 0;
}


