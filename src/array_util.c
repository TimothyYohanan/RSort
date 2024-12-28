#include <string.h>
#include <stdint.h>

#include "array_util.h"


int arrmove(void* arr, size_t arrSize, size_t elemSize, int srcIdx, int destIdx, size_t src_nElem)
{
    if ((destIdx + src_nElem) > arrSize) {
        fprintf(stderr, "arrmove() - ERROR: (destIdx + src_nElem) > arrSize == true");
        return ARRMOVE_ERROR;
    }

    if (destIdx == srcIdx) {
        fprintf(stderr, "arrmove() - ERROR: destIdx == srcIdx");
        return ARRMOVE_ERROR;
    }

    const size_t  cRangeOffset = elemSize * src_nElem;
    const uint8_t tmp[cRangeOffset];

    const size_t  srcMemOffset  = elemSize * srcIdx;
    const size_t  destMemOffset = elemSize * destIdx;

    const unsigned char* srcBytesStart  = (unsigned char *)arr + srcMemOffset;
    const unsigned char* destBytesStart = (unsigned char *)arr + destMemOffset;
    
    memset(tmp, 0, cRangeOffset); // potential optimization: remove this line.
    memcpy(tmp, (void *)srcBytesStart, cRangeOffset);

    if (destIdx < srcIdx)
    {
        const size_t         nShiftedBytes     = srcMemOffset - destMemOffset;
        const unsigned char* shiftedBytesStart = destBytesStart;
        const unsigned char* shiftedBytesDest  = shiftedBytesStart + cRangeOffset;

        memmove(shiftedBytesDest, shiftedBytesStart, nShiftedBytes);
    } else
    {
        const size_t         nShiftedBytes     = ((destIdx - srcIdx + 1) < src_nElem) ? cRangeOffset - destMemOffset + srcMemOffset : destMemOffset - srcMemOffset;
        const unsigned char* shiftedBytesDest  = srcBytesStart;
        const unsigned char* shiftedBytesStart = shiftedBytesDest + cRangeOffset;

        memmove(shiftedBytesDest, shiftedBytesStart, nShiftedBytes);
    }

    memcpy((void *)destBytesStart, tmp, cRangeOffset);
    
    return ARRMOVE_DONE;
}