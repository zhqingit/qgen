#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "include/qgen.h"


void print_genes(QGEN_genes gis ,char * chrom, long unsigned int coor, char strand ){
    int rgn;
    QGEN_gene *genes = gis->get_gene_from_coor(gis,chrom,strand,coor,&rgn);
    QGEN_return_gene_from_coor res;
    //printf("totoal serach gnee:%d\n",rgn);
    for (int i=0;i<rgn;i++){
      //if (genes[i]!=NULL) printf("search gene: %s,%d\n",genes[i]->name,genes[i]->ntx);
      //else printf("search gene: Intergenic\n");
      res=genes[i]->return_gene_from_coor(genes[i], chrom,strand,coor);
      //printf("ntra:%d\n",res->ntx);
      //for (int j=0; j<res->ntx;j++) printf("=-====%d,%ld\n",j,res->disStarts[j]);
      res->print_return_gene_from_coor(res);
    }
}
void print_genes_from_file(QGEN_genes gis,char * file){
  int vlen = 2*2*4096;
  FILE * fp = NULL;
  char buff[vlen];
  fp = fopen(file,"r");
  printf("#Gene\tChromsome\tStrand\tCoordinate\tInputStrand\tTranscripts\tUTR5\tUTR3\tBlock\tBlockIndex\tFrame\tDisToStart\tDisToEnd\n");
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



int main(int argc, char ** argv){
  //printf("%d--%s\n",argc,*argv);

  char *input = NULL;
  char *ref = NULL;
  int c;

  int opterr = 0;
  while((c = getopt(argc, argv, "r:i:h")) != -1){
    switch (c){
      case 'r':
        ref = optarg;
        break;
      case 'i' :
        input = optarg;
        break;
      case 'h':
        printf("%s -r -i -h\n",argv[0]);
        exit(-1);
    }
  }
  if (input==NULL || ref==NULL){
    printf("Error: please input gene reference file and target file!\n");
    printf("qgen -r reference -i input\n");
    printf("-r: reference file (uscs table format)\n");
    printf("-i: input\n");
    printf("Example: ./qgen -r example/20200508_NCBIRef_hg19_test.txt -i chr1:22021559:-\n");
    printf("Example: ./qgen -r example/20200508_NCBIRef_hg19_test.txt -i example/test.txt\n");


    exit(-1);
  }


  //fp = fopen(ref,"r");
  char * str[100000];
  int len;
  split_str(str,ref,",", &len);
  //printf("str length:%d\n",len);
  QGEN_genes gis = qgen_init_genes();
  for (int i=0;i<len;i++){
    //printf("reffi--%s\n",str[i]);
    if (access(str[i], F_OK) != -1) {
      printf("[Processing] Reading file %s...\n",str[i]);
      qgen_create_genes_from_file(gis,str[i]);
    }else{
      printf("%s doesn't exists!\n",str[i]);
      free(gis);
      exit(-1);
    }
  }

  split_str(str,input,",", &len);
    //printf("reffi--%s\n",str[i]);
  if (access(input, F_OK) != -1) {
    printf("[Processing] Reading file %s...\n",input);
    print_genes_from_file(gis,input);
  }else{
    split_str(str,input,":", &len);
    if (len!=3){
      printf("Error: please input chr:coordinate:strand or a file with same format\n");
      exit(-1);
    }

    int coor = atoi(str[1]);
    char strand = str[2][0];
    char * chrom = str[0];
    //printf("%s++%s++%s\n",str[2],str[1],chrom);
    int rgn;
    QGEN_gene *genes = gis->get_gene_from_coor(gis,chrom,strand,coor,&rgn);
    QGEN_return_gene_from_coor res;
    printf("#Total find gene:%d\n",rgn);
    for (int i=0;i<rgn;i++){
      if (genes[i]!=NULL) printf("#Find gene: %s; Transcript number: %d\n",genes[i]->name,genes[i]->ntx);
      else printf("#Find gene: Intergenic\n");
      res=genes[i]->return_gene_from_coor(genes[i], chrom,strand,coor);
      //printf("ntra:%d\n",res->ntx);
      //for (int j=0; j<res->ntx;j++) printf("=-====%d,%ld\n",j,res->disStarts[j]);
      printf("#Gene\tChromsome\tStrand\tCoordinate\tInputStrand\tTranscripts\tUTR5\tUTR3\tBlock\tBlockIndex\tFrame\tDisToStart\tDisToEnd\n");
      res->print_return_gene_from_coor(res);
    }

  }

/*
  int coor = 226488904;
  int strand = '-';
  char * chrom = "chr1";
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

  //fclose(fp);
  /*
  char *s = "test";
  printf("hellow world!%s\n",s);

  HashMap map = createHashMap(NULL, NULL);
  //map->put(map, "asdfasdf", "asdfasdfds");
  Put(map, "asdfasdf", "asdfasdfds");
  printf("---%d\n",map->size);
  //TEST test = malloc(sizeof(struct tEST));
  //test->put = defaultPut;
  //test->put(map, "asdfasdf", "asdfasdfds");

  Put(map, "sasdasd", "asdfasdfds");
  Put(map, "asdhfgh", "asdfasdfds");
  Put(map, "4545", "asdfasdfds");
  Put(map, "asdfaasdasdsdf", "asdfasdfds");
  Put(map, "asdasg", "asdfasdfds");
  Put(map, "qweqeqwe", "asdfasdfds");

  printf("key: 4545, exists: %s\n", Existe(map, "4545") ? "true" : "false");
  printf("4545: %s\n", Get(map, "4545"));
  printf("remove 4545 %s\n", Remove(map, "4545") ? "true" : "false");
  printf("remove 4545 %s\n", Remove(map, "4545") ? "true" : "false");
  printf("key: 4545, exists: %s\n", Existe(map, "4545") ? "true" : "false");
  map->clear(map);
  */

  //QGEN_transcript tx = qgen_init_tx();
  //printf("test gene:%ld\n",tx->txStart);
  //free(tx);

  //qgen_create_gene_from_file("sources/20200508_NCBIRef_hg19_test.txt");


  /*
  FILE * fp = NULL;

  char buff[512];
  fp = fopen("sources/20200508_NCBIRef_hg19.txt","r");
  fscanf(fp, "%s", buff);
  printf("1: %s\n", buff );

  fgets(buff, 512, (FILE*)fp);
  printf("2: %s\n", buff );

  fgets(buff, 512, (FILE*)fp);
  printf("3: %s\n", buff );

  int length;
  int *pc = &length;
  //*pc = 21;

  char ** str = split(buff,"\t",pc);
  release_split(str,pc);
  printf("%d---\n",*pc);
  for (int i=0; i<20;i++){
    printf("res:%s,%d,%p\n",*(str+i),i,str[i]);
  }
  release_split(str,pc);

  printf("%d+++++++++++++++++\n",*pc);
  for (int i=0; i<20;i++){
    printf("res:%s============%d,%p\n",*(str+i),i,str[i]);
  }
  */

  /*
  char * dest[10000];
  char * tmp;
  int pc = 0;
  char * ch="\t";
	char * ptr = strtok(buff, ch); // 实现字符串的分割
	while(ptr != NULL)
	{
		printf("%d--\n", pc);
    dest[pc] = malloc(strlen(ptr)+1);
    strcpy(dest[pc],ptr);
    //memset(dest[pc++],ptr,strlen(ptr)+1);
    printf("%s-----%s\n",ptr,dest[pc]);

		ptr = strtok(NULL, ch);
		printf("%s==%d\n",ptr, pc);
    pc++;
	}
  fclose(fp);
  for (int i=0;i<pc;i++){
    free(dest[i]);
  }
  */
  //printf("===========\n");




  /*

  HashMapIterator iterator = createHashMapIterator(map);
  while (hasNextHashMapIterator(iterator)) {
      iterator = nextHashMapIterator(iterator);
      printf("{ key: %s, key: %s, hashcode: %d }\n",
          (char *)iterator->entry->key, (char *)iterator->entry->value, iterator->hashCode);
  }
  freeHashMapIterator(&iterator);
  */


  return 0;
}
