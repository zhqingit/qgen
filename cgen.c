#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "include/qgen.h"
#include "include/cgen.h"


void print_genes(QGEN_genes gis ,char * chrom, long unsigned int coor, char strand ){
    int rgn;
    QGEN_gene *genes = gis->get_gene_from_coor(gis,chrom,strand,coor,&rgn);
    QGEN_return_gene_from_coor res;
    printf("totoal serach gnee:%d\n",rgn);
    for (int i=0;i<rgn;i++){
      if (genes[i]!=NULL) printf("search gene: %s,%d\n",genes[i]->name,genes[i]->ntx);
      else printf("search gene: Intergenic\n");
      res=genes[i]->return_gene_from_coor(genes[i], chrom,strand,coor);
      printf("ntra:%d\n",res->ntx);
      for (int j=0; j<res->ntx;j++) printf("=-====%d,%ld\n",j,res->disStarts[j]);
      res->print_return_gene_from_coor(res);
    }
}
void print_genes_from_file(QGEN_genes gis,char * file){
  int vlen = 2*2*4096;
  FILE * fp = NULL;
  char buff[vlen];
  fp = fopen(file,"r");
  while(fgets(buff, vlen, (FILE*)fp)){
    char * str[100000];
    int len;
    split_str(str, buff,":", &len);

    if (len!=3){
      printf("Error: please input chr:coordinate:strand or a file with same format\n");
      exit(-1);
    }

    int coor = atoi(str[1]);
    char strand = str[2][0];
    char * chrom = str[0];
    //printf("%s++%s++%s\n",str[2],str[1],chrom);
    print_genes(gis,chrom,coor, strand);
  }
  fclose(fp);
}

void extend_star_fusion(char * lbp, char * rbp, QGEN_genes gis){
  char * lbps[3];
  int lenl;
  split_str(lbps,lbp,":", &lenl);
  if (lenl!=3){
    printf("Error: please input chr:coordinate:strand or a file with same format\n");
    exit(-1);
  }
  char * rbps[3];
  int lenr;
  split_str(rbps,rbp,":", &lenr);
  if (lenr!=3){
    printf("Error: please input chr:coordinate:strand or a file with same format\n");
    exit(-1);
  }

  char * chroml = lbps[0];
  int coorl = atoi(lbps[1]);
  char strandl = lbps[2][0];

  char * chromr = rbps[0];
  int coorr = atoi(rbps[1]);
  char strandr = rbps[2][0];

  int rgnl=0;
  QGEN_gene *genesl = gis->get_gene_from_coor(gis,chroml,strandl,coorl,&rgnl);
  int rgnr=0;
  QGEN_gene *genesr = gis->get_gene_from_coor(gis,chromr,strandr,coorr,&rgnr);
  printf("left----%s,%d---right---%s,%d\n",lbp,rgnl,rbp,rgnr);

  for (int i=0;i<rgnl;i++){
    for (int j=0; j< rgnr;j++){
      char *fus;
      strcat_two_str(genesl[i]->name, genesr[j]->name, &fus, ":");
      char *bps;
      strcat_two_str(lbp, rbp, &bps, "/");

      //combine transcripts
      char *trans=malloc(strlen("")+1);
      strcpy(trans,"");
      char *tmpt;

      //combine blocks
      char *blocks=malloc(strlen("")+1);
      strcpy(blocks,"");
      char *tmpb;

      for (int it=0;it<genesl[i]->ntx;it++){
        for (int jt=0;jt<genesr[j]->ntx;jt++){
          char *ts;
          //transcrips
          strcat_two_str(genesl[i]->transcripts[it]->name, genesr[j]->transcripts[jt]->name, &ts, ":");
          tmp = malloc(strlen(trans)+1);
          strcpy(tmp,trans);
          free(trans);
          trans = NULL;
          strcat_two_str(tmp,ts, &trans, "/");
          free(ts);

          strcat_two_str(genesl[i]->[it]->name, genesr[j]->transcripts[jt]->name, &ts, ":");
          tmp = malloc(strlen(trans)+1);
          strcpy(tmp,trans);
          free(trans);
          trans = NULL;
          strcat_two_str(tmp,ts, &trans, "/");
          free(ts);


        }
      }
      trans++;
      printf("fusiIs: %s,%s,%s\n",fus,bps,trans);
    }
  }

}



int main(int argc, char ** argv){
  //printf("%d--%s\n",argc,*argv);

  char *fusfi = NULL;
  char *ref = NULL;
  int c;

  int opterr = 0;
  while((c = getopt(argc, argv, "r:f:h")) != -1){
    switch (c){
      case 'r':
        ref = optarg;
        break;
      case 'f' :
        fusfi = optarg;
        break;
      case 'h':
        printf("%s -r -f -h\n",argv[0]);
        exit(-1);
    }
  }
  if (fusfi==NULL || ref==NULL){
    printf("Error: please input gene reference file and target file!\n");
    exit(-1);
  }


  //fp = fopen(ref,"r");
  char * str[100000];
  int len;
  split_str(str,ref,",", &len);
  printf("str length:%d\n",len);
  QGEN_genes gis = qgen_init_genes();
  for (int i=0;i<len;i++){
    printf("reffi--%s\n",str[i]);
    if (access(str[i], F_OK) != -1) {
      printf("[Processing] Reading file %s...\n",str[i]);
      qgen_create_genes_from_file(gis,str[i]);
    }else{
      printf("%s doesn't exists!\n",str[i]);
      free(gis);
      exit(-1);
    }
  }

    //printf("reffi--%s\n",str[i]);
  if (access(fusfi, F_OK) != -1) {
    printf("[Processing] Reading file %s...\n",fusfi);
    int vlen = 2*2*4096;
    FILE * fp = NULL;
    char buff[vlen];
    fp = fopen(fusfi,"r");
    while(fgets(buff, vlen, (FILE*)fp)){
      char * str[100000];
      int len;
      split_str(str, buff,"\t", &len);
      if (str[0][0]=='#'){
        char * des;
        strcat_from_char_array(str, len, &des, "\t");
        des[strlen(des)-1] = 0;
        printf("%s\n",des);
        free(des);
      }else{
        extend_star_fusion(str[5], str[7], gis);
      }
    }

    /*
    //printf("%s++%s++%s\n",str[2],str[1],chrom);
    int rgn;
    QGEN_gene *genes = gis->get_gene_from_coor(gis,chrom,strand,coor,&rgn);
    QGEN_return_gene_from_coor res;
    printf("totoal serach gnee:%d\n",rgn);
    for (int i=0;i<rgn;i++){
      if (genes[i]!=NULL) printf("search gene: %s,%d\n",genes[i]->name,genes[i]->ntx);
      else printf("search gene: Intergenic\n");
      res=genes[i]->return_gene_from_coor(genes[i], chrom,strand,coor);
      printf("ntra:%d\n",res->ntx);
      for (int j=0; j<res->ntx;j++) printf("=-====%d,%ld\n",j,res->disStarts[j]);
      res->print_return_gene_from_coor(res);
    }
    */
  }else{
    printf("Error: %s doesn't exists or can't open\n",fusfi);
    exit(-1);


  }

  return 0;
}
