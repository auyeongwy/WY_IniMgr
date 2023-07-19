/**
 * @file WY_IniMgr.h
 * Header file that declares the APIs of the WY_IniMgr library. Users should only need to use the APIs here to use the library. Refer to the demo application main.c for example usage.
 * \n
 * Example Usage: 
 * @code
 * #include "WY_IniMgr.h" 
 * 
 * char *val; 
 * 
 * if(wyini_open("inifile", 1024) == WYINI_OK) { 
 *  if(wyini_get_var_val("VAR_1", &val) == WYINI_OK)
 *      printf("The value of VAR_1 in inifile is %s\n", val);
 * } 
 * wyini_clean(); 
 * @endcode
 * 
*/
#ifndef _WY_INIMGR_H_
#define _WY_INIMGR_H_

/**
 * Initialises WY_IniMgr internals. Always call this function first before calling any other API or bad things will happen.
 */
void wyini_init();

/**
 * Reads content from a file into internal buffer. 
 * Internally, this performs the following actions: 
 * -# Opens the file. 
 * -# Reads all file content into an internal buffer. 
 * -# Closes the file. 
 * 
 * Calling this function implicitly calls wyini_clean() at the beginning, to clean up the internal buffers. So for e.g. if one wants to read 2 files one after the other, we can call this function consecutively without the need to call wyini_clean() twice. But wyini_clean() is always required to properly perform final cleanup. E.g. <br>
 * @code
 * wyini_init();
 * wyini_open("file1", MAXSIZE); 
 * // ... process the content in file1 
 * wyini_open("file2", MAXSIZE); 
 * // ... process the content in file2 
 * wyini_clean() // Done with processing file content so final cleanup. 
 * @endcode
 * @param p_file File to open. 
 * @param p_max_size Limits the size of the file to parse. E.g. passing 1024*1024 limits us to not parsing a file more than 1MB in size.
 * @return WYINI_OK if success. Else a negative value defined in WY_IniDefs.h. 
 */
int wyini_open(const char *restrict const p_file, const unsigned int p_max_size);

/**
 * Saves the content of the internal buffer to a file - overwriting it if it already exists. Obviusly this only works if the internal buffer is already populated via an earlier API calls such as wyini_open(). 
 * @param p_file The file name.
 * @return WYINI_OK if success. Else a negative value defined in WY_IniDefs.h.
 */
int wyini_save(const char *restrict const p_file);

/**
 * Cleans up the internal buffers. Call this function when all read/write operations are completed. Note that wyini_open() calls this function implicitly at the beginning of execution, but NOT at the end of execution. Hence this function must be called for a final clean-up. 
 */
void wyini_clean();

/**
 * Gets the char value of a variable.
 * @param p_var The variable name to search for.
 * @param p_val Returns the value assigned to p_var.
 * @return WYINI_OK if success. Else a negative value defined in WY_IniDefs.h.
 */
int wyini_get_var_val(const char *restrict const p_var, char *restrict *restrict p_val);

/**
 * Writes the char value of an existing variable to the internal buffer opened by wyini_open().  
 * @param p_var The variable name.
 * @param p_val The value to write.
 * @return WYINI_OK if success. Else a negative value defined in WY_IniDefs.h. Causes for error are usually:<br>
 * (1) The internal buffer is empty. <br>
 * (2) The variable does not exist. <br>
 * (3) Writing the variable exceeds the maximum allowed size of the file or variable length.
*/
int wyini_write_val(const char *restrict const p_var, const char *restrict const p_val);


#endif