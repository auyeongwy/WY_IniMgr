/**
 * @file WY_IniDefs.h
 * Declares application-wide definitions / variables / functions.
*/
#ifndef _WY_INIDEFS_H_
#define _WY_INIDEFS_H_

#define WYINI_MAX_VAL_LEN 128 /**< The Maximum size a value read from the ini file can be. Values larger than this size will be truncated. Change this value to be able to read larger values. */

/**
 * The internal buffer structure maintained by WY_IniMgr. 
 */
struct S_wyini_buffer
{
    unsigned int m_max_file_size; /**< Max file size allowed. */
    unsigned int m_buffer_len; /**< Size of the file content currently in m_buffer. */
    char * m_buffer; /**< The internal buffer that the content of the file is copied into. */
    char * m_val_buffer; /**< An internal buffer that stores the value of a variable extracted from the file. This will be allocated with a size of WYINI_MAX_VAL_LEN. */  
};

#endif