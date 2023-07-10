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
    const char *restrict const var = "VAR_2";
    char *val;

    if(wyini_open("inifile", 1024) != 0) {/* Open the file and copy the content. */
        printf("Error reading file.\n");
        return 0;
    }
    if(wyini_get_var_val(var, &val) == 0)
        printf("VAR: %s | VAL: %s | VAL len:%lu\n", var, val, strlen(val));
    else
        printf("VAR: %s not found\n", var);
    wyini_clean();
    return 0;
}
