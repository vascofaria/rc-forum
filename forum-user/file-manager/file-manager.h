#ifndef __FILE_MANHAGER_H__
#define __FILE_MANHAGER_H__

char* 
get_file_size(const char *file_name, const char* mode);

char*
get_file_data(const char *file_name, const char* mode);

char*
get_file_ext(const char *file_name);

void
write_to_tcp_socket(int sock_tcp, char *buffer);

int 
read_from_tcp_socket(int sock_tcp, char *buffer, int size, char final_char);

void 
write_from_socket_to_file(int sock_tcp, char *file_path, int file_size);

#endif