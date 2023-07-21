/**
 * @file WY_IniDefs.h
 * Declares application-wide definitions / variables / functions.
*/
#ifndef _WY_INIDEFS_H_
#define _WY_INIDEFS_H_

#define WYINI_MAX_VAL_LEN 128 /**< The Maximum size a value read from the ini file can be. Values larger than this size will be truncated. Change this value to be able to read larger values. */

#define WYINI_OK 0 /**< Status OK. */
#define WYINI_MEMORY_ERR -1 /**< Status NOK caused by memory error. E.g. memory allocation failed, memory limits exceeded, etc. */
#define WYINI_IO_ERR -2 /**< Status NOK caused by system IO error. E.g. failed to find or open file. */
#define WYINI_NOT_FOUND -3 /**< Status NOK caused by resource not found. E.g. pattern not found. */
#define WYINI_VAL_NOT_FOUND -4 /**< Status NOK caused by variable not found in the pattern 'var=val'. */

/**
 * The internal buffer structure maintained by WY_IniMgr. 
 */
struct S_wyini_buffer
{
    unsigned int m_max_file_size; /**< Max file size allowed. */
    unsigned int m_buffer_len; /**< Size of the file content currently in m_buffer. Size of the file content is always capped at m_max_file_size-1. This is to allow us to append a terminating 0 to the last byte. */
    char * m_buffer; /**< The internal buffer that the content of the file is copied into. The size here is provided by m_buffer_len. */
    char * m_val_buffer; /**< An internal buffer that stores the value of a variable extracted from the file. This will be allocated with a size of WYINI_MAX_VAL_LEN. */  
};

#endif