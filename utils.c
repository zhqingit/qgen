#include <string.h>
#include <stdio.h>
#include <stdlib.h>



void * split_str(char *p[], char *buff, char * delim, int * len){


	if(NULL ==  buff || 0 == strlen(buff)) return NULL;
	if(NULL == delim || 0 == strlen(delim)) return NULL;

  char * strcp = malloc(strlen(buff)+1);
  strcpy(strcp,buff);

	char * ptr = strtok(strcp, delim); // 实现字符串的分割
  int i =  0;
	while(ptr != NULL)
	{
    p[i] = malloc(strlen(ptr)+1);
    strcpy(p[i],ptr);
    i++;
		ptr = strtok(NULL, delim);
	}

	*len = i;

  free(strcp);

  //return p;

  /*
	*pCount=0;
	ptr = strtok(str, delim);
	while(ptr != NULL)
	{
		for(int j =0; tmp[j]!='\0';j++)
		{
			if(tmp[j]=='\n')break; //到达行末
			(*dest)[j] = tmp[j];
		}
		(*dest)[j]='\0';
		dest++;
		(*pCount)++;

		ptr = strtok(NULL, delim);
	}
  */

}

void release_split_str(char **s){
  //printf("prefree--%s===%p---%ld\n",*(s+i),*(s+i),strlen(*(s+i)));
  int i=0;
  while(s[i]!=NULL){
    free(s[i]);
    s[i] = NULL;
    i++;
  }
}

void cp_str_malloc(char ** nstr, char *str){
    //printf("---%p\n",nstr);
    *nstr = (char *)malloc(strlen(str)+1);
    strcpy(*nstr,str);
    //printf("===%p\n",nstr);
}

void strcat_two_str(char *src1, char*src2, char **des, char *sep){
	char * tmp = malloc(strlen(src1)+strlen(sep)+strlen(src2)+1);
	strcpy(tmp,src1);
	strcat(tmp,sep);
	strcat(tmp,src2);
	*des = tmp;
}

void strcat_from_char_array(char **src, int len, char **des, char *sep){
	char *tmp1;
	char * tmp0 = malloc(strlen(src[0])+1);
	strcpy(tmp0,src[0]);
	for (int i=1;i<len;i++){
		tmp1 = malloc(strlen(tmp0)+strlen(sep)+strlen(src[i])+1);
		strcpy(tmp1,tmp0);
		strcat(tmp1,sep);
		strcat(tmp1,src[i]);
		free(tmp0);
		tmp0 = tmp1;
	}
	*des = tmp0;
	//printf("combine++++++++++++%s\n",*des);
}


void strcat_from_suint_array(short unsigned int src[], int len, char **des, char *sep){
	char * tmp2;
	char *tmp1;
	char * tmp0 = malloc(sizeof(short unsigned int));
	sprintf(tmp0,"%d",src[0]);
	for (int i=1;i<len;i++){
		//printf("=======%d\n",src[i]);
		tmp1 = malloc(strlen(tmp0)+strlen(sep)+sizeof(short unsigned int)+1);
		strcpy(tmp1,tmp0);
		strcat(tmp1,sep);
		tmp2 = malloc(sizeof(short unsigned int));
		sprintf(tmp2,"%d",src[i]);
		strcat(tmp1,tmp2);
		free(tmp2);
		free(tmp0);
		tmp0 = tmp1;
	}
	*des = tmp0;
	//printf("combine++++++++++++%s\n",*des);
}

void strcat_from_sint_array(short int src[], int len, char **des, char *sep){
	char * tmp2;
	char *tmp1;
	char * tmp0 = malloc(sizeof(short int));
	sprintf(tmp0,"%d",src[0]);
	for (int i=1;i<len;i++){
		//printf("=======%d\n",src[i]);
		tmp1 = malloc(strlen(tmp0)+strlen(sep)+sizeof(short int)+1);
		strcpy(tmp1,tmp0);
		strcat(tmp1,sep);
		tmp2 = malloc(sizeof(short int));
		sprintf(tmp2,"%d",src[i]);
		strcat(tmp1,tmp2);
		free(tmp2);
		free(tmp0);
		tmp0 = tmp1;
	}
	*des = tmp0;
	//printf("combine++++++++++++%s\n",*des);
}

void strcat_from_lint_array(long int src[], int len, char **des, char *sep){
	char * tmp2;
	char *tmp1;
	char * tmp0 = malloc(sizeof(long int));
	sprintf(tmp0,"%ld",src[0]);
	for (int i=1;i<len;i++){
		//printf("=======%d\n",src[i]);
		tmp1 = malloc(strlen(tmp0)+strlen(sep)+sizeof(long int)+1);
		strcpy(tmp1,tmp0);
		strcat(tmp1,sep);
		tmp2 = malloc(sizeof(long int));
		sprintf(tmp2,"%ld",src[i]);
		strcat(tmp1,tmp2);
		free(tmp2);
		free(tmp0);
		tmp0 = tmp1;
	}
	*des = tmp0;
	//printf("combine++++++++++++%s\n",*des);
}
