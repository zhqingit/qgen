#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>
#include <string.h>
#include "include/gene.h"
#include "include/utils.h"

QGEN_transcript qgen_init_tx(){
  QGEN_transcript tx = malloc(sizeof(struct qgen_transcript));
  tx->geneName = NULL;
  tx->name = NULL;
  tx->chromsome = NULL;
  tx->strand = -1;
  tx->txStart = -1;
  tx->txEnd = -1;
  tx->cdsStart = -1;
  tx->cdsEnd = -1;
  tx->exonStarts = NULL;
  tx->exonEnds = NULL;
  tx->intronStarts = NULL;
  tx->intronEnds = NULL;
  tx->exonCount = -1;
  tx->exonFrames = NULL;
  tx->cdsStartStat = NULL;
  tx->cdsEndStat = NULL;

  return tx;
};

void qgen_del_tx(QGEN_transcript tx){
  free(tx);
  tx=NULL;
};

//"sources/20200508_NCBIRef_hg19.txt"
void qgen_create_gene_from_file(char * file){

  static int vlen = 4096;
  FILE * fp = NULL;
  char buff[vlen];
  fp = fopen(file,"r");
  printf("-----------------\n");
  while(fgets(buff, vlen, (FILE*)fp)){
    int length;
    int *pc = &length;
    char * str[1000];
    split_str(str, buff,"\t",pc);

    int length1;
    int *pc1 = &length1;
    char * str1[1000];
    split_str(str1, buff,"\t",pc1);
    //char ** str = NULL;
    //QGEN_transcript tx = qgen_init_tx();
    //printf("befor split---%s\n",buff);
    //printf("string:%s +++++ %s\n",tx->chromsome, *(str+1));
    //qgen_create_tx_from_str(tx, str);
    //qgen_create_tx_from_str(tx, buff);

    //printf("after split---%s\n",str[0]);
    //qgen_del_tx(tx);
    //printf("string:%s ----- %s------%s-------%ld====%c\n",tx->chromsome, *(str+1),tx->geneName,tx->txStart, tx->strand);
    //release_split_str(str);
    //release_split_str(str1);
    //str=NULL;
    //str1=NULL;
    //for (int i=0; i<*pc;i++){
      //free(str[i]);
      //str[i] = NULL;
    //}

    //int ll;
    //int *pcc = &ll;
    //char *test="66999355,67000051,67091593,67098777,67105516,67108547,67109402,67136702,67137678,67139049,67142779,67145435,67154958,67155999,";
    //char ** estr = split_str(test,",",pcc);
    //release_split_str(estr,pcc,0);
  }
  fclose(fp);
}

//assign the values from ucsc table to transcript
//void qgen_create_tx_from_str(QGEN_transcript tx, char ** str){
void qgen_create_tx_from_str(QGEN_transcript tx, char * str){
  /*
  cp_str_malloc(&(tx->chromsome), *(str+2));
  cp_str_malloc(&(tx->name), *(str+1));
  cp_str_malloc(&(tx->geneName), *(str+12));
  tx->strand =  (*(str+3))[0];
  tx->txStart = atoi(*(str+4));
  tx->txEnd = atoi(*(str+5));
  tx->cdsStart = atoi(*(str+6));
  tx->cdsEnd = atoi(*(str+7));
  tx->exonCount = atoi(*(str+8));
  */

  int ll;
  int *pcc = &ll;
  //printf("before start:%s\n",*(str+9));
  //char *test="66999355,67000051,67091593,67098777,67105516,67108547,67109402,67136702,67137678,67139049,67142779,67145435,67154958,67155999";
  //char *test="66999355,67000051,67091593,67098777,67105516,67108547,67109402,67136702,67137678,67139049,67142779,67145435,67154958,67155999";
  //char ** estr = split_str(test,",",pcc);
  //printf("%s---%ld\n",*(str+9),strlen(*(str+9)));
  //char *tmp = malloc(strlen(*(str+9))+1);
  //char *tmp = malloc(strlen(test)+1);
  //char *tmp = malloc(2096);
  //printf("%s+++%ld\n",*(str+9),strlen(*(str+9)));
  //strcpy(tmp,*(str+9));
  //char ** estr = split_str1(tmp,",",pcc);
  //char ** estr = NULL;
  //split_str(&estr, *(str+9),",",pcc);
  //split_str(&estr, str,"\t",pcc);
  //release_split_str(estr);
  //printf("-------%s-%d\n",test,*pcc);
  //for (int i=0;i<ll;i++){
    //printf("%s-----------%d\n",estr[i],i);
    //free(estr[i]);
    //estr[i] = NULL;
  //}
  //estr = NULL;
  //for (int i=0;i<ll;i++){
    //printf("splitted values--%s\n",*(estr+i));
  //}

  //char ** estr = split_str(*(str+9),",",pcc);
  //printf("after start:%s--%d\n",*(str+9),ll);
  //tx->exonStarts = NULL;
  //tx->exonEnds = NULL;
  //tx->intronStarts = NULL;
  //tx->intronEnds = NULL;
  //tx->exonFrames = NULL;
  /*
  cp_str_malloc(&(tx->cdsStartStat), *(str+13));
  cp_str_malloc(&(tx->cdsEndStat), *(str+14));
  */
}


/*
//get transcript from gene based on name
typedef transcript(*qgen_get_tx_from_gene)(qgen_gene * gene, char * name);

//get the block from transcript
typedef qgen_block(*qgen_get_block_from_tx)(qgen_transcript * tran, int indexRNA, char * type);
//get the exon from transcript
typedef qgen_block(*qgen_get_exon_from_tx)(qgen_transcript * tran, int indexRNA);
//get the intron from transcript
typedef qgen_block(*qgen_get_intron_from_tx)(qgen_transcript * tran, int indexRNA);
 */
