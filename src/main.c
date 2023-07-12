/**
 * @file main.c
 * Demo application to demonstrate usage of the WY_IniMgr library. 
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "WY_IniMgr.h"


int main(int argc, char *argv[])
{
    char *val;

    if(wyini_open("inifile", 1024) != 0) {/* Open the file and copy the content. */
        printf("Error reading file.\n");
        return 0;
    }

    if(wyini_get_var_val("VAR_2", &val) == 0) /* Read a var. */
        printf("VAR: %s | VAL: %s | VAL len:%lu\n", "VAR_2", val, strlen(val));
    else
        printf("VAR: %s not found\n", "VAR_2");

    if(wyini_get_var_val("VAR_3", &val) == 0) /* Read another var. */
        printf("VAR: %s | VAL: %s | VAL len:%lu\n","VAR_3", val, strlen(val));
    else
        printf("VAR: %s not found\n", "VAR_3");

    wyini_clean(); /* Cleanup. */
    return 0;
}
