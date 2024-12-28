#include <string.h>

#include "rsort.h"

#ifdef RSORT_LOG_DEBUG
  #include "bitmask.h"
#endif


void default_tokenizer(void* val, rs_size_col* buff)
{
    memcpy(buff, val, sizeof(rs_size_col));
}

void lowercase_character_tokenizer(void* val, rs_size_col* buff) 
{
    char _val = *(char*)val;
    char token = _val - 'a';
    memcpy(buff, (void*)(&token), sizeof(rs_size_col));
}

rs_uint rsort_prechecks(const rs_size_row rowSize, const rs_size_col colSize, rs_size_col nDV, void(*tokenizer)(void*, rs_size_col*))
{
    if (rowSize < colSize) 
    {
        fprintf(stderr, "rsort() - ERROR: rowSize < colSize == true\n");
        return RSORT_ERROR;
    }

    if (rowSize % colSize != 0) 
    {
        fprintf(stderr, "rsort() - ERROR: rowSize modulo colSize != 0\n");
        fprintf(stderr, "rsort() - %i mod %i = %i\n", rowSize, colSize, rowSize % colSize);
        return RSORT_ERROR;
    }

    if (colSize > 4) 
    {
        fprintf(stderr, "rsort() - ERROR: colSize > 8\n");
        return RSORT_ERROR;
    }

    if (nDV <= 0)
    {
        fprintf(stderr, "rsort() - ERROR: nDV <= 0.\n");
        return RSORT_ERROR;
    }

    if (((rs_size_max)1 << (8 * colSize)) - 1 > RS_SIZE_COL_MAX || ((rs_size_max)1 << (8 * colSize)) - 1 < 0)
    {
        fprintf(stderr, "rsort() - ERROR: rs_size_col is too small to hold the selected colSize. %i > %i || %i < 0 \n", ((rs_size_max)1 << (8 * colSize)) - 1, RS_SIZE_COL_MAX, ((rs_size_max)1 << (8 * colSize)) - 1);
        fprintf(stderr, "rsort() - You can change the size of rs_size_col by defining it to be the same as a different datatype in rsort.h\n");
        return RSORT_ERROR;
    }

    if (tokenizer == NULL)
    {
        fprintf(stderr, "rsort() - ERROR: tokenizer is null.\n");
        return RSORT_ERROR;
    }

    return RSORT_HEALTHY;
}

rs_uint rsort_lsb_unsigned_bytes_v1(const void* memStart, const rs_size_max nRows, const rs_size_row rowSize, const rs_size_col colSize, rs_size_col nDV, void(*tokenizer)(void*, rs_size_col*))
{
    if (memStart == NULL) return RSORT_ERROR;

#ifdef RSORT_PREFLIGHT_CHECKS
    if ((rsort_prechecks(rowSize, colSize, nDV, tokenizer) != RSORT_HEALTHY)) return RSORT_ERROR;
#endif

    void* mS = memStart;
    void* mE = memStart + (nRows * rowSize);

    const rs_size_row nSteps = rowSize / colSize;

    void*       tmp[2][nDV][nRows];
    rs_size_max ct[2][nDV];
    rs_size_col buff = 0;
    
    memset(tmp, 0, sizeof(tmp));
    memset(ct, 0, sizeof(ct));
    
    rs_int stepCt = nSteps - 1;
    void* memPtr = mS + stepCt;

    for (memPtr; memPtr < mE; memPtr += rowSize)
    {
        tokenizer(memPtr + 0, &buff);
        tmp[0][buff][ct[0][buff]] = memPtr;
        ct[0][buff] += 1;

#ifdef RSORT_LOG_DEBUG
        char* binary_str = TO_BINARY_STRING(rs_size_col, buff);
        fprintf(stdout, "rsort itteration %i: sorted token '%s' aka '%i' into tmp[%i]\n\n", stepCt + 1, binary_str, buff, 0);
#endif
    }

    --stepCt;

    rs_size_max dVct;
    rs_uint currIdx = 0;
    rs_uint nextIdx = 1;

    for (stepCt; stepCt >= 0; --stepCt)
    {
        for (rs_size_max i = 0; i < nDV; ++i)
        {
            dVct = ct[currIdx][i];

            if (dVct > 0)
            {
                for (rs_size_max j = 0; j < dVct; ++j)
                {
                    memPtr = tmp[currIdx][i][j];

                    if (memPtr == NULL)
                    {
                        break;
                    } else
                    {
                        --memPtr;

                        tokenizer(memPtr, &buff);
                        tmp[nextIdx][buff][ct[nextIdx][buff]] = memPtr;
                        ct[nextIdx][buff] += 1;

#ifdef RSORT_LOG_DEBUG
                        char* binary_str = TO_BINARY_STRING(rs_size_col, buff);
                        fprintf(stdout, "rsort itteration %i: sorted token '%s' aka '%i' into tmp[%i]\n\n", stepCt + 1, binary_str, buff, nextIdx);
#endif
                    }
                }
            }
        }

        if (stepCt != 0)
        {
            memset(tmp[currIdx], 0, sizeof(tmp[currIdx]));
            memset(ct[currIdx], 0, sizeof(ct[currIdx]));
        }

        if (currIdx == 0)
        {
            currIdx = 1;
            nextIdx = 0;
        } else
        {
            currIdx = 0;
            nextIdx = 1;
        }
    }

    /*
    * Note for application-specific usages:
    * Writing to tmp2 and then copying to memStart is not efficient, but it is safe.
    * 
    * Something like this would be faster:
    *   void* tmp2 = malloc(nRows * rowSize)
    *   <execute loop, then...>
    *   free(memStart) // if applicable
    *   memStart = tmp2
    * 
    *   [don't forget to free memStart again before the application closes.]
    */
    void* tmp2[nRows][rowSize];
    memPtr = tmp2;
    void* srcPtr;

#ifdef RSORT_LOG_DEBUG
    void* tmp2mS = tmp2;
    void* tmp2mE = tmp2 + (nRows * rowSize);
#endif

    for (rs_size_max i = 0; i < nDV; ++i)
    {
        dVct = ct[currIdx][i];

        if (dVct > 0)
        {
            for (rs_size_max j = 0; j < dVct; ++j)
            {
                srcPtr = tmp[currIdx][i][j];

                if (srcPtr == NULL)
                {
                    break;
                } else
                {
#ifdef RSORT_LOG_DEBUG
                        char* binary_str = TO_BINARY_STRING(rs_size_col, buff);
                        fprintf(stdout, "copying %i bytes from tmp[%i][%i][%i] to input at memory location %p, which will overwrite any data stored there. The range of the input is [%p, %p].\n\n", rowSize, currIdx, i, j, memPtr, tmp2mS, tmp2mE);
#endif
                    memcpy(memPtr, srcPtr, rowSize);

                    memPtr += rowSize;
                }
            }
        }
    }

    memcpy(mS, tmp2, nRows * rowSize);

    return RSORT_DONE;
}

rs_uint rsort_lsb_lowercase_text_v1(const void* memStart, const rs_size_max nWords, const uint8_t wordSize)
{
    if (memStart == NULL) return RSORT_ERROR;

    void* mS = memStart;
    void* mE = memStart + (nWords * wordSize);

    void*       tmp[2][26][nWords];
    rs_size_max ct[2][26];
    char        buff = 0;
    
    memset(tmp, 0, sizeof(tmp));
    memset(ct, 0, sizeof(ct));
    
    int stepCt = wordSize - 1;
    void* memPtr = mS + stepCt;

    for (memPtr; memPtr < mE; memPtr += wordSize)
    {
        lowercase_character_tokenizer(memPtr + 0, &buff);
        tmp[0][buff][ct[0][buff]] = memPtr;
        ct[0][buff] += 1;

#ifdef RSORT_LOG_DEBUG
        char* binary_str = TO_BINARY_STRING(char, buff);
        fprintf(stdout, "rsort itteration %i: sorted token '%s' aka '%i' into tmp[%i]\n\n", stepCt + 1, binary_str, buff, 0);
#endif
    }

    --stepCt;

    rs_size_max dVct;
    uint8_t currIdx = 0;
    uint8_t nextIdx = 1;

    for (stepCt; stepCt >= 0; --stepCt)
    {
        for (uint8_t i = 0; i < 26; ++i)
        {
            dVct = ct[currIdx][i];

            if (dVct > 0)
            {
                for (rs_size_max j = 0; j < dVct; ++j)
                {
                    memPtr = tmp[currIdx][i][j];

                    if (memPtr == NULL)
                    {
                        break;
                    } else
                    {
                        --memPtr;

                        lowercase_character_tokenizer(memPtr, &buff);
                        tmp[nextIdx][buff][ct[nextIdx][buff]] = memPtr;
                        ct[nextIdx][buff] += 1;

#ifdef RSORT_LOG_DEBUG
                        char* binary_str = TO_BINARY_STRING(char, buff);
                        fprintf(stdout, "rsort itteration %i: sorted token '%s' aka '%i' into tmp[%i]\n\n", stepCt + 1, binary_str, buff, nextIdx);
#endif
                    }
                }
            }
        }

        if (stepCt != 0)
        {
            memset(tmp[currIdx], 0, sizeof(tmp[currIdx]));
            memset(ct[currIdx], 0, sizeof(ct[currIdx]));
        }

        if (currIdx == 0)
        {
            currIdx = 1;
            nextIdx = 0;
        } else
        {
            currIdx = 0;
            nextIdx = 1;
        }
    }

    /*
    * Note for application-specific usages:
    * Writing to tmp2 and then copying to memStart is not efficient, but it is safe.
    * 
    * Something like this would be faster:
    *   void* tmp2 = malloc(nWords * wordSize)
    *   <execute loop, then...>
    *   free(memStart) // if applicable
    *   memStart = tmp2
    * 
    *   [don't forget to free memStart again before the application closes.]
    */
    void* tmp2[nWords][wordSize];
    memPtr = tmp2;
    void* srcPtr;

#ifdef RSORT_LOG_DEBUG
    void* tmp2mS = tmp2;
    void* tmp2mE = tmp2 + (nWords * wordSize);
#endif

    for (uint8_t i = 0; i < 26; ++i)
    {
        dVct = ct[currIdx][i];

        if (dVct > 0)
        {
            for (rs_size_max j = 0; j < dVct; ++j)
            {
                srcPtr = tmp[currIdx][i][j];

                if (srcPtr == NULL)
                {
                    break;
                } else
                {
#ifdef RSORT_LOG_DEBUG
                    char* binary_str = TO_BINARY_STRING(char, buff);
                    fprintf(stdout, "copying %i bytes from tmp[%i][%i][%i] to input at memory location %p, which will overwrite any data stored there. The range of the input is [%p, %p].\n\n", wordSize, currIdx, i, j, memPtr, tmp2mS, tmp2mE);
#endif
                    memcpy(memPtr, srcPtr, wordSize);

                    memPtr += wordSize;
                }
            }
        }
    }

    memcpy(mS, tmp2, nWords * wordSize);

    return RSORT_DONE;
}




