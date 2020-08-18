# qgen
## A tool to annotate the coordinate. It is developed to easily get all gene informaiton from one coordinate.

#### Usage: ./qgen -f reference_file -i coordinates
- -f: the ucsc table formatted reference files (separated by ,)
- -i: the coordinates information (chr:coordinate:strand); the strand can be '*' if you are not sure about it. Multiple coordinates can be seperated by , or in a list file

#### example: 
1. ./qgen -r example/20200508_NCBIRef_hg19_test.txt -i chr1:22021559:-
1. ./qgen -r example/20200508_NCBIRef_hg19_test.txt -i example/test.txt



#### Output:
 - Gene	: GeneName
 - Chromsome: Chromsome
 - Strand: Strand from gene
 - Coordinate: Coordinate
 - InputStrand: Strand from the input
 - Transcripts: all transcripts
 - UTR5: if the coordinate is in the UTR5 (following the transcripts orders, separated by ,)
 - UTR3: if the coordinate is in the UTR3 (following the transcripts orders, separated by ,)
 - Block: the block that the coordinate locates at, intron, exon, or intergenic (following the transcripts orders, separated by ,)
 - BlockIndex: the block index that the coordinate locates at (following the transcripts orders, separated by ,)
 - Frame: the frame infomation of the block that the coordinate locates at (following the transcripts orders, separated by ,)
 - DisToStart: the distance to the block start of the coordinate (following the transcripts orders, separated by ,)
 - DisToEnd: the distance to the block end of the coordinate (following the transcripts orders, separated by ,)



#### todo list:
1. compatible to the gtf format
1. infer the frame from two coordinates
1. Extract the mRNA or DNA sequece around a coordiante
