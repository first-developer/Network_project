/* Test file for associative arrays library */

#include <stdio.h>
#include <stdlib.h>

#include "libarrays.h"

int main(void){
AssocArray *a=NULL;
arraysDisplayArray(stdout,a);
int v0=1;
arraysSetValue(&a,"1",(void *)&v0,sizeof(int),AARRAY_FORCE_NUMERIC);
arraysDisplayArray(stdout,a);
int v=*(int *)arraysGetValue(a,"1",NULL,AARRAY_FORCE_NUMERIC);
fprintf(stdout,"value=%d\n",v);
char c0=42;
arraysSetValue(&a,"10",(void *)&c0,sizeof(char),AARRAY_FORCE_NUMERIC);
arraysDisplayArray(stdout,a);
int c=*(char *)arraysGetValue(a,"10",NULL,AARRAY_FORCE_NUMERIC);
fprintf(stdout,"value=%hhx\n",c);
arraysSetValue(&a,"text",(void *)"abcdefghijklmnopq",18,0);
arraysDisplayArray(stdout,a);
char *s=(char *)arraysGetValue(a,"text",NULL,0);
fprintf(stdout,"string=%s\n",s);
arraysFreeArray(a);
exit(0);
}
