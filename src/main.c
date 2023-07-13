/**
 * @file main.c
 * Demo application to demonstrate usage of the WY_IniMgr library. 
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "WY_IniMgr.h"


int main(int argc, char *argv[])
{
    char var[32];
    char num_val[32];
    char *val;

    wyini_init();
    if(wyini_open("inifile", 1024) != 0) {/* Open the file and copy the content. */
        printf("Error reading file.\n");
        return 0;
    }

    strncpy(var, "VAR_2", strlen("VAR_2")+1);
    if(wyini_get_var_val(var, &val) == 0) /* Read a var. */
        printf("VAR:%s | VAL:%s\n", var, val);
    else
        printf("VAR:%s not found\n", var);

    strncpy(var, "VAR_3", strlen("VAR_3")+1);
    if(wyini_get_var_val(var, &val) == 0) /* Read another var. */
        printf("VAR:%s | VAL:%s\n", var, val);
    else
        printf("VAR:%s not found\n", var);

    strncpy(var, "VAR_4", strlen("VAR_4")+1);
    if(wyini_get_var_val(var, &val) == 0) /* Read another var. */
        printf("VAR:%s | VAL:%s\n", var, val);
    else
        printf("VAR: %s not found\n", var);

    strncpy(var, "NUM_VAR_1", strlen("NUM_VAR_1")+1);
    if(wyini_get_var_val(var, &val) == 0) /* Read another var. */
        printf("VAR:%s | VAL:%s\n", var, val);
    else {
        printf("VAR:%s not found\n", var);
        printf("This should not happen in this demo. Check inifile. Exiting.\n");
        goto do_exit;
    }

    snprintf(num_val, 32, "%d", atoi(val) + 1); /* Increment the number read from "NUM_VAR_1" by 1. */
    if(wyini_write_val(var, num_val) != 0) {
        printf("Write VAR:%s to VAL%s failed.\n", var, val);
        printf("This should not happen in this demo. Exiting.\n");
        goto do_exit;
    } else 
        printf("Wrote VAL:%s to VAR:%s\n", num_val, var);

    if(wyini_get_var_val(var, &val) == 0) /* Let's read "NUM_VAR_1" after we overwrote the value. */
        printf("VAR:%s | VAL:%s\n", var, val);
    else {
        printf("VAR:%s not found\n", var);
        printf("This should not happen in this demo - write was unsuccessful. Exiting.\n");
        goto do_exit;
    }

    if(wyini_save("inifile")==0) 
        printf("Updated file content saved\n");
    else
        printf("Save to file failed. This should not happen in this demo. Exiting.\n");


do_exit:
    wyini_clean(); /* Cleanup. */
    return 0;
}
