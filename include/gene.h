#include <stdbool.h>
#include "./hashmap.h"

//regular chromsome gene numbers
//#define GENES_REG_CHROM 30000
//un-regular chromsome gene numbers
//#define GENES_NREG_CHROM 1000

//max transcripts number of each gene
#define MAX_TRANSCRIPS_GENE 200

//max scaffold number
#define MAX_SCAFFOLDS 1000



typedef struct qgen_block{
  char * type;
  long unsigned int Start;
  long unsigned int End;
  long unsigned int Length;
}*QGEN_block;

//transcript structure
typedef struct qgen_transcript{
  char * geneName;
  char * name;
  char * chromsome;
  char strand;
  long unsigned int txStart;
  long unsigned int txEnd;
  long unsigned int cdsStart;
  long unsigned int cdsEnd;
  long unsigned int UTR5Start;
  long unsigned int UTR5End;
  long unsigned int UTR3Start;
  long unsigned int UTR3End;
  long unsigned int * exonStarts;
  long unsigned int * exonEnds;
  long unsigned int * intronStarts;
  long unsigned int * intronEnds;
  unsigned short int exonCount;
  int * exonFrames;
  char * cdsStartStat;
  char * cdsEndStat;
  struct qgen_stranscript * next; // for chain function
}*QGEN_transcript;


typedef struct qgen_return_gene_from_coor *QGEN_return_gene_from_coor;

typedef void (*PrintReturnGeneFromCoor)(QGEN_return_gene_from_coor res);

typedef struct qgen_return_gene_from_coor{
  char * geneName;
  char * chromsome;
  char geneStrand;
  char strand;
  long unsigned int coor;

  char *txNames[MAX_TRANSCRIPS_GENE];
  char *blocks[MAX_TRANSCRIPS_GENE];
  short int iblocks[MAX_TRANSCRIPS_GENE];
  short unsigned int ntx;
  short unsigned int inUTR5[MAX_TRANSCRIPS_GENE];
  short unsigned int inUTR3[MAX_TRANSCRIPS_GENE];

  short int frames[MAX_TRANSCRIPS_GENE];
  long int disStarts[MAX_TRANSCRIPS_GENE];
  long int disEnds[MAX_TRANSCRIPS_GENE];
  PrintReturnGeneFromCoor print_return_gene_from_coor;

}*QGEN_return_gene_from_coor;

static void print_return_gene_from_coor(QGEN_return_gene_from_coor res);


typedef struct qgen_gene *QGEN_gene;
typedef struct qgen_genes *QGEN_genes;

//add new transcript to gene
typedef void(*UpdateGene)(QGEN_gene gi, QGEN_transcript tx, QGEN_genes gis);
typedef QGEN_return_gene_from_coor(*ReturnGeneFromCoor)(QGEN_gene gi, char * chrom, char strand, int coor);
typedef int (*TxExists)(QGEN_gene gi, QGEN_transcript tx);
//gene structure
typedef struct qgen_gene{
  char * name;
  char * chromsome;
  char *txNames[MAX_TRANSCRIPS_GENE];
  char strand;
  long unsigned int txStart;
  long unsigned int txEnd;
  long unsigned int cdsStart;
  long unsigned int cdsEnd;
  short unsigned int maxExonCount;
  short unsigned int minExonCount;
  short unsigned int ntx;
  //struct qgen_transcript * (*transcripts); //transcripts list
  QGEN_transcript transcripts[MAX_TRANSCRIPS_GENE]; //transcripts list
  UpdateGene update_gene_from_tx;
  ReturnGeneFromCoor return_gene_from_coor;
  TxExists check_tx_exists;
  struct qgen_gene * next; // for chain function
  struct qgen_gene * same; // same name but different chromsome
}*QGEN_gene;

static void update_gene_from_tx(QGEN_gene gi, QGEN_transcript tx, QGEN_genes gis);
static QGEN_return_gene_from_coor return_gene_from_coor(QGEN_gene gi, char * chrom, char strand, int coor);
static int check_tx_exists(QGEN_gene gi, QGEN_transcript tx);


//short int find_sc_index_from_chrom(char * lookup, short unsigned int nscaffold, char * chrom);
typedef void(*AddGene)(QGEN_genes gis, QGEN_gene gi, int createnew);
typedef void(*CreateGenesArray)(QGEN_genes gis);
typedef QGEN_gene(*GetGenefromName)(QGEN_genes gis, void * key);
//typedef QGEN_gene(*GetGenefromCoor)(QGEN_genes gis, char * chrom, char strand, int coor, int *rgn);
typedef QGEN_gene*((*GetGenefromCoor)(QGEN_genes gis, char * chrom, char strand, int coor, int *rgn));
typedef void(*DelGenes)(QGEN_genes gis);

typedef struct qgen_genes{
  QGEN_gene scaffolds[MAX_SCAFFOLDS];
  QGEN_gene *scArray[MAX_SCAFFOLDS]; //scallfold Array
  char *lookup[MAX_SCAFFOLDS];
  short unsigned int ngenes[MAX_SCAFFOLDS];
  short unsigned int nscaffold;
  short int (*find_sc_index_from_chrom)(char * lookup[], short unsigned int nscaffold, char * chrom);
  AddGene add_gene;
  CreateGenesArray create_genes_array;
  GetGenefromName get_gene_from_name;
  GetGenefromCoor get_gene_from_coor;
  DelGenes del_genes;
  HashMap gName2Gene;
}*QGEN_genes;



//handle transcript structure
QGEN_transcript qgen_init_tx();
void qgen_del_tx(QGEN_transcript tx);

//handle gene structure
QGEN_gene qgen_init_gene();
void qgen_del_gene(QGEN_gene gi, int txRelease);

//handle a set of gene structure (QGEN_genes)
QGEN_genes qgen_init_genes();
static void add_gene(QGEN_genes gis, QGEN_gene gi, int createnew);
static void create_genes_array(QGEN_genes gis);
static QGEN_gene get_gene_from_name(QGEN_genes gis, void * key);
static QGEN_gene * get_gene_from_coor_binary(QGEN_genes gis, char * chrom, char strand, int coor, int *rgn);
static void del_genes(QGEN_genes gis);
static QGEN_gene * get_gene_from_coor_base(QGEN_gene array[], int index, char strand, int coor, int ngene, int *rgn);
static int check_gene_from_coor_strand(char gistrand, char strand);

//void qgen_del_gene(QGEN_gene gi, int txRelease);

void qgen_create_genes_from_file(QGEN_genes gis, char * file);
//void qgen_create_tx_from_str(QGEN_transcript tx, char ** str);
//void qgen_create_tx_from_str(QGEN_transcript tx, char * str);
void qgen_create_tx_from_str(QGEN_transcript tx, char * str[]);
void qgen_create_gene_from_tx(QGEN_gene gi, QGEN_transcript tx);
//initial exon start and end
void * init_exon(long unsigned int **p, char **str, int offset);
void * init_exonFrame(int **p, char **str);
void * init_intron(long unsigned int **p, int elen, long unsigned int *e, int offset);


//functions

/*
//init transcript
//#define newTx() (qgen_transcript)(malloc(sizeof(struct qgen_transcript)));
typedef qgen_transcript(*qgen_init_tx)();
//init gene
//#define newGene() (qgen_gene)(malloc(sizeof(struct qgen_gene)));
typedef qgen_gene(*qgen_init_gene)();

//get transcript from gene based on name
typedef qgen_transcript(*qgen_get_tx_from_gene)(qgen_gene * gene, char * name);

//get the block from transcript
typedef qgen_block(*qgen_get_block_from_tx)(qgen_transcript * tran, int indexRNA, char * type);
//get the exon from transcript
typedef qgen_block(*qgen_get_exon_from_tx)(qgen_transcript * tran, int indexRNA);
//get the intron from transcript
typedef qgen_block(*qgen_get_intron_from_tx)(qgen_transcript * tran, int indexRNA);
*/
