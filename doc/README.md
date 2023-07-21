[TOC]
Introduction
============
WY_IniMgr is a C library that provides a simple API to read values from a configuration file (INI format) of the form:

Variable 1 = Value 1 <br>
Variable 2 = Value 2 <br>

This library has the following key characteristics:
- The library is developed on a Linux system and excludes using regular expression functions from regex.h so it can be more easily compiled on Windows systems without the need to get the associated PCRE library, which in turn adds another dependency.
- It adheres to using standard C functions to make re-building on other systems easy.
- Implements write functions in addition to read functions so users can read and write to INI files.  
- Uses the MIT license so it is easy to integrate into any application.
- Consciously developed with a simple-to-use API to make it super-easy to use in any application. 

File Organisation
=================
There are 3 directories in the base directory: 

src: Contains all source code files. <br>

build: Contains the following:
- The Makefile. 
- A sample text file inifile which can be used for the demo.
- A Windows-style formatted text file win_inifile.txt which can be used for the demo.
- After compilation, all object, library and demo files will be in this directory. 

doc: Contains the following:
- A README.md and Doxyfile used by doxygen.
- By running doxygen using the provided Doxyfile, all HTML documentation will be generated in this directory.

Compilation and Usage
=====================
Before compiling, one piece of code you may need to change is found in WY_IniDefs.h. The description is self-explanatory:

    #define WYINI_MAX_VAL_LEN 128 /** The Maximum size a value read from the ini file can be. */ 
    /* Values larger than this size will be truncated.*/ 
    /* Change this value to be able to read larger values. */

After any required modifications, we can proceed to build the library and demo application by entering the build directory and entering "make". This generates:
1. A library lib_WY_IniMgr.a.
2. A demo application compiled from demo.c named demo.

How to use the library:
1. Include WY_IniMgr.h in your code. 
2. Link to lib_WY_IniMgr.a in your build.
3. And that's it.

For an example, refer to demo.c. This is the best guide to using the library.

The supplied Makefile defaults to using the following compiler flags. Modify them as required to suit your own build system.<br>
`-std=c17 -Wall -O2 -march=native`

To clean up object files, run `make clean`. To clean up all files including library files and the demo application, run `make distclean`.

Demo application
================
The demo application reads and writes to either inifile or win_inifile.txt (with Windows-style '\r\n' nextline). Users can easily switch between either file by changing the source code in demo.c.

    const char *filename = "win_inifile.txt";
    //const char *filename = "inifile";

demo.c is easy to read and understand - there is not much more to say about it.

Implementation Details
======================

Reading values from a file
-------------------------- 
-# Call wyini_init() to initialise the internal buffers in the library.
-# Some limits on internal buffer sizes are defined in WY_IniDefs.h. So to change the limits, this file needs to be modified and the library re-compiled.
-# Call wyini_open() to open a file. This reads the content into a dynamically allocated internal buffer.
-# Note: The file is opened and closed with wyini_open(). Other API function calls only operate on the internal buffers maintained by the WY_IniMgr library.
-# Call wyini_get_var_val() to read a variable-value pair of the form var=val.
-# The library parses the content line-by-line by looking for the '\n' character.
-# The library discards trailing whitespace in every line after the 'var=val' pattern. E.g. "var=value   " will be read as "var=value".
-# Do not quote values if the quotes are not required. E.g. var1 = "value in var1". The library will actually copy the double quotes as part of the variable's value.
-# Call wyini_clean() to clean up all internal buffers when processing is completed.

Writing values to a file
------------------------ 
-# The mechanics for writing to a file is almost identical to reading values from a file, save that we have use other API functions to perform the write operations.
-# After file content is read into the internal buffer with wyini_open(), call wyini_write_val() to write values to existing varaiables.
-# Note: The file is only opened and closed with wyini_open(). All subsequent function calls only operate on the internal buffers maintained by the WY_IniMgr library. So there is actually no more system IO after this function call.
-# The library will automatically look for a valid 'var=' pattern in order to write the new value. If the var is not found, the function call will fail.
-# If writing variables will cause the file size to exceed the limits defined in the wyini_open() API function call, the write operation will fail.  
-# To save the internal buffer content to a file, call wyini_save().
-# Call wyini_clean() to clean up all internal buffers when processing is completed.

Windows-style nextline
----------------------
The library supports both '\\n' and '\r\\n' nextline indicators. 

Erasing values assigned to a variable
-------------------------------------
We can erase a value assigned to a variable by simply writing an empty space " " to it. Example:

    wyini_write_val("VARNAME"," ");

Variable with no value
----------------------
If wyini_get_var_val() is called with `p_var_only = false` and the variable is found but no value is found in the 'var=val' pattern, `WYINI_VAL_NOT_FOUND` is returned. This is to support locating variables that are found in the INI config file with no values assigned. 

Security cautions
-----------------
Obviously writing values to configuration files is a matter of critical security concern. It is the users' resposibility to make the necessary checks before making the write function calls! 