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
  tx->UTR5Start = -1;
  tx->UTR5End = -1;
  tx->UTR3Start = -1;
  tx->UTR3End = -1;
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

int check_tx_exists(QGEN_gene gi, QGEN_transcript tx){
  QGEN_transcript txo;
  for (int i=0;i<gi->ntx;i++){
    txo = gi->transcripts[i];
    int ident = 1;
    if (txo->exonCount != tx->exonCount) continue;
    for (int j=0; j< txo->exonCount; j++){
      if (txo->exonStarts[j]!=tx->exonStarts[j] || txo->exonEnds[j]!=tx->exonEnds[j]){
        ident = 0;
        break;
      }
    }
    if (ident) return 1;
  }
  return 0;
}
void update_gene_from_tx(QGEN_gene gio, QGEN_transcript tx, QGEN_genes gis){
  //if (strcmp(gi->name,"AKAP17A")==0){
    //printf("differnet chrom: %s, %s,%s,%s\n",gi->name,tx->geneName,gi->chromsome,tx->chromsome);
    //printf("differnet chrom1: %d, %d\n",strcmp(gi->name,tx->geneName),strcmp(gi->chromsome,tx->chromsome));
  //}
  QGEN_gene gi = gio;
  while(gi!=NULL){
    if (strcmp(gi->name,tx->geneName)==0 && strcmp(gi->chromsome,tx->chromsome)==0 && (gi->strand==tx->strand)){
      if(gi->check_tx_exists(gi,tx)) return;
        //if (strcmp(gi->name,"LIN9")==0) printf("tr id:%s\n",tx->name)
      if (gi->ntx >= MAX_TRANSCRIPS_GENE){
        printf("Error: the number of transcripts of each is beyond the default value (200)");
        exit(0);
      }
      if (tx->txStart < gi->txStart ) gi->txStart = tx->txStart;
      if (tx->txEnd > gi->txEnd ) gi->txEnd = tx->txEnd;

      if (tx->cdsStart < gi->cdsStart ) gi->cdsStart = tx->cdsStart;
      if (tx->cdsEnd > gi->cdsEnd ) gi->cdsEnd = tx->cdsEnd;

      if (tx->exonCount > gi->maxExonCount ) gi->maxExonCount = tx->exonCount;
      if (tx->exonCount < gi->minExonCount ) gi->minExonCount = tx->exonCount;
      gi->transcripts[gi->ntx] = tx;
      gi->txNames[gi->ntx] = tx->name;
      gi->ntx++;
      return;
    }
    gi = gi->same;
  }
  QGEN_gene gin = qgen_init_gene();
  qgen_create_gene_from_tx(gin,tx);
  gis->add_gene(gis,gin,0);
  gin->same = gio->same;
  gio->same = gin;
  gi = NULL;
  //printf("%s---%s----%s---%s\n",gio->name,gio->same->name,gio->chromsome,gio->same->chromsome);
  //if (strcmp(gi->name,tx->geneName)==0 & strcmp(gi->chromsome,tx->chromsome)!=0){
    //printf("differnet chrom: %s, %s, %s, %s\n",gi->name,tx->geneName, gi->chromsome, tx->chromsome);
  //}
}

void print_return_gene_from_coor(QGEN_return_gene_from_coor res){
  char *txnames;
  strcat_from_char_array(res->txNames,res->ntx,&txnames,",");
  char *blocks;
  strcat_from_char_array(res->blocks,res->ntx,&blocks,",");
  char *iblocks;
  strcat_from_sint_array(res->iblocks,res->ntx,&iblocks,",");
  char *utr5;
  strcat_from_suint_array(res->inUTR5,res->ntx,&utr5,",");
  char *utr3;
  strcat_from_suint_array(res->inUTR3,res->ntx,&utr3,",");
  char *frames;
  strcat_from_sint_array(res->frames,res->ntx,&frames,",");
  char *diss;
  strcat_from_lint_array(res->disStarts,res->ntx,&diss,",");
  char *dise;
  strcat_from_lint_array(res->disEnds,res->ntx,&dise,",");
  printf("%s\t%s\t%c\t%ld\t%c\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",res->geneName,res->chromsome,res->geneStrand,res->coor,res->strand,txnames,utr5,utr3,blocks,iblocks,frames,diss,dise);
}

QGEN_return_gene_from_coor return_gene_from_coor(QGEN_gene gi, char * chrom, char strand, int coor){
  if (strcmp(chrom,gi->chromsome)!=0){
    return NULL;
  }else{
    if (strand=='+' || strand=='-'){
      if (strand != gi->strand) return NULL;
    }
  }
  if (coor < gi->txStart || coor > gi->txEnd) return NULL;
  QGEN_return_gene_from_coor res = malloc(sizeof(struct qgen_return_gene_from_coor));

  cp_str_malloc(&(res->chromsome), gi->chromsome);
  cp_str_malloc(&(res->geneName), gi->name);
  res->geneStrand = gi->strand;
  res->coor = coor;
  res->strand = strand;
  res->ntx = gi->ntx;
  res->print_return_gene_from_coor = print_return_gene_from_coor;
  QGEN_transcript tx;
  for (int i=0; i<res->ntx;i++){
    cp_str_malloc(&(res->txNames[i]), gi->txNames[i]);
    tx = gi->transcripts[i];
    if (coor>=tx->UTR5Start && coor<=tx->UTR5End) res->inUTR5[i] = 1;
    else res->inUTR5[i] = 0;
    if (coor>=tx->UTR3Start && coor<=tx->UTR3End) res->inUTR3[i] = 1;
    else res->inUTR3[i] = 0;

    cp_str_malloc(&(res->blocks[i]),"intergenic");
    res->iblocks[i] = -1;
    res->disStarts[i] = -1;
    res->disEnds[i] = -1;
    for (int j=0;j<tx->exonCount;j++){
      if (coor >= tx->exonStarts[j] && coor <= tx->exonEnds[j]){
        cp_str_malloc(&(res->blocks[i]),"exon");
        res->iblocks[i] = j+1;
        res->disStarts[i] = coor-tx->exonStarts[j];
        res->disEnds[i] = tx->exonEnds[j]-coor;
      }
    }
    for (int j=0;j<tx->exonCount-1;j++){
      if (coor >= tx->intronStarts[j] && coor <= tx->intronEnds[j]){
        cp_str_malloc(&(res->blocks[i]),"intron");
        res->iblocks[i] = j+1;
        res->disStarts[i] = coor-tx->intronStarts[j];
        res->disEnds[i] = tx->intronEnds[j]-coor;
      }
    }

    //printf("-----------------%ld,%ld,%d,%d,%d,%d,%s,%s\n",tx->UTR5Start,tx->UTR5End,res->ntx,res->inUTR3[i],res->inUTR5[i],res->blocks[i],res->txNames[i]);
    if (res->inUTR3[i] || res->inUTR5[i] || strcmp(res->blocks[i],"intron")==0|| strcmp(res->blocks[i],"intergenic")==0){
      res->frames[i] = -1;
    }
    else{
      int ib = res->iblocks[i]-1;
      int fr = tx->exonFrames[ib];
      if (fr == -1){
        printf("Error: the exon frame is negative\n");
        exit(-1);
      }
      long unsigned int start;
      if (res->strand == '+') start = tx->exonStarts[ib]>=tx->cdsStart ? tx->exonStarts[ib]:tx->cdsStart;
      else{
        if (res->strand == '-') start = tx->exonEnds[ib]<=tx->cdsEnd ? tx->exonEnds[ib]:tx->cdsEnd;
      }
      int dis;
      if (res->strand == '+') dis = coor - start + fr;
      else{
        if (res->strand == '-') dis = start - coor + fr;
      }
      int frame = dis%3+1;
      res->frames[i] = frame;
      //printf("============================%s,%d,%d,%d,%d%d,%d,%s\n",res->blocks[i],res->frames[i],ib,start,fr,dis,frame,res->txNames[i]);
      //printf("============================%d,%d,%s,%d,%d,%d,%d,%s\n",res->inUTR3[i],res->inUTR5[i],res->blocks[i],res->frames[i],ib,start,fr,dis,frame,res->txNames[i]);
    }
    //printf("============%d,%d\n",i,res->ntx);
  }
  //printf("return gene-%s\n",res->geneName);
  return res;
}

QGEN_gene qgen_init_gene(){
  QGEN_gene gi = malloc(sizeof(struct qgen_gene));
  gi->name = NULL;
  gi->chromsome = NULL;
  gi->strand = -1;
  gi->txStart = -1;
  gi->txEnd = -1;
  gi->cdsStart = -1;
  gi->cdsEnd = -1;
  gi->maxExonCount = -1;
  gi->minExonCount = -1;
  gi->ntx = -1;
  gi->next = NULL;
  gi->same = NULL;
  gi->update_gene_from_tx = update_gene_from_tx;
  gi->return_gene_from_coor = return_gene_from_coor;
  gi->check_tx_exists = check_tx_exists;
  return gi;
};

// find the scaffold index based on chrom in genes structure
short int find_sc_index_from_chrom(char * lookup[], short unsigned int nscaffold, char * chrom){
  int index;
  for (index=0;index<nscaffold;index++){
    if (strcmp(lookup[index],chrom)==0) return index;
  }
  return index;
}

//short int add_gene(char * lookup[], char * chrom, QGEN_gene * scaffolds[], short unsigned int nscaffold,  short unsigned int index){
void add_gene(QGEN_genes gis, QGEN_gene gi, int createnew){
  int index = gis->find_sc_index_from_chrom(gis->lookup, gis->nscaffold, gi->chromsome);
  if (index > (MAX_SCAFFOLDS - 1)){
    printf("Error: The number of scaffold is larger than maximal scaffold (1000)\n");
    exit(0);
  }
  //new chromsome
  if (index > (gis->nscaffold - 1)){
    gis->lookup[index] = malloc(strlen(gi->chromsome)+1);
    strcpy(gis->lookup[index],gi->chromsome);
    gis->nscaffold++;
  }

  if (createnew) gis->gName2Gene->put(gis->gName2Gene, gi->name, gi);

  if (gis->scaffolds[index] == NULL){
    gis->scaffolds[index] = gi;
    gis->ngenes[index]++;
  }else{
    QGEN_gene next = NULL;
    next = gis->scaffolds[index];
    if (next->txStart > gi->txStart){
      gi->next = next;
      gis->scaffolds[index] = gi;
      gis->ngenes[index]++;
    }else{
      while (next->next != NULL){
        if (next->next->txStart < gi->txStart) next = next->next;
        else break;
      }
      gi->next = next->next;
      next->next = gi;
      gis->ngenes[index]++;
    }
  }
  //gis->ngenes[index]++;
}

void create_genes_array(QGEN_genes gis){
  for (int i=0; i<gis->nscaffold;i++){
    int ngi = gis->ngenes[i];
    gis->scArray[i] = (QGEN_gene *)malloc(ngi*sizeof(QGEN_gene));
    QGEN_gene next = gis->scaffolds[i];
    for (int j=0; j<gis->ngenes[i];j++){
      gis->scArray[i][j] = next;
      next = next->next;
    }
  }
}

void del_genes(QGEN_genes gis){
  QGEN_gene gi0;
  QGEN_gene gi1;
  for (int i=0; i<gis->nscaffold;i++){
    //release array
    free(gis->scArray[i]);
    gis->scArray[i] = NULL;
    free(gis->lookup[i]);
    gis->lookup[i] = NULL;
    //release gene struct
    gi0 = gis->scaffolds[i];
    gi1 = gi0->next;
    while (gi0 != NULL){
      qgen_del_gene(gi0,1);
      gi0 = gi1;
      if (gi1 != NULL){
        gi1 = gi1->next;
      }
    }
  }
  gi0 = NULL;
  gi1 = NULL;
  gis->gName2Gene->clear;
  free(gis);
}

QGEN_gene get_gene_from_name(QGEN_genes gis, void * key){
  //printf("--%s--%d\n",key,gis->gName2Gene->exists(gis->gName2Gene, key));
  if (gis->gName2Gene->exists(gis->gName2Gene, key)){
    return gis->gName2Gene->get(gis->gName2Gene, key);
  }
  else return NULL;
}

int check_gene_from_coor_strand(char gistrand, char strand){
    if (strand == '+' || strand == '-'){
      if (strand == gistrand) {
        return 1;
      }
    }else{
      return 1;
    }
  return 0;
}

QGEN_gene * get_gene_from_coor_base(QGEN_gene array[], int index, char strand, int coor, int ngene, int *rgn){
  int sumg = 0;
  int igs[1000];
  //forward
  for (int i = index; i< ngene; i++){
    if (array[i]->txStart <= coor && array[i]->txEnd >= coor){
      if (check_gene_from_coor_strand(array[i]->strand, strand)){
        igs[sumg]=i;
        sumg++;
      }
    } else break;
  }
  //back
  for (int i = index-1; i>=0; i--){
    if (array[i]->txStart <= coor && array[i]->txEnd >= coor){
      if (check_gene_from_coor_strand(array[i]->strand, strand)){
        igs[sumg]=i;
        sumg++;
      }
    } else break;
  }
  QGEN_gene * genes = malloc(sizeof(QGEN_gene)*sumg);
  for(int i=0; i< sumg; i++){
    genes[i] = array[igs[i]];
  }
  *(rgn) = sumg;
  return genes;
}
QGEN_gene * get_gene_from_coor_binary(QGEN_genes gis, char * chrom, char strand, int coor, int *rgn){
  int chIndex = gis->find_sc_index_from_chrom(gis->lookup, gis->nscaffold, chrom);
  QGEN_gene *array = gis->scArray[chIndex];
  //printf("chrom is from %d\n",chIndex);
  int low, high, mid;
  low = 0;
  high = gis->ngenes[chIndex]-1;
  while(low<=high){
    mid = (low+high)/2;
    if (array[mid]->txStart <= coor && array[mid]->txEnd >= coor){
      QGEN_gene *p = get_gene_from_coor_base(array,mid,strand, coor,gis->ngenes[chIndex], rgn);
      //printf("total find genes-----%d\n",*rgn);
      //return array[mid];
      return p;
    }
    if (array[mid]->txStart > coor) high = mid-1;
    if (array[mid]->txEnd < coor) low = mid+1;
  }
  return NULL;
}

QGEN_genes qgen_init_genes(){
  QGEN_genes gis = malloc(sizeof(struct qgen_genes));
  gis->nscaffold = 0;
  gis->find_sc_index_from_chrom =  find_sc_index_from_chrom;
  for (int i = 0; i< MAX_SCAFFOLDS; i++){
    gis->ngenes[i] = 0;
    gis->scaffolds[i] = NULL;
  }
  //HashMap gName2Gene = createHashMap(NULL);
  gis->gName2Gene = createHashMap(NULL);

  gis->add_gene = add_gene;
  gis->get_gene_from_name = get_gene_from_name;
  gis->del_genes = del_genes;
  gis->create_genes_array = create_genes_array;
  gis->get_gene_from_coor = get_gene_from_coor_binary;
  return gis;
};

void qgen_del_tx(QGEN_transcript tx){
  free(tx->chromsome);
  tx->chromsome = NULL;

  free(tx->name);
  tx->name = NULL;

  free(tx->geneName);
  tx->geneName = NULL;

  free(tx->exonStarts);
  tx->exonStarts = NULL;

  free(tx->exonEnds);
  tx->exonEnds = NULL;

  free(tx->intronEnds);
  tx->intronEnds = NULL;

  free(tx->intronStarts);
  tx->intronStarts = NULL;

  free(tx->exonFrames);
  tx->exonFrames = NULL;

  tx->next = NULL;

  free(tx);
  tx=NULL;
};

void qgen_del_gene(QGEN_gene gi, int txRelease){
  free(gi->chromsome);
  gi->chromsome = NULL;

  free(gi->name);
  gi->name = NULL;

  for (int i=0; i< gi->ntx;i++) gi->txNames[i] = NULL;

  if (txRelease){
    for (int i=0; i< gi->ntx;i++) qgen_del_tx(gi->transcripts[i]);
  }

  for (int i=0; i< gi->ntx;i++) gi->transcripts[i] = NULL;

  gi->next = NULL;
  gi->same = NULL;
  free(gi);
  gi=NULL;
};

//"sources/20200508_NCBIRef_hg19.txt"
void qgen_create_genes_from_file(QGEN_genes gis, char * file){

  //QGEN_genes gis = qgen_init_genes();
  char wired_chrom_mark = '_';
  static int vlen = 2*2*4096;
  FILE * fp = NULL;
  char buff[vlen];
  fp = fopen(file,"r");
  while(fgets(buff, vlen, (FILE*)fp)){
    char * str[100000];
    int len;
    split_str(str, buff,"\t", &len);
    //if (str[0][0]!='#' & strchr(str[2],wired_chrom_mark)==NULL){
    if (str[0][0]!='#'){
      QGEN_transcript tx = qgen_init_tx();
      qgen_create_tx_from_str(tx, str);
      //add_gene(gis->lookup,gi->chromsome, gis->scaffolds, gis->nscaffold, index);
      QGEN_gene gene = gis->get_gene_from_name(gis, tx->geneName);
      if (gene == NULL){
        QGEN_gene gi = qgen_init_gene();
        qgen_create_gene_from_tx(gi,tx);
        gis->add_gene(gis,gi,1);
      }else{
        gene->update_gene_from_tx(gene,tx,gis);
      }
      gene = NULL;

      //printf("%s-----%s------------,%d--%d--%d\n",tx->name,tx->geneName,gis->find_sc_index_from_chrom(gis->lookup, gis->nscaffold, tx->chromsome),gis->gName2Gene->size, gis->gName2Gene->listSize);
      //printf("%s--\n",gis->get_gene_from_name(gis,gi->name)->name);
      //qgen_del_gene(gi, 1);
      //qgen_del_tx(tx);
    }
    release_split_str(str);
  }
  //HM_entry entry = hm_init_entry();
  gis->create_genes_array(gis);
  /*
  QGEN_gene gene = gis->get_gene_from_name(gis, "RSC1A1");
  while(gene!=NULL){
    printf("Gene is %s-----%s--%ld--%ld\n",gene->name,gene->chromsome,gene->txStart,gene->txEnd);
    gene=gene->same;
  }

  //HashMap gName2Gene = createHashMap(NULL);
  //gName2Gene->put(gName2Gene,"test","test");

  QGEN_gene tmp=gis->scaffolds[0];
  for (int i=0; i<10;i++){
    //printf("chromosome si %s\n",gis->lookup[i]);
    printf("%s---gene is %s\n",tmp->name,gis->scArray[0][i]->name);
    tmp = tmp->next;
  }

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
    //for (int j=0; j< res->ntx;j++){
      //printf("%s-frame is:%d\n",res->txNames[j],res->frames[j]);
    //}
  }
  free(genes);

  printf("size:%d --listsize:%d\n",gis->gName2Gene->size,gis->gName2Gene->listSize);

  int dis[gis->gName2Gene->listSize];
  int sum = 0;
  int max = 0;
  gis->gName2Gene->dis(gis->gName2Gene,dis);
  for (int i=0; i< gis->gName2Gene->listSize;i++){
    //printf("-----%d\n",dis[i]);
    sum += dis[i];
    if (max < dis[i]) max = dis[i];
  }

  printf("max:%d -- sum:%d\n",max,sum);


  gis->del_genes(gis);
  */

  fclose(fp);
  //return gis;
}

//assign the values from ucsc table to transcript
void qgen_create_tx_from_str(QGEN_transcript tx, char * str[]){
  cp_str_malloc(&(tx->chromsome), *(str+2));
  cp_str_malloc(&(tx->name), *(str+1));
  cp_str_malloc(&(tx->geneName), *(str+12));
  tx->strand =  (*(str+3))[0];
  tx->txStart = atoi(*(str+4))+1;
  tx->txEnd = atoi(*(str+5));
  tx->cdsStart = atoi(*(str+6))+1;
  tx->cdsEnd = atoi(*(str+7));
  tx->exonCount = atoi(str[8]);
  if (tx->strand == '+'){
    tx->UTR5Start = tx->txStart;
    tx->UTR5End = tx->cdsStart -1;
    tx->UTR3Start = tx->cdsEnd+1;
    tx->UTR3End = tx->txEnd;
  }else{
    if (tx->strand == '-'){
      tx->UTR3Start = tx->txStart;
      tx->UTR3End = tx->cdsStart -1;
      tx->UTR5Start = tx->cdsEnd+1;
      tx->UTR5End = tx->txEnd;
    }
  }

  char * estr[100000];

  int len;
  split_str(estr, str[9],",",&len);
  init_exon(&(tx->exonStarts),estr,1);
  release_split_str(estr);

  split_str(estr, str[10],",",&len);
  init_exon(&(tx->exonEnds),estr,0);
  release_split_str(estr);

  //printf("trans: %s--%d\n",tx->name,tx->exonCount);
  init_intron(&(tx->intronStarts),tx->exonCount,tx->exonEnds,1);
  init_intron(&(tx->intronEnds),tx->exonCount,&tx->exonStarts[1],-1);

  split_str(estr, str[15],",",&len);
  init_exonFrame(&(tx->exonFrames),estr);
  release_split_str(estr);
}

void qgen_create_gene_from_tx(QGEN_gene gi, QGEN_transcript tx){
  gi->ntx = 1;
  cp_str_malloc(&(gi->chromsome), tx->chromsome);
  cp_str_malloc(&(gi->name),tx->geneName);
  gi->txNames[0] = tx->name;
  gi->strand =  tx->strand;
  gi->txStart = tx->txStart;
  gi->txEnd = tx->txEnd;
  gi->cdsStart = tx->cdsStart;
  gi->cdsEnd = tx->cdsEnd;
  gi->maxExonCount = tx->exonCount;
  gi->minExonCount = tx->exonCount;
  gi->transcripts[0] = tx;
  //gi->transcripts = tx;
}

void * init_exon(long unsigned int **p, char **str, int offset){
  int len = 0;
  while(str[len]!=NULL){
    len++;
  }

  *p = (long unsigned int *)malloc(len*sizeof(long unsigned int));
  long unsigned int *pp = NULL;
  pp = *p;

  for (int i=0;i<len;i++){
    *((*p)++) = atoi(str[i])+offset;
    //printf("----%s---%ld\n",str[i],*((*p)-1));
  }
  *p = pp;
  pp = NULL;
}

void * init_exonFrame(int **p, char **str){
  int len = 0;
  while(str[len]!=NULL){
    len++;
  };

  *p = (int *)malloc(len*sizeof(int));
  int *pp = NULL;
  pp = *p;

  for (int i=0;i<len;i++){
    *((*p)++) = atoi(str[i]);
    //printf("----%s---%ld\n",str[i],*((*p)-1));
  };
  *p = pp;
  pp = NULL;
}

void * init_intron(long unsigned int **p, int elen, long unsigned int *e, int offset){

  if (elen<=1) return NULL;

  int len = elen - 1;

  *p = (long unsigned int *)malloc(len*sizeof(long unsigned int));
  long unsigned int *pp = NULL;
  pp = *p;

  for (int i=0;i<len;i++){
    *((*p)++) = e[i]+offset;
    //printf("----%ld---%ld\n",e[i],*((*p)-1));
  }
  *p = pp;
  pp = NULL;
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
