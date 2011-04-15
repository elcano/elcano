// RNDF.c : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rndf_mdf_reader.h"

int _tmain(int argc, _TCHAR* argv[])
{
	struct RNDF *rndf;
	struct MDF *mdf;
	FILE *rndf_file, *mdf_file;
	fopen_s(&mdf_file, "Sample_MDF.txt", "r");
    mdf = parseAnalyzeMdfFile(mdf_file);
	fopen_s(&rndf_file, "sample_rndf.txt", "r");
	rndf = parseAnalyzeRndfFile(rndf_file);
	/* TO DO: write rndf and mdf to mdf_rndf.c
	mdf_rndf.c sets constant values for data that is used by the C4 Planner.
	This file is included when C4 Planner is compiled on the Arduino.
	Thus the micro-processor does not need to be able to read files.
	A new version of the C4 Software will be uploaded for each mission.
	*/
	freeMDF(mdf);
	freeRNDF(rndf);
	return 0;
}

