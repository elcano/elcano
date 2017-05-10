use the parseAnalyzeRndfFile function in the rndf_mdf_reader.h file
by passing the file descriptor of the rndf file to it and it will return 
a RNDF struct which contains all the information in the file

use freeRNDF in the rndf_mdf_reader.h file to free all space 
allocated for the RNDF struct

use the parseAnalyzeMdfFile function in the rndf_mdf_reader.h file
by passing the file descriptor of the mdf file to it and it will return 
a MDF struct which contains all the information in the file

use freeMDF in the rndf_mdf_reader.h fileto free all space 
allocated for the MDF struct

the program will fail/crash if there is any type error in the input file
like : if it was space delimited instead of tab delimited or writing
the file parameters in any other order than the standard.
