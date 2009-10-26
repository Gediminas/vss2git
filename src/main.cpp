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
	printf("WARNING: Set these user environment variables before start (example):\n");
	printf("ssDir=\\\\vilnius\\sources-vss\\MxKozijn\n");
	printf("ssUser=Ilgas\n");
	printf("ssPwd=\n");
	printf("\n");
	//printf("Press any key to start!\n");
	//getchar();

	processor::Run();

	printf("\nDone!\n");
	getchar();
	return 0;
}


