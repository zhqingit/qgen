
//char ** read_parse_file(char *filename, char * delim);

//char ** split_str(char *str, char * delim, int *pc);
//char ** split_str(char ***p, char *str, char * delim, int *pc);
void * split_str(char *p[], char *str, char * delim, int *len);
void release_split_str(char **s);
void cp_str_malloc(char **nstr, char *str);
void strcat_from_char_array(char **src, int len, char **des, char *sep);
void strcat_from_suint_array(short unsigned int src[], int len, char **des, char *sep);
void strcat_from_sint_array(short int src[], int len, char **des, char *sep);
void strcat_from_lint_array(long int src[], int len, char **des, char *sep);
void strcat_two_str(char *src1, char*src2, char **des, char *sep);
