#include <stdio.h>

#include "../src/array_util.h"

int main () 
{
    const int tArray[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int buff[8];

    /*
    * Test 0
    * Move 1 element from idx 0 to idx 4
    */
    memcpy(buff, tArray, sizeof(tArray));
    arrmove(buff, 8, sizeof(int), 0, 4, 1);
//  const int tArray [8] = {0, 1, 2, 3, 4, 5, 6, 7};
    const int expect0[8] = {1, 2, 3, 4, 0, 5, 6, 7};
    if (memcmp(buff, expect0, sizeof(tArray)) != 0)
    {
        fprintf(stderr, "test array util - ERROR [0]");
        return -1;
    }

    /*
    * Test 1
    * Move 2 elements from idx 0 to idx 4
    */
    memcpy(buff, tArray, sizeof(tArray));
    arrmove(buff, 8, sizeof(int), 0, 4, 2);
//  const int tArray [8] = {0, 1, 2, 3, 4, 5, 6, 7};
    const int expect1[8] = {2, 3, 4, 5, 0, 1, 6, 7};
    if (memcmp(buff, expect1, sizeof(tArray)) != 0)
    {
        fprintf(stderr, "test array util - ERROR [1]");
        return -1;
    }

    /*
    * Test 2
    * Move 3 elements from idx 2 to idx 3
    */
    memcpy(buff, tArray, sizeof(tArray));
    arrmove(buff, 8, sizeof(int), 2, 3, 3);
//  const int tArray [8] = {0, 1, 2, 3, 4, 5, 6, 7};
    const int expect2[8] = {0, 1, 5, 2, 3, 4, 6, 7};
    if (memcmp(buff, expect2, sizeof(tArray)) != 0)
    {
        fprintf(stderr, "test array util - ERROR [2]");
        return -1;
    }

    /*
    * Test 3
    * Move 3 elements from idx 2 to idx 4
    */
    memcpy(buff, tArray, sizeof(tArray));
    arrmove(buff, 8, sizeof(int), 2, 4, 3);
//  const int tArray [8] = {0, 1, 2, 3, 4, 5, 6, 7};
    const int expect3[8] = {0, 1, 5, 6, 2, 3, 4, 7};
    if (memcmp(buff, expect3, sizeof(tArray)) != 0)
    {
        fprintf(stderr, "test array util - ERROR [3]");
        return -1;
    }

    /*
    * Test 4
    * Move 2 elements from idx 0 to idx 5
    */
    memcpy(buff, tArray, sizeof(tArray));
    arrmove(buff, 8, sizeof(int), 0, 5, 2);
//  const int tArray [8] = {0, 1, 2, 3, 4, 5, 6, 7};
    const int expect4[8] = {2, 3, 4, 5, 6, 0, 1, 7};
    if (memcmp(buff, expect4, sizeof(tArray)) != 0)
    {
        fprintf(stderr, "test array util - ERROR [4]");
        return -1;
    }

    return 0;
}