#ifndef V_ASSEMBLY_H
#define V_ASSEMBLY_H

#include <stddef.h>

typedef struct {
    unsigned val;
}coord_t;

double v_assembly(unsigned degree, coord_t* x, coord_t* y);

#endif
