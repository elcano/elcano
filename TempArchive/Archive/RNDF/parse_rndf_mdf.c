/*
 *Author : sherif ahmed
 *Date : 26/3/2011
 *Purpose : parse rndf and mdf files
 */

#include "parse_rndf_mdf.h"

/*
 *divideLine function
 *
 *line: a null terminated string of words separated by tabes 
 *now: an integer represinting the number of words in the line
 *mwl: an integer represinting the max word length
 *
 *the function will return an array of null terminated strings 
 *represinting each word in the line
 *
 *free the returned array by freeCharArray(returned_array, now);
 */
char **divideLine(char *line, int now, int mwl)
{
  char c;
  int n1 = 0;
  int n2 = 0;
  int n3 = 0;
  char **params;
  int flag = 0;
  
  params = mallocCharArray(now, mwl + 1);// add 1 for the null character
  assert(params != 0);
  while((c = line[n1++]) != '\0'){
    if(c != '\t'){
      params[n2][n3] = c;
      flag = 1;
      n3++;
    }
    else if(c == '\t' && flag == 1){
      flag = 0;
      params[n2][n3] = '\0';
      n3 = 0;
      n2++;
    }
  }
  if(n2 < now){
    params[n2][n3] = '\0';//------
  }
  return params;
}

/*
 *lineLength function
 *
 *line: a string
 *
 *this function will return the length of a null terminated string.
 */
int lineLenght(char *line)
{
  int count = 0;

  while(line[count++] != '\0');

  return (count - 1);
}

/*
 *numberOfTabs function
 *
 *line: a null terminated string
 *
 *this function will return the number of tabs in a null terminated string.
 */
int numberOfTabs(char *line)
{
  int count = 0;
  int n = 0;
  char c;

  while((c = line[n++]) != '\0')
    if(c == '\t')
      count++;

  return count;
}

/*
 *numberOfWords function
 *
 *line: a null terminated string
 *
 *this function will return the number of words in a tab delimited string
 */
int numberOfWords(char *line)
{
  int count = 0;
  int n = 0;
  int flag = 0;
  char c;

  while((c = line[n]) != '\0'){
    if(c != '\t' && flag == 0){
      count++;
      flag = 1;
    }
    else if(c == '\t')
      flag = 0;
    n++;
  }
  return count;
}

/*
 *getArrayOfLines function
 *
 *file: a pointer to a file descriptor
 *max_nol: maximum number of lines 
 *max_ll: maximum line length
 *
 *this function will return an array of null terminated string represinting
 *the file of the file descriptor in the first variable
 *
 *free the returned array by freeCharArray(returned_arr, max_nol)
 */
char** getArrayOfLines(const FILE *file, const int max_nol, const int max_ll)
{
  char c;
  char **temp;
  int lc = 0;
  int i;
  
  temp = (char**)malloc(sizeof(char*)*max_nol);
  assert(temp != 0);
  c = getc((_IO_FILE *)file);
  while(c != EOF && lc < max_nol){
    char *line = (char*)malloc(sizeof(char)*max_ll);
    assert(line != 0);
    i = 0;
    while(c != '\n' && c != '\r' && c != EOF){
      line[i] = c;
      c = getc((_IO_FILE *)file);
      i++;
    }
    if(c == '\r'){
      c = getc((_IO_FILE *)file);
      assert(c == '\n');
    }
    line[i] = '\0';
    temp[lc] = line;
    lc++;
    c = getc((_IO_FILE *)file);
  }
  while(lc < max_nol){
    char *line = (char*)malloc(sizeof(char)*max_ll);
    line[0] = '\0';
    temp[lc] = line;
    lc++;
  }

  return temp;
}

/*
 *numberOfLines function
 *
 *file: a file descriptor
 *
 *this function will return the number of lines in a file
 */
int numberOfLines(const FILE *file)
{
  int count = 0;
  char c;

  while((c = getc(( _IO_FILE *)file)) != EOF)
    if(c == '\n')
      count++;

  return ++count;
}

/*
 *mallocCharArray function
 *
 *dim1: an integer representing the length of the first dimention of the array
 *dim2: an integer representing the length of the second dimention of the array
 *
 *this function will allocate memory for a two dimentional char array and return
 *the base address of the allocated memory
 */
char** mallocCharArray(int dim1, int dim2)
{
  char **arr;
  int i;
  
  arr = malloc(dim1 * sizeof(char*));
  assert(arr != 0);
  for(i = 0; i < dim1; i++){
    arr[i] = malloc(dim2 * sizeof(char));
    assert(arr[i] != 0);
  }

  return arr;
}

/*
 *freeCharArray function
 *
 *arr: a two dimentional array pointer
 *dim: an integer representing the length of the first dimention of the array
 *
 *this function will free the memory allocated for a two dimentional char
 *array
 */
void freeCharArray(char **arr, int dim1)
{
  int i;
  for(i = 0; i < dim1; i++)
    free(arr[i]);

  free(arr);
}

/*
 *compTwoStrings function
 *
 *st1 : a pointer to the first null terminated string
 *st2 : a pointer to the second null terminated string
 *
 *this function will return 1 if the two strings are identical
 *and will return 0 if they are not identical
 */
int compTwoStrings(char *st1, char *st2)
{
  int ch = 1;
  int n = -1;

  do{
    n++;
    if(st1[n] != st2[n])
      ch = 0;
  }while(st1[n] != '\0' && st2[n] != '\0');

  return ch;
}

/*
 *stringCopy function
 *
 *str1 : a pointer to a null terminated string
 *str2 : a pointer to a null terminated string
 *
 *will copy str1 to str2
 */
void stringCopy(char *str1, char *str2)
{
  int n = 0;
  
  do{
    str2[n] = str1[n];
  }while(str1[n++] != '\0');
}

/*
 *searchLine function
 *
 *p : array of pointers to null terminated strings
 *nol : number of strings in p
 *line : the line that will be searched for in p
 *
 *this function will search for a string (line) in
 *an array of pointers to null terminated strings (p)
 */
int searchLine(char **p, int nol, char *line)
{
  int n = 0;
  int i;

  for(i = 0; i < nol; i++){
    if(compTwoStrings(p[i], line))
      return ++i;
  }
  return 0;
}

/*
 *removeEmptyLines function
 *
 *arr : is an array of pointers to a null terminated strings
 *nol : the number of the strings
 *mll : the maximum string/line length
 *
 *this function will remove the empty from the beginning 
 *lines of the arr and between the strings and will return
 *an array without the empty lines but 
 */
char **removeEmptyLines(char **arr, int nol, int mll)
{
  int i;
  int j = 0;
  char **res = malloc(nol * sizeof(char*));
  for(i = 0; i < nol; i++)
    res[i] = malloc(mll * sizeof(char));
  for(i = 0; i < nol; i++){
    if(numberOfWords(arr[i]) > 0){
      stringCopy(arr[i], res[j]);
      j++;
    }
  }
  for(i = j; i < nol; i++)
    arr[i][0] = '\0';

  return res;
}

/*
 *removeComments function
 *
 *arr : an array of pointers to a null terminated strings
 *nol : the number of strings in arr
 *mll : the maximim string length
 *
 *this function will remove the comments from the end
 *of the strings in arr 
 *the comment begins by '/*' ,every thing after that will
 *be removed
 */
char **removeComments(char **arr, int nol, int mll)
{
  int i;
  int j = 0;
  char **res = malloc(nol * sizeof(char*));
  int cpos;

  for(i = 0; i < nol; i++)
    res[i] = malloc(mll * sizeof(char));
  for(i = 0; i < nol; i++){
    stringCopy(arr[i], res[i]);
    if((cpos = searchForCommentStart(res[i], mll)) != 0){
      res[i][cpos - 1] = '\0';
      if(!searchForCommentEnd(res[i], mll)){
	i++;
	while(!searchForCommentEnd(res[i], mll)){
	    res[i][0] = '\0';
	    i++;
	}
	res[i][0] = '\0';
      }
    }
  }
  return res;
}

/*
 *searchForCommentStart function
 *
 *line : the line to search for comment in
 *mll : maximum line length
 *
 *this function will retrun the comment start position
 *in line
 *it will search for '/*', if it didn't find it
 *it will return 0
 */
int searchForCommentStart(char *line, int mll)
{
  int i;
  int n = 0;
  
  for(i = 0; i < mll - 1; i++)
    if(line[i] == '/' && line[i+1] == '*')
      return ++i;
  return 0;
}

/*
 *searchForCommentEnd function
 *
 *line : the line to search for comment end in
 *mll : maximum line length
 *
 *this function will retrun 1 if it finds
 *the comment end, if it didn't  find it
 *it will return 0
 */
int searchForCommentEnd(char *line, int mll)
{
  int i;

  for(i = 0; i < mll - 1; i++)
    if(line[i] == '*' && line[i+1] == '/')
      return 1;
  return 0;
}
//END
