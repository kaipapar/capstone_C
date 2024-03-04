#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "csvparser.h"
#include "csvwriter.h"

#define Y 450
#define X 5
#define KWLEN 50

int parser(char ***cleaned, char* file_in);
char **remove_dupes(const char **cleaned);
int sort(char ***cleaned);
int compare(const void *a, const void *b);
int writer(char **cleaned, char* file_out);
void EC(void *pointer);

int main() {
    char* file_in = "output.csv";
    char* file_out = "KWs_formatted.csv";

    char **cleaned = calloc(Y, sizeof(char *)); // to keep formatted keywords from parser

    EC((char **)cleaned);

    for (int i=0; i < Y; i++){
        cleaned[i] = calloc(KWLEN, sizeof(char)); //allocate x columns
        EC((char *) cleaned[i]);
    }

    parser(&cleaned, file_in);
    sort(&cleaned);
    char **no_dupes = remove_dupes((const char **)cleaned);
    writer(no_dupes, file_out);

    for (int i = 0; i<Y; i++){
        free(cleaned[i]);
        free(no_dupes[i]);
    }
    free(cleaned);
    free(no_dupes);

    return 0;
}

int parser(char ***cleaned, char* file_in) {
    int i =  0;
    int row_cnt = 0;
    int cnt = 1; // start from one because of header
    //                                   file, delimiter, first_line_is_header?
    CsvParser *csvparser = CsvParser_new("output.csv", ",", 1);

    CsvRow *row;
    const CsvRow *header = CsvParser_getHeader(csvparser);

    if (header == NULL) {
        printf("%s\n", CsvParser_getErrorMessage(csvparser));
        return 1;
    }
    const char **headerFields = CsvParser_getFields(header);
    for (i = 0 ; i < CsvParser_getNumFields(header) ; i++) {
        printf("TITLE: %s\n", headerFields[i]);
    }
    while ((row = CsvParser_getRow(csvparser)) ) {
    	printf("==ROW: %d==\n", row_cnt);
        row_cnt++;
        const char **rowFields = CsvParser_getFields(row);
        for (i = 0 ; i < CsvParser_getNumFields(row) ; i++) {
            if (rowFields[i][0] != '\0'){
                printf("FIELD %d: %s\n", cnt, rowFields[i]);
                // add keyword to its own row if its not a dupe
                //if (dupe(*cleaned, rowFields[i]) == 0){
                    strcpy((*cleaned)[cnt], rowFields[i]);
                    printf("cleaned: %s\n", (*cleaned)[cnt]);
                    cnt++;   
                //}
            }
        }
		printf("\n");
        CsvParser_destroy_row(row);
    }
    CsvParser_destroy(csvparser);
    return 0;
}
/* returns array of strings with no duplicates */
char **remove_dupes(const char **cleaned){
    char **no_dupes = calloc(Y, sizeof(char *));
    int cnt = 0;
    EC((char **)no_dupes);
    for (int i=0; i < Y; i++){
        no_dupes[i] = calloc(KWLEN, sizeof(char)); //allocate x columns
        EC((char *) no_dupes[i]);
    }
    printf("check: %s\n",cleaned[0]);
    for (int i = 0; i < Y-1; i++){
        if (cleaned[i] == NULL || cleaned[i+1] == NULL){
            printf("check: %d \n", i);
            continue;            
        } else if (compare(&cleaned[i], &cleaned[i+1]) != 0){
            // no_dupes[cnt] = cleaned[i];
            strcpy(no_dupes[cnt], cleaned[i]);
            printf("nodupes: %d, %s \n",cnt,no_dupes[cnt]);
            cnt++;
        }
    }


    return no_dupes;
}


int sort(char ***cleaned) {
    qsort(*cleaned, Y, sizeof(char *), compare);
    return 0;
}
int compare(const void *a, const void *b) {
    return strcmp((*(const char**)a), (*(const char**)b));
}


int writer(char **cleaned, char* file_out) {
	
	CsvWriter *csvWriter = CsvWriter_new(file_out, ",", 0);  
    char *header[] = {"Keywords", "Category[0-4]", "what", "else", "could here lay"};
   
    for (int i = 0 ; i < Y ; i++) {    
		for (int j = 0 ; j < X ; j++) {
            if(i==0){
                if (CsvWriter_writeField(csvWriter, header[j])) {    
                    printf("Error: %s\n", CsvWriter_getErrorMessage(csvWriter));    
                    return 1;   
                }
            }  
            if (j==0) {
                if (CsvWriter_writeField(csvWriter, cleaned[i])) {    
                    printf("Error: %s\n", CsvWriter_getErrorMessage(csvWriter));    
                    return 1;    
                }                    
            } else {
                if (CsvWriter_writeField(csvWriter, "\0")) {    
                    printf("Error: %s\n", CsvWriter_getErrorMessage(csvWriter));    
                    return 1;    
                }                             
            }

		}    
		CsvWriter_nextRow(csvWriter);    
	}    
	CsvWriter_destroy(csvWriter);   
    return 0;
}


/**
 * Error Checking function for pointers, use with all mem allocation
*/
void EC(void *pointer)
{
	if (pointer == NULL)
	{
		fprintf(stderr, "Error, contiguous memory allocation failed");
		exit(-1);
	}
}