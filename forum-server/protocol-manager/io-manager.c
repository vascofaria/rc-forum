/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * io-manager.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#include <stdbool.h>


// int parseargs(char *arguments){
//     char *parsed_arguments;

// }

typedef struct node{
    char *string;
    struct node* next;
} node_t;


node_t* create_table(){
    node_t *head = NULL;
    head = (node_t *) malloc(sizeof(node_t)); //TODO
    head->string = NULL;
    head->next = NULL;
    return head;
}

void add_to_table(node_t *table, char* argument) {
    if (table->string == NULL) {
        table->string = argument;
    }
    else {
        node_t *n = table;
        node_t *new = (node_t *) malloc(sizeof(node_t)); //TODO
        
        while (n->next != NULL) {
            n = n->next;
        }
        new->string = argument;
        new->next = NULL;
        n->next = new;
    }
}

void print_table(node_t *n) {
    while (n != NULL) {
        printf("%s\n", n->string);
        n = n->next;
    }
}

char* handle_reg(node_t *n) {
    char *ptr;
    long number = strtol(n->string, &ptr, 10);
    if (strlen(n->string) != 5 || number == 0 || strlen(number) != 5 || n->next != NULL) { //strtol returns long

        printf("RGR NOK\n");
        return "RGR NOK";
    } else {
         printf("RGR OK\n");
        return "RGR OK";
    }

}

char* handle_request(char *arguments){
    printf("olha so: %s\n", arguments);
    char delim[] = " ";
    node_t* table;
    char *buf = strtok(arguments, delim);

    table = create_table();

    while (buf != NULL) {
        add_to_table(table, buf);
        buf = strtok(NULL, delim);
    }

    // print_table(table);

    if (strlen(table->string) != 3) {
        return "ERR";
    }
    else {
        if (!strcmp(table->string, "REG")) {
            return handle_reg(table->next);
        }
    }
    // printf("R %s\n", arguments[0]);
    
    // if(!strcmp(&arguments[0],"R") && !strcmp(&arguments[1],"E") && strcmp(&arguments[2],"G")){
    //     write(1,"SO YOU TRYING TO REGISTER", 255);
    // }
}

