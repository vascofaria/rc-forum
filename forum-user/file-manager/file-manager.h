#ifndef __FILE_MANHAGER_H__
#define __FILE_MANHAGER_H__

char* 
get_file_size(const char *file_name, const char* mode);

char*
get_file_data(const char *file_name, const char* mode);

char*
get_file_ext(const char *file_name);

int 
write_to_tcp_socket(int socket_tcp, char *buffer, char final_char);

int 
read_from_tcp_socket(int socket_tcp, char *buffer, int size, char final_char);

int 
write_from_socket_to_file(int sock_tcp, char *file_path, int file_size);

int 
write_from_file_to_socket(int sock_tcp, char *file_path, int file_size);

#endif