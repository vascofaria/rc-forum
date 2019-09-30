#ifndef __FILE_MANHAGER_H__
#define __FILE_MANHAGER_H__

char* 
get_file_size(const char *file_name, const char* mode);

char*
get_file_data(const char *file_name, const char* mode);

char*
get_file_ext(const char *file_name);

#endif