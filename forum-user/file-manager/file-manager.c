#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file-manager.h"

char*
get_file_size(const char *file_name, const char* mode) {
	FILE*    fp;
	long int size;
	char     *size_str;

	if (file_name != NULL) {
		if ((fp = fopen(file_name, mode)) != NULL) {
			
			fseek(fp, 0L, SEEK_END);
			size = ftell(fp);
			fclose(fp);

			size_str = (char*) malloc ((sizeof(size) / sizeof(long int)) + sizeof(char));

			if (size_str != NULL) {
				sprintf(size_str, "%ld", size);
				return size_str;
			}
		}
	} 
	return NULL;
}

char*
get_file_data(const char* file_name, const char* mode) {
	FILE*    fp;
	long int size;
	char*    data;

	if (file_name != NULL) {
		if ((fp = fopen(file_name, mode)) != NULL) {
			
			fseek(fp, 0L, SEEK_END);
			size = ftell(fp); 
			fseek(fp, 0L, SEEK_SET);

			data = (char*) malloc (sizeof(char) * (size + 1));

			if (data != NULL) {
				fread(data, sizeof(char), size + 1 ,fp);
				fclose(fp);
				return data;
			}
		}
	}
	return NULL;
}	

char*
get_file_ext(const char* file_name) {
	char  *ext, *ext_aux;
	ext_aux = strchr(file_name, '.');
	ext     = (char *) malloc (sizeof(char) * (strlen(ext_aux) + 1));
	strcpy(ext, ext_aux);
	return ext;
}