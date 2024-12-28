#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../src/rsort.h"

static const char test_lowercase_words[5][3] = {
    {'d', 'o', 'g'},
    {'c', 'a', 't'},
    {'j', 'a', 'r'},
    {'j', 'a', 'm'},
    {'l', 'a', 'd'}
};

static const char expect_lowercase_words[5][3] = {
    {'c', 'a', 't'},
    {'d', 'o', 'g'},
    {'j', 'a', 'm'},
    {'j', 'a', 'r'},
    {'l', 'a', 'd'}
};

static const uint8_t test_unsigned_numbers[11] = {78, 24, 12, 9, 106, 35, 17, 22, 99, 243, 77};

static const uint8_t expect_unsigned_numbers[11] = {9, 12, 17, 22, 24, 35, 77, 78, 99, 106, 243};

static const uint8_t test_dates[8][4] = {
    {20, 20, 1, 8},
    {20, 24, 12, 20},
    {18, 00, 7, 9},
    {19, 00, 10, 29},
    {20, 03, 8, 8},
    {20, 03, 8, 9},
    {20, 03, 9, 1},
    {20, 24, 12, 21}
};

static const uint8_t expect_dates[8][4] = {
    {18, 00, 7, 9},
    {19, 00, 10, 29},
    {20, 03, 8, 8},
    {20, 03, 8, 9},
    {20, 03, 9, 1},
    {20, 20, 1, 8},
    {20, 24, 12, 20},
    {20, 24, 12, 21}
};

static const uint16_t test_dates2[8][3] = {
    {2020, 1, 8},
    {2024, 12, 20},
    {1800, 7, 9},
    {1900, 10, 29},
    {2003, 8, 8},
    {2003, 8, 9},
    {2003, 9, 1},
    {2024, 12, 21}
};

static const uint16_t expect_dates2[8][3] = {
    {1800, 7, 9},
    {1900, 10, 29},
    {2003, 8, 8},
    {2003, 8, 9},
    {2003, 9, 1},
    {2020, 1, 8},
    {2024, 12, 20},
    {2024, 12, 21}
};

// ToDo:
// static const int64_t test_signed_numbers[11] = {101010, -7000, -1, 0, 829, 1000000, 10000, 9, 99, 140000, -8000};
// static const int64_t expect_signed_numbers[11] = {-8000, -7000, -1, 0, 9, 99, 829, 10000, 101010, 140000, 1000000};

int main () 
{
    /*
    * Test 0
    * List of 5 words with 3 characters.
    * rsort_lsb_lowercase_text_v1
    * tokenizer: auto
    */
    void* buff = malloc(sizeof(test_lowercase_words));

    if (buff == NULL)
    {
        fprintf(stderr, "test_rsort() - failed to allocate buffer.");
        free(buff);
        return -1;
    }

    memcpy(buff, test_lowercase_words, sizeof(test_lowercase_words));

    rsort_lsb_lowercase_text_v1(buff, 5, 3);
    if (memcmp(buff, expect_lowercase_words, sizeof(test_lowercase_words)))
    {
        fprintf(stderr, "test rsort - ERROR [0]\n\n");
        free(buff);
        return -1;
    }

    /*
    * Test 1
    * List of 11 numbers
    * rsort_lsb_unsigned_bytes_v1
    * tokenizer: default
    */
    buff = realloc(buff, sizeof(test_unsigned_numbers));

    if (buff == NULL)
    {
        fprintf(stderr, "test_rsort() - failed to reallocate buffer.");
        free(buff);
        return -1;
    }

    memcpy(buff, test_unsigned_numbers, sizeof(test_unsigned_numbers));

    rsort_lsb_unsigned_bytes_v1(buff, 11, 1, 1, 255, default_tokenizer);
    if (memcmp(buff, expect_unsigned_numbers, sizeof(test_unsigned_numbers)))
    {
        fprintf(stderr, "test rsort - ERROR [1]\n\n");
        free(buff);
        return -1;
    }

    /*
    * Test 2
    * List of 8 dates expressed in uint8
    * rsort_lsb_unsigned_bytes_v1
    * tokenizer: default
    */
    buff = realloc(buff, sizeof(test_dates));

    if (buff == NULL)
    {
        fprintf(stderr, "test_rsort() - failed to reallocate buffer.");
        free(buff);
        return -1;
    }

    memcpy(buff, test_dates, sizeof(test_dates));

    rsort_lsb_unsigned_bytes_v1(buff, 8, 4, 1, 99, default_tokenizer);
    if (memcmp(buff, expect_dates, sizeof(test_dates)))
    {
        fprintf(stderr, "test rsort - ERROR [2]\n\n");
        free(buff);
        return -1;
    }

    /*
    * Test 3
    * List of 8 dates expressed in uint16
    * rsort_lsb_unsigned_bytes_v1
    * tokenizer: default
    */
    buff = realloc(buff, sizeof(test_dates2));

    if (buff == NULL)
    {
        fprintf(stderr, "test_rsort() - failed to reallocate buffer.");
        free(buff);
        return -1;
    }

    memcpy(buff, test_dates2, sizeof(test_dates2));

    rsort_lsb_unsigned_bytes_v1(buff, 8, 6, 1, 255, default_tokenizer);
    if (memcmp(buff, expect_dates2, sizeof(test_dates2)))
    {
        fprintf(stderr, "test rsort - ERROR [3]\n\n");
        free(buff);
        return -1;
    }

    free(buff);

    return 0;
}