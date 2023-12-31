#ifndef CSV_H
#define CSV_H

#ifndef MAX_STR_SIZE
#define MAX_STR_SIZE 256
#endif

typedef struct{
	const char * filepath;
	char separator;
	int rows, cols;
} csv_t;

void csv_init(csv_t * csv, const char * FILEPATH, char separator);
char ** read_row_from_csv(csv_t * csv, int row_index);

#endif
#ifndef CSV_SRC
#define CSV_SRC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void csv_init(csv_t * csv, const char * FILEPATH, char separator){
	if(!csv || !FILEPATH)
		return;

	csv->filepath = FILEPATH;
	// we assume that there is at least one row and column
	// but no more than that
	csv->rows = 1;
	csv->cols = 1;
	csv->separator = (separator ? separator : ',');

	FILE * fp = fopen(FILEPATH, "r");
	char * line = NULL;
	size_t line_len = 0;

	getline(&line, &line_len, fp);

	strtok(line, &separator);
	while(strtok(NULL, &separator))
		csv->cols++;

	while(getline(&line, &line_len, fp) != -1)
		csv->rows++;

	if(line) 
		free(line);
}

char ** read_row_from_csv(csv_t * csv, int row_index){
	char ** str_arr = (char ** )calloc(csv->cols * MAX_STR_SIZE, sizeof(char));

	if((row_index + 1) > csv->rows)
		return NULL;
	
	FILE * fp = fopen(csv->filepath, "r");
	char * line = NULL;
	size_t line_len = 0;

	for(int i = 0; i < row_index; i++)
		getline(&line, &line_len, fp);

	char * current_str;
	for(int c = 0; c < csv->cols; c++){
		if(c == 0)
			current_str = strtok(line, &csv->separator);
		else
			current_str = strtok(NULL, &csv->separator);
		str_arr[c] = current_str;
	}

	return str_arr;
}

#endif
