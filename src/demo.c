/**
 * @file demo.c
 * Demo application to demonstrate usage of the WY_IniMgr library. 
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "WY_IniDefs.h"
#include "WY_IniMgr.h"


int main(int argc, char *argv[])
{
    //const char *filename = "win_inifile.txt";
    const char *filename = "inifile";
    char var[32];
    char num_val[32];
    char *val;

    wyini_init();
    if(wyini_open(filename, 1024) != WYINI_OK) {/* Open the file and copy the content. */
        printf("Error reading file %s.\n", filename);
        return 0;
    } else
        printf("Open and read file %s\n", filename);

    strncpy(var, "VAR_2", strlen("VAR_2"));
    var[strlen("VAR_2")] = 0;
    if(wyini_get_var_val(var, &val) == WYINI_OK) /* Read a var. */
        printf("VAR:%s | VAL:%s | VAL len:%zd\n", var, val, strlen(val));
    else
        printf("VAR:%s not found\n", var);

    strncpy(var, "VAR_3", strlen("VAR_3"));
    var[strlen("VAR_3")] = 0;
    if(wyini_get_var_val(var, &val) == WYINI_OK) /* Read another var. */
        printf("VAR:%s | VAL:%s | VAL len:%zd\n", var, val, strlen(val));
    else
        printf("VAR:%s not found\n", var);

    strncpy(var, "VAR_4", strlen("VAR_4"));
    var[strlen("VAR_4")] = 0;
    if(wyini_get_var_val(var, &val) == WYINI_OK) /* Read another var. */
        printf("VAR:%s | VAL:%s | VAL len:%zd\n", var, val, strlen(val));
    else
        printf("VAR: %s not found\n", var);

    strncpy(var, "VAR_5", strlen("VAR_5"));
    var[strlen("VAR_5")] = 0;
    int tmp = wyini_get_var_val(var, &val); /* Read a var with no value assigned. */
    if(tmp == WYINI_OK) 
        printf("VAR:%s | VAL:%s | VAL len:%zd\n", var, val, strlen(val));
    else if(tmp == WYINI_VAL_NOT_FOUND)
        printf("VAR: %s has no value\n", var);
    else
        printf("VAR: %s not found\n", var);

    strncpy(var, "NO_VAR", strlen("NO_VAR"));
    var[strlen("NO_VAR")] = 0;
    if(wyini_get_var_val(var, &val) == WYINI_OK) /* Read var that does not exist. */
        printf("VAR:%s | VAL:%s | VAL len:%zd\n", var, val, strlen(val));
    else
        printf("VAR: %s not found\n", var);

    strncpy(var, "NUM_VAR_1", strlen("NUM_VAR_1"));
    var[strlen("NUM_VAR_1")] = 0;
    if(wyini_get_var_val(var, &val) == WYINI_OK) /* Read another var. */
        printf("VAR:%s | VAL:%s | VAL len:%zd\n", var, val, strlen(val));
    else {
        printf("VAR:%s not found\n", var);
        goto do_exit;
    }

    snprintf(num_val, 32, "%d", atoi(val) + 1); /* Increment the number read from "NUM_VAR_1" by 1. */
    if(wyini_write_val(var, num_val) != WYINI_OK) {
        printf("Write VAL:%s to VAR%s failed.\n", val, var);
        goto do_exit;
    } else 
        printf("Wrote VAL:%s to VAR:%s\n", num_val, var);

    if(wyini_get_var_val(var, &val) == WYINI_OK) /* Let's read "NUM_VAR_1" after we overwrote the value. */
        printf("VAR:%s | VAL:%s | VAL len:%zd\n", var, val, strlen(val));
    else {
        printf("VAR:%s not found\n", var);
        goto do_exit;
    }

    if(wyini_save(filename)==WYINI_OK) 
         printf("Updated file content saved\n");
    else
         printf("Save to file failed.\n");


do_exit:
    wyini_clean(); /* Cleanup. */
    return 0;
}
