#ifndef BITMASK_H
#define BITMASK_H


// #define BITMASK(type, out, start, end, flip) ({                \
//     if (flip)                                                  \
//     {                                                          \
//         type mask = 0;                                         \
//         for (int i = start; i <= end; ++i)                     \
//         {                                                      \
//             mask |= (1ULL << i); /* sets bit, or leaves it alone if already set */ \
//         }                                                      \
//         out = mask;                                            \
//     } else                                                     \
//     {                                                          \
//         type mask = ~0;                                        \
//         for (int i = start; i <= end; ++i)                     \
//         {                                                      \
//             mask &= ~(1ULL << i);             /* clears bit */ \
//         }                                                      \
//         out = mask;                                            \
//     }                                                          \
// })

#define BITMASK(type, out, start, end, flip) ({                           \
    type mask = 0;                                                        \
    if (flip)                                                             \
    {                                                                     \
        mask = ((1ULL << ((end) + 1)) - 1) ^ ((1ULL << (start)) - 1);     \
    } else                                                                \
    {                                                                     \
        mask = ~(((1ULL << ((end) + 1)) - 1) ^ ((1ULL << (start)) - 1));  \
    }                                                                     \
    out = mask;                                                           \
})

#define PRINT_BITMASK(type, bitmask, mode) ({                             \
    if (mode == 0)                                                        \
    {                                                                     \
        /* Binary */                                                      \
        fprintf(stdout, "PRINT_BITMASK() - ");                            \
        for (int i = sizeof(type) * 8 - 1; i >= 0; --i)                   \
        {                                                                 \
            fprintf(stdout, "%d", (bitmask >> i) & 1);                    \
        }                                                                 \
        fprintf(stdout, "\n");                                            \
    } else if (mode == 1)                                                 \
    {                                                                     \
        /* Hex */                                                         \
        fprintf(stdout, "PRINT_BITMASK() - 0x%016llX\n", bitmask);        \
    } else {                                                              \
        fprintf(stderr, "PRINT_BITMASK() - Invalid mode!\n");             \
    }                                                                     \
})

#define TO_BINARY_STRING(type, in) ({                                     \
    static char out[sizeof(type) * 8 + 1];                                \
    size_t size = sizeof(type) * 8;                                       \
    for (int i = 0; i < size; ++i) {                                      \
        out[i] = ((in >> (size - 1 - i)) & 1) ? '1' : '0';                \
    }                                                                     \
    out[size] = '\0';                                                     \
    out;                                                                  \
})

#endif /* BITMASK_H */