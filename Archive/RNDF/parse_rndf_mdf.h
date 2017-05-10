/*
 *Author : sherif ahmed
 *Date : 26/3/2011
 *Purpose : parse rndf and mdf files
 */

#ifndef PARSE_RNDF_MDF_H
#define PARSE_RNDF_MDF_H

#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

#ifndef _IO_FILE
#define _IO_FILE FILE
#endif

char **divideLine(char*, int, int);
int lineLenght(char*);
int numberOfTabs(char*);
int numberOfWords(char*);
char **getArrayOfLines(const FILE*, const int, const int);
int numberOfLines(const FILE*);
char **mallocCharArray(int, int);
void freeCharArray(char**, int);
int compTwoStrings(char*, char*);
void stringCopy(char*, char*);
char **removeEmptyLines(char**, int, int);
char **removeComments(char**, int, int);
int searchForCommentStart(char*, int);
int searchForCommentEnd(char*, int);

#endif

//END
