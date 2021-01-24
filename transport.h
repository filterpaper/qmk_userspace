#include <string.h>
#include <stddef.h>

#include "config.h"
#include "matrix.h"
#include "quantum.h"

bool transport_master(matrix_row_t matrix[]);
void transport_slave(matrix_row_t matrix[]);
void transport_master_init(void);
void transport_slave_init(void);

#ifdef USE_SERIAL
#    include "serial.h"
void transport_rgblight_master(void);
void transport_rgblight_slave(void);
#endif
