
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  char * str[3];
  char * aa = "helloworld0";
  char * bb = "helloworld1";
  char * cc = "helloworld2";
  str[0] = (char *)malloc(strlen(aa)+1);
  strcpy(str[0],aa);
  str[1] = (char *)malloc(strlen(bb)+1);
  strcpy(str[1],bb);
  str[2] = (char *)malloc(strlen(cc)+1);
  strcpy(str[2],cc);

  char ** strr = str;


  printf("Before free----Length:%ld,Content:%s,Address:%p\n",strlen(strr[1]),strr[1],strr[1]);
  free(strr[1]);
  strr[1] = NULL;
  if (strr[1]) printf("not NULL 1\n");
  printf("After free----Length:%ld,Content:%s,Address:%p\n\n",strlen(strr[1]),strr[1],strr[1]);


  printf("Before free----Length:%ld,Content:%s,Address:%p\n",strlen(strr[0]),strr[0],strr[0]);
  free(strr[0]);
  strr[0] = NULL;
  if (strr[0]==NULL) printf("not NULL 0\n");
  printf("After free----Length:%ld,Content:%s,Address:%p\n\n",strlen(strr[0]),strr[0],strr[0]);

  printf("Before free----Length:%ld,Content:%s,Address:%p\n",strlen(strr[2]),strr[2],strr[2]);
  free(strr[2]);
  strr[2] = NULL;
  if (strr[2]) printf("not NULL 2\n");
  printf("After free----Length:%ld,Content:%s,Address:%p\n\n",strlen(strr[2]),strr[2],strr[2]);

}
