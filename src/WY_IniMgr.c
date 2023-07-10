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


static unsigned int m_buffer_len = 0; /**< Length of the internal buffer allocated. */
static char * m_buffer = NULL; /**< The internal buffer that the content of the file is copied into. */
static char * m_val_buffer = NULL; /**< An internal buffer that stores the value of a variable extracted from the file. This will be allocated with a size of WYINI_MAX_VAL_LEN. */


/**
 * Get the next line in m_buffer. If the m_buffer ends without a next line this returns the index up to the terminating character in m_buffer.
 * \param p_start_offset Position in m_buffer to start parsing.
 * \param p_end_offset Returns position of the next line or terminating character found.
 */
static void wyini_get_nextline(const unsigned int p_start_offset, unsigned int *restrict p_end_offset);

/**
 * Find the value assigned to a variable in a line of characters.
 * \param p_start_offset The starting offset in the line of characters to parse in m_buffer. 
 * \param p_end_offset The ending offset in the line of characters to parse in m_buffer. 
 * \param p_var_len The length of the variable to match.
 * \param p_var The variable to match.
 * \param p_val_offset Returns the offset pointing to the start of the value assigned to the variable.
 * \return 0 if success. Else -1.
 */
static int wyini_find_var_val_inline(const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_var_len, const char *restrict const p_var,  unsigned int *restrict p_val_offset);

/**
 * Removes trailing whitespace from the variable value within a start and end index for m_buffer.
 * @param p_start_offset The starting index in m_buffer pointing to a variable.
 * @param p_end_offset The ending index in m_buffer that points to a variable.
 * @return A new p_end_offset that excludes any trailing whitespace originally found before p_end_offset.
 */
static unsigned int wyini_remove_ending_whitespace(const unsigned int p_start_offset, unsigned int p_end_offset);


int wyini_open(const char *restrict const p_file, const unsigned int p_max_size)
{
    wyini_clean();
    int return_val = -1;

    if(wyini_read_file(p_file, p_max_size, &m_buffer_len, &m_buffer) == 0) {
        m_buffer[m_buffer_len-1] = 0; /* Zero out the eof. */
        printf("File content:\n%s\n", m_buffer);

        if((m_val_buffer = (char*)malloc(WYINI_MAX_VAL_LEN)) != NULL)
            return_val = 0; /* All file reads and memory allocation successful. */
    } 

    if(return_val!=0)
        wyini_clean();

    return return_val;
}


void wyini_clean()
{
    if(m_buffer!=NULL) {
        free(m_buffer);
        m_buffer = NULL;
        m_buffer_len = 0;
    }
    if(m_val_buffer != NULL) {
        free(m_val_buffer);
        m_val_buffer = NULL;
    }
}


int wyini_get_var_val(const char *restrict const p_var, char *restrict *restrict p_val)
{
    if(m_buffer == NULL)
        return -1;

    *p_val = m_val_buffer; /* This value would be invalid if wyini_get_val() below returns failure. */

    const unsigned int max_len = m_buffer_len;
    const unsigned int var_len = strlen(p_var);
    unsigned int start_offset = 0;
    unsigned int end_offset = 0;
    unsigned int val_offset = 0;
    unsigned int val_len = 0;

    while(start_offset < max_len) {
        wyini_get_nextline(start_offset, &end_offset); /* Get the next line in m_buffer. */
                
        if(wyini_find_var_val_inline(start_offset, end_offset, var_len, p_var, &val_offset)==0) { /* Find the variable=value pair in the line. */

            val_len = wyini_remove_ending_whitespace(start_offset, end_offset) - val_offset; /* Remove trailing whitespace after variable=value. */
            if(val_len < WYINI_MAX_VAL_LEN) {  
                snprintf(m_val_buffer, val_len+1, m_buffer+val_offset); /* snprintf writes a terminating char in the last position so val_len+1. */
                return 0; /* Found everything. Return success. */
            } else
                return -1; /* If val_len exceeds buffer assume we'll get the wrong value. Return failure. */
        } 
        
        start_offset = end_offset + 1;
    }

    return -1;
}


static void wyini_get_nextline(const unsigned int p_start_offset, unsigned int *restrict p_end_offset)
{
    const unsigned int max_len = m_buffer_len;
    *p_end_offset = p_start_offset;
    do {
        if(m_buffer[*p_end_offset] == '\n')  /* Found the "\n". */ 
            return;
    } while ((*p_end_offset += 1) < max_len);

    *p_end_offset -= 1; /* Exceeded end of the buffer. Retrace to skip the terminating character. */
}


static int wyini_find_var_val_inline(const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_var_len, const char *restrict const p_var,  unsigned int *restrict p_val_offset)
{
    if(strncmp(m_buffer + p_start_offset, p_var, p_var_len) != 0) /* Match the var. */
        return -1;

    int i = p_start_offset + p_var_len;
    while(i<p_end_offset) { /* Var found. Try to match the " =" pattern after the var. */
        if(m_buffer[i]==' ') /* Skip the whitespace. */
            ++i;
        else if(m_buffer[i]=='=') { /* Found '='. Break the loop. */
            ++i;
            break;
        }
        else /* Found something that isn't a whitespace or '=' after the var. No match. Exit. */
            return -1;
    }

    while(i<p_end_offset) { /* Skip any whitespace after the ' =' pattern. */
        if(m_buffer[i]==' ')
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


static unsigned int wyini_remove_ending_whitespace(const unsigned int p_start_offset, unsigned int p_end_offset)
{
    while((m_buffer[p_end_offset-1] == ' ') && (p_end_offset > p_start_offset))
        --p_end_offset;
    return p_end_offset;
}