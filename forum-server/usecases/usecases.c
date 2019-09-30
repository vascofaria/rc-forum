/* _______________________________________________________________________________
 * »»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
 * IST - RC 2019/2020
 *
 * usecases.c
 *
 * -------------------------------------------------------------------------------
 *
 * Made by: Bruno Meira (89421), Nikoletta Matsur (89513) and Vasco Faria (89559)
 *
 * «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
 */

#include <string.h>
#include "usecases.h"
#include "../exceptions.h"
#include "../protocol-manager/protocol.h"
#include "../file-manager/file-manager.h"

#define MAX_STATUS_RESPONSE 9
#define MAX_TOPIC_LIST_RESPONSE 1100 
//(10+1+5)*99 + 99*1


//TODO fazer mallocs em todos os sitios onde se aloca a memoria estaticamente

char *parse_output_RGR(int error_code) {
    char res[MAX_STATUS_RESPONSE];
    strcpy(res, RGR);
    if (error_code == SUCCESS) { 
        return strcat(res, OK);
    }
    else if (error_code == BAD_INPUT) {
        return ERR;
    }
}

char *parse_output_LTR(int error_code) {
     char res[MAX_STATUS_RESPONSE];
    if (error_code == BAD_INPUT) {
        return ERR;
    }
    else if (error_code == SUCCESS) {
        char **dir_list;
        int res = get_topics(&dir_list);
        if (res == SUCCESS) {
            int num = 0;
            char dirs[MAX_TOPIC_LIST_RESPONSE];
            char ret_dirs[MAX_TOPIC_LIST_RESPONSE + 6];
            for(num = 0; !strcmp(dir_list[num],"(null)"); num++){
                strcat(dirs, " ");
                strcat(dirs, dir_list[num]);
            }
            strcat(ret_dirs, LTR);
            strcat(ret_dirs, " ");
            strcat(ret_dirs, num);
            strcat(ret_dirs, " ");
            strcat(ret_dirs, dir_list);
            return ret_dirs;
        }
    } else {
        return ERR;
    }
}

char *parse_output_PTR(int error_code, char* userID, char* topic){
    if (error_code == BAD_INPUT){
        return ERR;
    }
    int error_code = post_topic(topic, userID);
    char response[MAX_STATUS_RESPONSE];
    strcat(response, PTR);
    if (error_code == TOPIC_ALREADY_EXISTS){
        strcat(response, DUP);
        return response;
    }
    else if (error_code == FAILURE){
        return strcat(response, NOK);
    }
    else if (error_code == TOPICS_FULL){
        return strcat(response, FUL);
    }
    else if (error_code == SUCCESS){
        return strcat(response, OK);
    }
}

char *parse_output_LQR(char* topic, int error_code){
    char response[MAX_STATUS_RESPONSE];
    strcat(response, LQR);
    if (error_code == BAD_INPUT){
        return ERR;
    }
    else if (error_code == SUCCESS){
        char **quest_list;
        int res = get_topics(&quest_list);
        if (res == SUCCESS) {
            int num = 0;
            error_code = get_questions(topic, &quest_list);
            char questions[MAX_TOPIC_LIST_RESPONSE];
            char ret_quest[MAX_TOPIC_LIST_RESPONSE + 6];
            if (error_code == SUCCESS){
                for(num = 0; !strcmp(quest_list[num],"(null)"); num++){
                    strcat(questions, " ");
                    strcat(questions, quest_list[num]);
                }
            }
            strcat(ret_quest, LQR);
            strcat(ret_quest, " ");
            strcat(ret_quest, num);
            strcat(ret_quest, " ");
            return ret_quest;
        }
    } else if (error_code == TOPIC_DOESNT_EXIST){
        strcat(response, LQR);
        return strcat(response, NOK);
    }
}

// --------------------------- tcp -------------------------------



