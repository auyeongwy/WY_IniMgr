/**
 * @file WY_IniMgr.c
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "WY_IniDefs.h"
#include "WY_IniMgr.h"
#include "WY_IniIO.h"
#include "WY_IniParseAgent.h"
#include "WY_IniWriteAgent.h"


static struct S_wyini_buffer m_wyini_buffer; /**< Internal buffer maintained by WY_IniMgr. */


void wyini_init()
{
    m_wyini_buffer.m_max_file_size = 0;
    m_wyini_buffer.m_buffer_len = 0;
    m_wyini_buffer.m_buffer = NULL;
    m_wyini_buffer.m_val_buffer = NULL;
}



int wyini_open(const char *restrict const p_file, const unsigned int p_max_size)
{
    wyini_clean();
    int return_val;

    if((return_val = wyini_read_file(p_file, p_max_size, &(m_wyini_buffer.m_buffer_len), &(m_wyini_buffer.m_buffer))) == WYINI_OK) {
        m_wyini_buffer.m_buffer[m_wyini_buffer.m_buffer_len-1] = 0; /* Zero out the eof. */
        m_wyini_buffer.m_max_file_size = p_max_size;

        if((m_wyini_buffer.m_val_buffer = (char*)malloc(WYINI_MAX_VAL_LEN)) != NULL)
            return_val = WYINI_OK; /* All file reads and memory allocation successful. */
        else
            return_val = WYINI_MEMORY_ERR;
    } 

    if(return_val != WYINI_OK)
        wyini_clean();
    return return_val;
}



int wyini_save(const char *restrict const p_file)
{
    if((m_wyini_buffer.m_buffer == NULL) || (m_wyini_buffer.m_buffer_len <= 1))  /* No data to write. Exit. */
        return WYINI_MEMORY_ERR;
    return wyini_save_file(p_file, m_wyini_buffer.m_buffer_len, m_wyini_buffer.m_buffer); 
}



void wyini_clean()
{
    m_wyini_buffer.m_max_file_size = 0;
    m_wyini_buffer.m_buffer_len = 0;
    if(m_wyini_buffer.m_buffer != NULL) { 
        free(m_wyini_buffer.m_buffer);
        m_wyini_buffer.m_buffer = NULL;
    }
    if(m_wyini_buffer.m_val_buffer != NULL) {
        free(m_wyini_buffer.m_val_buffer);
        m_wyini_buffer.m_val_buffer = NULL;
    }
}



int wyini_get_var_val(const char *restrict const p_var, char *restrict *restrict p_val)
{
    if(m_wyini_buffer.m_buffer == NULL)
        return WYINI_MEMORY_ERR;

    *p_val = m_wyini_buffer.m_val_buffer; /* This value would be invalid if wyini_get_val() below returns failure. */

    const unsigned int max_len = m_wyini_buffer.m_buffer_len;
    const unsigned int var_len = strlen(p_var);
    unsigned int start_offset = 0;
    unsigned int end_offset = 0;
    unsigned int val_offset = 0;
    unsigned int val_len = 0;

    while(start_offset < max_len) {
        wyini_get_nextline(start_offset, &end_offset, &m_wyini_buffer); /* Get the next line in m_buffer. */
                
        if(wyini_find_var_val_inline(false, start_offset, end_offset, var_len, p_var, &val_offset, &m_wyini_buffer)==WYINI_OK) { /* Find the variable=value pair in the line. */
            val_len = wyini_remove_ending_whitespace(start_offset, end_offset, &m_wyini_buffer) - val_offset; /* Remove trailing whitespace after variable=value. */
            if(val_len < WYINI_MAX_VAL_LEN) {  
                snprintf(m_wyini_buffer.m_val_buffer, val_len+1, m_wyini_buffer.m_buffer+val_offset); /* snprintf writes a terminating char in the last position so val_len+1. */
                return WYINI_OK; /* Found everything. Return success. */
            } else
                return WYINI_NOT_FOUND; /* If val_len exceeds buffer assume we'll get the wrong value. Return failure. */
        } 
        
        start_offset = end_offset + 1; /* Pattern not found. Move on to the next line. */
    }

    return WYINI_NOT_FOUND;
}



int wyini_write_val(const char *restrict const p_var, const char *restrict const p_val)
{
    if(m_wyini_buffer.m_buffer == NULL) /* Empty buffer, exit. */
        return WYINI_MEMORY_ERR;

    const unsigned int max_len = m_wyini_buffer.m_buffer_len;
    const unsigned int val_len = strlen(p_val);
    unsigned int start_offset = 0;
    unsigned int end_offset = 0;
    unsigned int val_offset = 0;

    if(val_len >= WYINI_MAX_VAL_LEN) /* Val size exceeds designated limit. Exit. */
        return WYINI_MEMORY_ERR;

    while(start_offset < max_len) {
        wyini_get_nextline(start_offset, &end_offset, &m_wyini_buffer); /* Get the next line in m_buffer. */
        if(wyini_find_var_val_inline(true, start_offset, end_offset, strlen(p_var), p_var, &val_offset, &m_wyini_buffer)==WYINI_OK) /* Find the "variable=" pattern in the line. */
            return wyini_write_val_inline(val_offset, end_offset, val_len, p_val, &m_wyini_buffer);
        start_offset = end_offset + 1; /* Pattern not found. Move on to the next line. */
    }

    return WYINI_NOT_FOUND; /* Not found, return failure. */
}
