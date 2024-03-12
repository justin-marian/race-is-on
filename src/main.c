#include "operations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Prints the data of a specific sensor.
 *
 * @param index_sensor Index of the sensor to print.
 * @param nr_sensors Number of sensors in the array.
 * @param sensors Array of sensor data.
 */
void print(int index_sensor, int nr_sensors, sensor *sensors)
{
	if (index_sensor < 0 || index_sensor >= nr_sensors) {
		printf("Index not in range!\n");
		return;
	}

	if (sensors[index_sensor].sensor_type == TIRE) {
		tire_sensor *tire =
		(tire_sensor *)sensors[index_sensor].sensor_data;

		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", tire->pressure);
		printf("Temperature: %.2f\n", tire->temperature);
		printf("Wear Level: %d%%\n", tire->wear_level);
		if (tire->performace_score != 0)
			printf("Performance Score: %d\n", tire->performace_score);
		else
			printf("Performance Score: Not Calculated\n");
	} else if (sensors[index_sensor].sensor_type == PMU) {
		pmu_sensor *pmu =
		(pmu_sensor *)sensors[index_sensor].sensor_data;

		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", pmu->voltage);
		printf("Current: %.2f\n", pmu->current);
		printf("Power Consumption: %.2f\n", pmu->power_consumption);
		printf("Energy Regen: %d%%\n", pmu->energy_regen);
		printf("Energy Storage: %d%%\n", pmu->energy_storage);
	} else {
		printf("Index not in range\n");
	}
}

/**
 * @brief Frees memory allocated for sensor data and operation indexes.
 *
 * @param sensors Array of sensor data.
 * @param nr_sensors Number of sensors in the array.
 */
void clear_memory(sensor *sensors, int nr_sensors)
{
	for (int i = 0; i < nr_sensors; i++) {
		if (sensors[i].sensor_data)
			free(sensors[i].sensor_data);
		if (sensors[i].operations_idxs)
			free(sensors[i].operations_idxs);
	}
	free(sensors);
}

/**
 * @brief Checks if the tire sensor data is valid.
 * Checks if:
 *  - the pressure is between 19 and 28
 *	- the temperature is between 0 and 120
 *	- the wear_level is between 0 and 100.
 * @param s Pointer to the sensor data.
 * @return True if the data is valid, false otherwise.
 */
static inline bool check_tire(sensor *s)
{
	tire_sensor *tire_data = (tire_sensor *)s->sensor_data;
	return tire_data->pressure >= 19 && tire_data->pressure <= 28 &&
		tire_data->temperature >= 0 && tire_data->temperature <= 120 &&
		tire_data->wear_level >= 0 && tire_data->wear_level <= 100;
}

/**
 * @brief Checks if the PMU sensor data is valid.
 * Checks if:
 *  - the voltage is between 10 and 20
 *  - the current is between -100 and 100
 *  - the power_consumption is between 0 and 1000
 *  - the energy_regen is between 0 and 100
 *  - the energy_storage is between 0 and 100.
 * @param s Pointer to the sensor data.
 * @return True if the data is valid, false otherwise.
 */
static inline bool check_pmu(sensor *s)
{
	pmu_sensor *pmu_data = (pmu_sensor *)s->sensor_data;
	return pmu_data->voltage >= 10 && pmu_data->voltage <= 20 &&
		pmu_data->current >= -100 && pmu_data->current <= 100 &&
		pmu_data->power_consumption >= 0 &&
		pmu_data->power_consumption <= 1000 &&
		pmu_data->energy_regen >= 0 && pmu_data->energy_regen <= 100 &&
		pmu_data->energy_storage >= 0 && pmu_data->energy_storage <= 100;
}

/**
 * @brief Updates the tire sensor data.
 * @param s Pointer to the sensor data to be updated.
 * @param tire_data Pointer to the new tire sensor data.
 */
static inline void change_tire(tire_sensor *s, tire_sensor *tire_data)
{
	s->pressure = tire_data->pressure;
	s->temperature = tire_data->temperature;
	s->wear_level = tire_data->wear_level;
	s->performace_score = tire_data->performace_score;
}

/**
 * @brief Updates the PMU sensor data.
 *
 * @param s Pointer to the sensor data to be updated.
 * @param pmu_data Pointer to the new PMU sensor data.
 */
static inline void change_pmu(pmu_sensor *s, pmu_sensor *pmu_data)
{
	s->voltage = pmu_data->voltage;
	s->current = pmu_data->current;
	s->power_consumption = pmu_data->power_consumption;
	s->energy_regen = pmu_data->energy_regen;
	s->energy_storage = pmu_data->energy_storage;
}

/**
 * @brief Filters and returns an array of valid sensors from the given array.
 *
 * @param nr_sensors Pointer to the number of sensors in the original array.
 *                   Updated to the number of valid sensors.
 * @param sensors Array of sensor data.
 *
 * @return Pointer to the array of valid sensors.
 */
sensor *clear_erroneous_sensors(int *nr_sensors, sensor *sensors)
{
	// Allocate memory for array of valid sensors
	sensor *valid_sensors = malloc(*nr_sensors * sizeof(sensor));

	int nr_valid = 0;

	// Check each sensor for validity
	for (int i = 0; i < *nr_sensors; i++) {
		if (sensors[i].sensor_type == TIRE && check_tire(&sensors[i])) {
			tire_sensor *tire_data = (tire_sensor *)sensors[i].sensor_data;
			valid_sensors[nr_valid].sensor_type = TIRE;
			valid_sensors[nr_valid].sensor_data = malloc(sizeof(tire_sensor));

			tire_sensor *tire_data_s =
			(tire_sensor *)valid_sensors[nr_valid].sensor_data;

			change_tire(tire_data_s, tire_data);
		}
		if (sensors[i].sensor_type == PMU && check_pmu(&sensors[i])) {
			pmu_sensor *pmu_data = (pmu_sensor *)sensors[i].sensor_data;
			valid_sensors[nr_valid].sensor_type = PMU;
			valid_sensors[nr_valid].sensor_data = malloc(sizeof(pmu_sensor));

			pmu_sensor *pmu_data_s =
			(pmu_sensor *)valid_sensors[nr_valid].sensor_data;

			change_pmu(pmu_data_s, pmu_data);
		}
		if ((sensors[i].sensor_type == PMU && check_pmu(&sensors[i])) ||
			(check_tire(&sensors[i]) && sensors[i].sensor_type == TIRE)) {
			valid_sensors[nr_valid].nr_operations =
			sensors[i].nr_operations;

			valid_sensors[nr_valid].operations_idxs =
			malloc(sensors[i].nr_operations * sizeof(int));
			for (int j = 0; j < sensors[i].nr_operations; j++) {
				valid_sensors[nr_valid].operations_idxs[j] =
				sensors[i].operations_idxs[j];
			}
			nr_valid++;
		}
	}

	clear_memory(sensors, *nr_sensors);
	valid_sensors = realloc(valid_sensors, nr_valid * sizeof(sensor));

	*nr_sensors = nr_valid;
	return valid_sensors;
}

/**
 * @brief Analyzes a specific sensor using its operations.
 *
 * @param index Index of the sensor to analyze.
 * @param sensors Array of sensor data.
 */
void analyze(int index, sensor *sensors)
{
	void (**operations) (void *) =
	(void (**)(void *)) malloc(MAX_NR_OPS * sizeof(void (*)(void *)));
	get_operations((void **)operations);

	void *sensor_data = NULL;
	if (sensors[index].sensor_type == TIRE) {
		tire_sensor *tire_sensor_data = (tire_sensor *)
		sensors[index].sensor_data;
		sensor_data = tire_sensor_data;
	} else if (sensors[index].sensor_type == PMU) {
		pmu_sensor *pmu_sensor_data = (pmu_sensor *)
		sensors[index].sensor_data;
		sensor_data = pmu_sensor_data;
	}

	for (int i = 0; i < sensors[index].nr_operations; i++)
		operations[sensors[index].operations_idxs[i]](sensor_data);
	free(operations);
}

/**
 * @brief Sorts the sensors array by separating PMU and TIRE sensors.
 *
 * @param nr_sensors Number of sensors in the array.
 * @param sensors Array of sensor data to be sorted.
 */
void sort_sensors(int nr_sensors, sensor *sensors)
{
	int pmu_count = 0;
	int tire_count = 0;
	// Count the number of PMU and TIRE sensors
	for (int i = 0; i < nr_sensors; ++i) {
		if (sensors[i].sensor_type == PMU)
			pmu_count++;
		else if (sensors[i].sensor_type == TIRE)
			tire_count++;
	}

	// Create arrays to hold the PMU and TIRE sensors
	sensor *pmu_sensors = malloc(pmu_count * sizeof(sensor));
	sensor *tire_sensors = malloc(tire_count * sizeof(sensor));

	int pmu_idx = 0;
	int tire_idx = 0;
	// Copy the PMU and TIRE sensors to their respective arrays
	for (int i = 0; i < nr_sensors; ++i) {
		if (sensors[i].sensor_type == PMU)
			memcpy(&pmu_sensors[pmu_idx++], &sensors[i], sizeof(sensor));
		else if (sensors[i].sensor_type == TIRE)
			memcpy(&tire_sensors[tire_idx++], &sensors[i], sizeof(sensor));
	}

	int idx = 0;
	// Merge the PMU and TIRE sensor arrays into the final sensors array
	for (int i = 0; i < pmu_count; i++)
		memcpy(&sensors[idx++], &pmu_sensors[i], sizeof(sensor));
	for (int i = 0; i < tire_count; i++)
		memcpy(&sensors[idx++], &tire_sensors[i], sizeof(sensor));

	// Free the PMU and TIRE sensor arrays
	free(pmu_sensors);
	free(tire_sensors);
}

/**
 * @brief Parses the .dat file and populates the sensors array.
 *
 * @param fd_dat File pointer to the .dat file.
 * @param sensors Array of sensor data.
 * @param nr_sensors Pointer to the number of sensors.
 *                   Updated to the number of sensors read from the file.
 *
 * @return Pointer to the array of sensor data.
 */
sensor *parser_dat_file(FILE *fd_dat, sensor *sensors, int *nr_sensors)
{
	// Read number of sensors from .dat
	fread(nr_sensors, sizeof(int), 1, fd_dat);
	// Allocate memory for sensors
	sensors = calloc(*nr_sensors, sizeof(sensor));

	for (int i = 0; i < *nr_sensors; i++) {
		int sensor_type;
		// Read type of sensor TIRE/UPB
		fread(&sensor_type, sizeof(int), 1, fd_dat);

		void *data = NULL; // sensor data

		int nr_ops = 0;
		int *sensor_ops = NULL;

		switch (sensor_type) {
		case TIRE:
			data = (tire_sensor *)calloc(1, sizeof(tire_sensor));
			// Parse tire sensor data TIRE
			// pressure, temperature, wear_level, performance_score
			fread(&((ts *)data)->pressure, sizeof(float), 1, fd_dat);
			fread(&((ts *)data)->temperature, sizeof(float), 1, fd_dat);
			fread(&((ts *)data)->wear_level, sizeof(int), 1, fd_dat);
			fread(&((ts *)data)->performace_score, sizeof(int), 1, fd_dat);
			break;
		case PMU:
			data = (pmu_sensor *)calloc(1, sizeof(pmu_sensor));
			// Parse tire sensor data PMU
			// voltage, current, power_consumption, energy_regen, energy_storage
			fread(&((pmu *)data)->voltage, sizeof(float), 1, fd_dat);
			fread(&((pmu *)data)->current, sizeof(float), 1, fd_dat);
			fread(&((pmu *)data)->power_consumption, sizeof(float), 1, fd_dat);
			fread(&((pmu *)data)->energy_regen, sizeof(int), 1, fd_dat);
			fread(&((pmu *)data)->energy_storage, sizeof(int), 1, fd_dat);
			break;
		default:
			printf("Unknown sensor type!\n");
			exit(EXIT_FAILURE);
		}

		fread(&nr_ops, sizeof(int), 1, fd_dat);
		sensor_ops = calloc(nr_ops, sizeof(int));
		if (!sensor_ops) {
			printf("Failed to allocate memory for sensor operations!\n");
			exit(EXIT_FAILURE);
		}
		fread(sensor_ops, sizeof(int), nr_ops, fd_dat);

		sensors[i] = (sensor){
			.sensor_type = sensor_type,
			.sensor_data = data,
			.nr_operations = nr_ops,
			.operations_idxs = sensor_ops
		};
	}
	fclose(fd_dat);
	return sensors;
}

int main(int argc, char const **argv)
{
	FILE *fd_dat = fopen(argv[1], "rb");

	int nr_sensors = 0;
	sensor *sensors = NULL;
	sensors = parser_dat_file(fd_dat, sensors, &nr_sensors);

	sort_sensors(nr_sensors, sensors);

	char *cmd = malloc(CMD_SIZ * sizeof(char));
	while (true) {
		int index;
		scanf("%s", cmd);
		if (!strncmp(cmd, "print", PRINT_SIZ)) {
			scanf("%d", &index);
			print(index, nr_sensors, sensors);
		} else if (!strncmp(cmd, "analyze", ANALYZE_SIZ)) {
			scanf("%d", &index);
			analyze(index, sensors);
		} else if (!strncmp(cmd, "clear", CLEAR_SIZ)) {
			sensors = clear_erroneous_sensors(&nr_sensors, sensors);
		} else  if (!strncmp(cmd, "exit", EXIT_SIZ)) {
			clear_memory(sensors, nr_sensors);
			break;
		}
	}

	free(cmd);
	return EXIT_SUCCESS;
}
