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


static struct S_wyini_buffer m_wyini_buffer; /**< Internal buffer maintained by WY_IniMgr. */


/**
 * Get the next line in m_buffer. If the m_buffer ends without a next line this returns the index up to the terminating or last char in m_buffer.
 * @param p_start_offset Position in m_buffer to start parsing.
 * @param p_end_offset Returns position of the next line or terminating character found.
 */
static void wyini_get_nextline(const unsigned int p_start_offset, unsigned int *restrict p_end_offset);

/**
 * Find the value assigned to a variable in a line of characters in the pattern 'var=val' within m_buffer.
 * @param p_var_only If set to true, this function only searches for the 'var=' pattern and returns p_val_offset as the index after 'var='. If set to false, this function searches for the patter 'var=val' and returns p_val_offset as the index after 'var=val'.  
 * @param p_start_offset The starting offset in the line of characters to parse in m_buffer. 
 * @param p_end_offset The ending offset in the line of characters to parse in m_buffer. 
 * @param p_var_len The length of the variable to match.
 * @param p_var The variable to match.
 * @param p_val_offset Returns the offset pointing to the start of the value assigned to the variable.
 * \return 0 if success. Else -1.
 */
static int wyini_find_var_val_inline(const bool p_var_only, const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_var_len, const char *restrict const p_var, unsigned int *restrict p_val_offset);

/**
 * Writes a variable 
*/
static int wyini_write_var(const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_var_len, const char *restrict const p_var);

/**
 * Removes trailing whitespace from the variable value within a start and end index for m_buffer.
 * @param p_start_offset The starting index in m_buffer pointing to a variable.
 * @param p_end_offset The ending index in m_buffer that points to a variable.
 * @return A new p_end_offset that excludes any trailing whitespace originally found before p_end_offset.
 */
inline static unsigned int wyini_remove_ending_whitespace(const unsigned int p_start_offset, unsigned int p_end_offset);



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
    int return_val = -1;

    if(wyini_read_file(p_file, p_max_size, &(m_wyini_buffer.m_buffer_len), &(m_wyini_buffer.m_buffer)) == 0) {
        m_wyini_buffer.m_buffer[m_wyini_buffer.m_buffer_len-1] = 0; /* Zero out the eof. */
        m_wyini_buffer.m_max_file_size = p_max_size;

        if((m_wyini_buffer.m_val_buffer = (char*)malloc(WYINI_MAX_VAL_LEN)) != NULL)
            return_val = 0; /* All file reads and memory allocation successful. */
    } 

    if(return_val!=0)
        wyini_clean();

    return return_val;
}



int wyini_save(const char *restrict const p_file)
{
    if((m_wyini_buffer.m_buffer == NULL) || (m_wyini_buffer.m_buffer_len <= 1))  /* No data to write. Exit. */
        return -1;

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
        return -1;

    *p_val = m_wyini_buffer.m_val_buffer; /* This value would be invalid if wyini_get_val() below returns failure. */

    const unsigned int max_len = m_wyini_buffer.m_buffer_len;
    const unsigned int var_len = strlen(p_var);
    unsigned int start_offset = 0;
    unsigned int end_offset = 0;
    unsigned int val_offset = 0;
    unsigned int val_len = 0;

    while(start_offset < max_len) {
        wyini_get_nextline(start_offset, &end_offset); /* Get the next line in m_buffer. */
                
        if(wyini_find_var_val_inline(false, start_offset, end_offset, var_len, p_var, &val_offset)==0) { /* Find the variable=value pair in the line. */
            val_len = wyini_remove_ending_whitespace(start_offset, end_offset) - val_offset; /* Remove trailing whitespace after variable=value. */
            if(val_len < WYINI_MAX_VAL_LEN) {  
                snprintf(m_wyini_buffer.m_val_buffer, val_len+1, m_wyini_buffer.m_buffer+val_offset); /* snprintf writes a terminating char in the last position so val_len+1. */
                return 0; /* Found everything. Return success. */
            } else
                return -1; /* If val_len exceeds buffer assume we'll get the wrong value. Return failure. */
        } 
        
        start_offset = end_offset + 1; /* Pattern not found. Move on to the next line. */
    }

    return -1;
}



int wyini_write_val(const char *restrict const p_var, const char *restrict const p_val)
{
    if(m_wyini_buffer.m_buffer == NULL) /* Empty buffer, exit. */
        return -1;

    const unsigned int max_len = m_wyini_buffer.m_buffer_len;
    const unsigned int val_len = strlen(p_val);
    unsigned int start_offset = 0;
    unsigned int end_offset = 0;
    unsigned int val_offset = 0;

    if(val_len >= WYINI_MAX_VAL_LEN) /* Val size exceeds designated limit. Exit. */
        return -1;

    while(start_offset < max_len) {
        wyini_get_nextline(start_offset, &end_offset); /* Get the next line in m_buffer. */
        if(wyini_find_var_val_inline(true, start_offset, end_offset, strlen(p_var), p_var, &val_offset)==0) /* Find the "variable=" pattern in the line. */
            return wyini_write_var(val_offset, end_offset, val_len, p_val);
        start_offset = end_offset + 1; /* Pattern not found. Move on to the next line. */
    }

    return -1; /* Not found, return failure. */
}



static void wyini_get_nextline(const unsigned int p_start_offset, unsigned int *restrict p_end_offset)
{
    const unsigned int max_len = m_wyini_buffer.m_buffer_len;
    *p_end_offset = p_start_offset;
    do {
        if(m_wyini_buffer.m_buffer[*p_end_offset] == '\n')  /* Found the "\n". */ 
            return;
    } while ((*p_end_offset += 1) < max_len);

    *p_end_offset -= 1; /* Exceeded end of the buffer. Retrace to skip the terminating character. */
}



static int wyini_find_var_val_inline(const bool p_var_only, const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_var_len, const char *restrict const p_var,  unsigned int *restrict p_val_offset)
{
    if(strncmp(m_wyini_buffer.m_buffer + p_start_offset, p_var, p_var_len) != 0) /* Match the var. */
        return -1;

    int i = p_start_offset + p_var_len;
    while(i<p_end_offset) { /* Var found. Try to match the " =" pattern after the var. */
        if(m_wyini_buffer.m_buffer[i]==' ') /* Skip the whitespace. */
            ++i;
        else if(m_wyini_buffer.m_buffer[i]=='=') { /* Found '='. Break the loop. */
            ++i;
            break;
        }
        else /* Found something that isn't a whitespace or '=' after the var. No match. Exit. */
            return -1;
    }

    if(p_var_only) { /* Only searching for the pattern 'var='. The 'val' that comes after is not required. */
        *p_val_offset = i;
        return 0;
    }

    while(i<p_end_offset) { /* Skip any whitespace after the ' =' pattern. */
        if(m_wyini_buffer.m_buffer[i]==' ')
            ++i;
        else
            break;
    }

    if(i<p_end_offset) { /* Get the value assigned to the var. */
        *p_val_offset = i;
        return 0;
    }

    return -1;
}



static int wyini_write_var(const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_val_len, const char *restrict const p_val)
{
    const unsigned int current_space = p_end_offset - p_start_offset; /* How much space between '=' and '\n' or '\0'. */
    
    if(current_space >= p_val_len) { /* Enough space to fit in the new variable. */ 
        if(current_space > p_val_len) {
            memmove(m_wyini_buffer.m_buffer + p_start_offset + p_val_len, m_wyini_buffer.m_buffer + p_end_offset, m_wyini_buffer.m_buffer_len - p_end_offset); /* Move existing content in m_buffer to fill in the space. No need for this step if current_space == p_val_len. */
            m_wyini_buffer.m_buffer_len -= (current_space - p_val_len); /* Current buffer size decreased. Update it. */ 
        }
        memcpy(m_wyini_buffer.m_buffer + p_start_offset, p_val, p_val_len); /* Write the new variable. */
    }

    else { /* Not enough space to fit in the new variable. Additional checks to conduct. */
        const unsigned int additional_space = p_val_len - current_space;
        if(additional_space + m_wyini_buffer.m_buffer_len > m_wyini_buffer.m_max_file_size) /* Writing the new var exceeds max file size. Return failure. */
            return -1;

        memmove(m_wyini_buffer.m_buffer + p_start_offset + p_val_len, m_wyini_buffer.m_buffer + p_end_offset, m_wyini_buffer.m_buffer_len - p_end_offset); /* Move existing content in m_buffer to make space. */
        memcpy(m_wyini_buffer.m_buffer + p_start_offset, p_val, p_val_len); /* Write the new variable. */
        m_wyini_buffer.m_buffer_len += additional_space; /* Update the increased buffer space. */
    }

    return 0;
}



inline static unsigned int wyini_remove_ending_whitespace(const unsigned int p_start_offset, unsigned int p_end_offset)
{
    while((m_wyini_buffer.m_buffer[p_end_offset-1] == ' ') && (p_end_offset > p_start_offset))
        --p_end_offset;
    return p_end_offset;
}
