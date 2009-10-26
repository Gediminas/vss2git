#include "stdafx.h"

#include "Functions/Processor.h"


int main(int argc, char* argv[])
{
	printf("Press any key to start!\n");
	getchar();

	processor::Collect();

	printf("Done!\n");
	getchar();
	return 0;
}

