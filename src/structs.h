#ifndef _STRUCTS_H_
#define _STRUCTS_H_

enum sensor_type {
    TIRE,   // Tire sensor type
    PMU     // Power Management Unit (PMU) sensor type
};

// Struct representing a generic sensor
typedef struct {
    enum sensor_type sensor_type;   // Type of the sensor (TIRE or PMU)
    void *sensor_data;              // Pointer to the sensor data
    int nr_operations;              // Number of operations
    int *operations_idxs;           // Array of indexes for operations
} sensor;

// Struct representing the power management unit sensor
typedef struct __attribute__((__packed__)) {
    float voltage;                  // Voltage value
    float current;                  // Current value
    float power_consumption;        // Power consumption value
    int energy_regen;               // Energy regeneration value
    int energy_storage;             // Energy storage value
} power_management_unit;

// Struct representing the tire sensor
typedef struct __attribute__((__packed__)) {
    float pressure;                 // Pressure value
    float temperature;              // Temperature value
    int wear_level;                 // Wear level value
    int performace_score;           // Performance score value
} tire_sensor;

#endif /* _STRUCTS_H_ */
