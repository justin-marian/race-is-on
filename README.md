# Race-Is-On

## Description

Simple sensor data analysis ***mini project***, comprising arrays of `Tire Sensors` and `Power Management Unit Sensors`. It's designed to enhance racing strategies by providing real-time insights into the car's performance and condition.

## Operations

- **Monitoring:** Tracks tire and power unit conditions.
- **Adaptation:** Enables quick response to changing race conditions.
- **Maintenance:** Identifies and mitigates potential failures.
- **Prioritization:** Focuses analysis on critical power management data.
- **Customization:** Offers `8` tailored operations for in-depth analysis, including **pressure, temperature, and wear level for tires, along with power, energy regeneration, usage, and battery health for power units**.

## Print

**Format Print:** concise output of sensor data, it distinguishes between **Tire sensors** and **Power Management Unit (PMU) sensors**, formatting their data distinctly to ensure easy interpretation.

- For *Tire sensors*, it displays **pressure, temperature, wear level, and performance score,** offering insights into tire condition and effectiveness.

```BASH
Tire Sensor Pressure: `<pressure>`
Temperature: `<temperature>` Wear
Level: `<wear_level>`
Performance Score: `<computed  score>`/Not Calculated
```

- For *PMU sensors*, it shows **voltage, current, power consumption, energy regeneration, and energy storage levels,** highlighting the car's energy efficiency and electrical system performance.

```BASH
Power Management Unit
Voltage: `<voltage>`
Current: `<current>`
Power Consumption: `<power_consumption>`
Energy Regen: `<energy_regen>`
Energy Storage: `<energy_storage>`
```

## Sort

Organizes the `sensors` array by `sensor_type`:

- Counts and categorizes sensors as `PMU` or `TIRE`.
- Allocates separate temporary arrays for each `sensor_type`.
- Copies sensors to the respective `pmu_sensors` or `tire_sensors` array based on their type.
- Combines both sorted arrays back into the original `sensors` array.
- `Power Management Unit` sensor data is prioritized over `Tire Sensor` data.

## Analyze

Performs a series of sensor operations at a specified index:

- Initiates an array of function pointers, `operations`, dynamically allocated for storing operations applicable to the sensor data.
- Utilizes `get_operations` to fill `operations` with pointers based on sensor type (`pmu_sensor` or `tire_sensor`).
- Fetches operation indices from `operations_idxs` for the target sensor and executes each using the sensor's data.
- Operations for `tire_sensor` include **pressure status, temperature status, wear level status, and performance score** calculation.
- For `pmu_sensor`, operations involve **computing power, regenerating energy, assessing energy usage, and battery health** evaluation.

## Clear

Processes the `sensors` array to filter and keep only valid sensors:

- Allocates memory for an array to hold valid sensors and sets up a counter for tracking.
- Iterates over the sensors, checking the `sensor_type` field to categorize as `TIRE` or `PMU`.
- For valid `TIRE` sensors, it creates a new `tire_sensor` object, transfers data, and updates the entry in `valid_sensors`.
- It replicates the `nr_operations` and dynamically allocates and copies `operations_idxs` for each valid sensor.
- After validating and segregating sensors, the original sensors array memory is cleared.
- Resizes the `valid_sensors` array to match the count of valid sensors and returns it.
- Ensures any allocated memory for sensor data and indices is freed appropriately.
