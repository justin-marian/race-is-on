#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include "structs.h"

#define CMD_SIZ 15
#define EXIT_SIZ 4
#define PRINT_SIZ 5
#define CLEAR_SIZ 5
#define ANALYZE_SIZ 7
#define MAX_NR_OPS 8

typedef power_management_unit pmu_sensor;
typedef tire_sensor ts;
typedef pmu_sensor pmu;

/**
 * @brief Array of pointers to various operations supported by the sensors.
 *
 * Populates the array 'operations' with pointers to various operations
 * that can be performed on the sensors. The array should be pre-allocated
 * with enough space to hold all the operation pointers.
 *
 * @param[out] operations A pointer to the array of operation pointers.
 * `operations` will be populated with function pointers.
 *
 * @note The array should be pre-allocated with at least 'MAX_NR_OPS' elements.
 */
void get_operations(void **operations);

#endif /* _OPERATIONS_H_ */