#ifndef RSORT_H
#define RSORT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define RSORT_LOG_DEBUG     // uncomment to log debug messages to the console.
#define RSORT_PREFLIGHT_CHECKS // uncomment to perform preflight checks (comment out for maximum performance)


/*
* If you know that the range of nItems you will be using will not 
* require uint64_t, or uint64_t is not available on your system, 
* you can define rs_uint to be whatever number size you like.
*/
typedef int rs_int;
#define RS_INT_MAX INT_MAX

typedef unsigned int rs_uint;
#define RS_UINT_MAX UINT_MAX

typedef uint8_t rs_size_col;
#define RS_SIZE_COL_MAX UINT8_MAX

typedef unsigned long rs_size_row;
#define RS_SIZE_ROW_MAX ULONG_MAX

typedef unsigned long long rs_size_max;
#define RS_SIZE_MAX ULLONG_MAX

#define RSORT_DONE (rs_uint)0
#define RSORT_ERROR (rs_uint)1
#define RSORT_HEALTHY (rs_uint)10


void default_tokenizer(void* val, rs_size_col* buff);

void lowercase_character_tokenizer(void* val, rs_size_col* buff);

/*
* memStart  - the beggining of the region of memory containing the data to be sorted
* nRows     - the number of 'elements' in the memory region
* rowSize   - the number of bytes per element
* colSize   - the number of bytes to use per itteration. generally, 1 is a good number to use here.
* nDV       - number of discrete values per column.
*             eg. lower-case letters may take up a byte of storage, which suggests 255 possible discrete values, but the programmer knows that there are only 26.
* tokenizer - a function that transforms each column into a numerical representation. The tokenizer's minimum output should be 0, and its maximum output should be nDV - 1.
*             reference default_tokenizer() and lowercase_character_tokenizer() for examples.
*/
rs_uint rsort_lsb_unsigned_bytes_v1(const void* memStart, const rs_size_max nRows, const rs_size_row rowSize, const rs_size_col colSize, rs_size_col nDV, void(*tokenizer)(void*, rs_size_col*));

/*
* ToDo:
* Use bitmask on 'last' itteration to take the signed bit out of the calculation,
* then perform one final itteration on only the signed bit.
*/
// rs_uint rsort_lsb_signed_bytes_v1(const void* memStart, const rs_size_max nRows, const rs_size_row rowSize, const rs_size_col colSize, rs_size_col nDV, void(*tokenizer)(void*, rs_size_col*));

rs_uint rsort_lsb_lowercase_text_v1(const void* memStart, const rs_size_max nWords, const uint8_t wordSize);


#endif /* RSORT_H */
