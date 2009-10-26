#include "StdAfx.h"
#include "Processor.h"

#include "Data.h"
#include "VssFunc.h"
#include "GitFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//	system("SET ssDir=");
//	system("SET ssUser=");
//	system("SET ssPwd=");

const CString ss = "\"c:\\Program Files\\Microsoft Visual Studio\\Common\\VSS\\win32\\ss.exe\"";


static void InitVss()
{
	system(ss + " workfold $/ ..\\tmp\\Working");
	system(ss + " History $/Wood/MatrixKozijn/StdAfx.h >>..\\tmp\\history.txt");

}

static void Collect(SDataVect &vect)
{

	vss::get_all();

};


void processor::Run()
{
	InitVss();

	SDataVect vect;
	Collect(vect);

}



