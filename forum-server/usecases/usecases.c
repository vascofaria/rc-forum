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

char *parse_output_RGR(int error_code) {
    if (error_code == SUCCESS) {
        printf(strcat(RGR, " OK\n"));
    }
    else if (error_code == BAD_INPUT) {
        return strcat(RGR, NOK);
    }
}

char *parse_output_LTR(int error_code) {
    if (error_code == BAD_INPUT) {
        return ERR;
    }
    else if (error_code == SUCCESS) {
        return strcat(LTR, "N");
    }
}