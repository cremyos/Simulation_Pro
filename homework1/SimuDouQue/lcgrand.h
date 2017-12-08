#ifndef LCGRAND_H
#define LCGRAND_H

#include <stdio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

    extern float lcgrand(int stream);
    extern void  lcgrandst(long zset, int stream);
    extern long  lcgrandgt(int stream);

#ifdef __cplusplus
}
#endif


#endif // LCGRAND_H
