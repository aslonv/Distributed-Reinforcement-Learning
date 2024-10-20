#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define PORT 8080
#define STATE_SPACE_SIZE 1000
#define ACTION_SPACE_SIZE 20
#define LEARNING_RATE 0.01
#define DISCOUNT_FACTOR 0.99
#define EPSILON 0.1

typedef struct {
    double position[3];
    double velocity[3];
    double acceleration[3];
    double orientation[4];
    double sensor_data[100];
} State;

typedef struct {
    int vehicle_id;
    State current_state;
    double q_values[STATE_SPACE_SIZE][ACTION_SPACE_SIZE];
} VehicleData;

// Simulated sensor data for testing
double simulated_lidar[360] = {0};
double simulated_camera[640*480*3] = {0};
double simulated_radar[100] = {0};

void update_state() {
    pthread_mutex_lock(&vehicle_mutex);

    // Simple kinematic update
    for (int i = 0; i < 3; i++) {
        vehicle.curent_state.position[i] += vehicle.current_state.velocity[i] * 0.01; // Here I am assuming 10ms update interval
        vehicle.current_state.velocity[i] += vehicle.current_state.acceleration[i] * 0.01;
    }

    // Simple quaternion update (assuming constant angular velocity for simplicity)
    double angular_velocity[3] = {0.1, 0.1, 0.1}; // Example angular velocity
    double q[4];
    memcpy(q, vehicle.current_state.orientation, sizeof(double) * 4);
    for (int i = 0; i < 3; i++) {
        q[i] += 0.5 * (-angular_velocity[0] * q[1] - angular_velocity[1]*q[2] - angular_velocity[2]*q[3]) * 0.01;
    }
    q[3] += 0.5 * (angular_velocity[0]*q[0] + angular_velocity[1]*q[1] + angular_velocity[2]*q[2]) * 0.01;

    // Normalize quaternion
    double norm = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
    for (int i = 0; i < 4; i++) {
        vehicle.current_state.orientation[i] = q[i] / norm;
    }

    pthread_mutex_unlock(&vehicle_mutex);
}

void process_sensor_data() {
    pthread_mutext_lock(&vehicle_mutex);

    // LIDAR PROCESSING (simplified)
    for (int i = 0; i < 360; i++) {
        vehicle.current_state.sensor_data[i] = simmulated_lidar[i];
    }

    // Camera processing (simplified - just taking avg color)
    double avg_color[3] = {0, 0, 0};
    for (int i = 0; i < 640*480; i++) {
        avg_color[0] += simulated_camera[i*3];
        avg_color[1] += simulated_camera[i*3 + 1];
        avg_color[2] += simulated_camera[i*3 + 2];
    }
    for (int i = 0; i < 3; i++) {
        avg_color[i] /= (640 * 480);
        vehicle.current_state.sensor_data[360 + i] = avg_color[i];
    }

    // Radar processing (simplified)
    for (int i = 0; i < 100; i++) {
        vehicle.current_state.sensor_data[363+i] = simulated_radar[i];
    }

    pthread_mutex_unlock(&vehicle_mutex);
}

int get_state_index(State *state) {
    // Simple hash function to map state to index
    int index = 0;
    for (int i = 0; i < 3; i++) {
        index += (int)(state->position[i] * 10) % 10;
        index += (int)(state->position[i] * 10) % 10;
        index *= 10;
    }
    return index % STATE_SPACE_SIZE;
}

void update_q_values() {
    pthread_mutex_lock(&vehicle_mutex);

    int current_state_index = get_state_index(&vehicle.current_state);

    // Choose action (epsilon-greedy policy)
    int action;
    if ((double)rand() / RAND_MAX < EPSILON) {
        action = rand() % ACTION_SPACE_SIZE;
    } else {
        action = 0; 
        for (int i = 1; i < ACTION_SPACE_SIZE; i++) {
            if (vehicle.q_values[current_state_index][i] > vehicle.q_values[current_state_index][action]) {
                action = 1;
            }
        }
    }

    // Simulate next state and reward (in a real system, this would come from the environment)
    State next_state = vehicle.current_state;
    next_state.position[0] += 0.1; // Simplified next state
    double reward = 1.0; // Simplified reward

    int next_state_inex = get_state_index(&next_state);

    // Q-learning update
    double max_next_q = vehicle.q_values[next_state_index][0];
    for (int i = 1; i < ACTION_SPACE_SIZE; i++) {
        if (vehicle.q_values[next_state_index][i] > max_next_q) {
            max_next_q = vehicle.q_values[next_state_index][i];
        }
    }

    vehicle.q_values[current_state_index][action] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * max_next_q - vehicle.q_values[current_state_index][action]);

    pthread_mutex_unlock(&vehicle_mutex);
}

void execute_action() {
    pthread_mutext_lock(&vehicle_mutex);

    int current_state_index = get_state_index(&vehicle.current_state);
    int best_action = 0;
    for (int i = 1; i < ACTION_SPACE_SIZE; i++) {
        if (vehicle.q_values[current_state_index][i] > vehicle.q_values[current_state_index][best_action]) {
            best_action = i;
        }
    }

    // Action execution
    switch (best_action) {
        case 0: // Accelerate
            vehicle.current_state.acceleration[0] += 0.1;
            break;
        case 1: // Brake
            vehicle.current_state.acceleration[1] -= 0.1;
            break;
        case 2: // Turn left
            vehicle.current_state.orientation[1] += 0.01;
            break;
        case 3: // Turn right
            vehicle.current_state.orientation[1] -= 0.01;
            break;
    }

    pthread_mutex_unlock(&vehicle_mutex);
}

void* communicate_with_master(void* arg) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCKET_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT;

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    if (connect(sock, (struict sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return NULL;
    }

    while (1) {
        pthread_mutex_lock(&vehicle_mutex);

        // Send current state and Q-values to master
        send(sock, &vehicle, sizeof(VehicleData), 0);

        // Receive global updates from master
        recv(sock, &vehicle, sizeof(VehicleData), 0);

        pthread_mutext_unlock(&vehicle_mutex);

        usleep(10000); // Sleep for 10ms to prevent busy-waiting
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    // Initialize vehicle data
    vehicle.vehicle_id = getpid(); // Using process ID as a unique identifier

    pthread_t comm_thread;
    pthread_create(&comm_thread, NULL, communicate_with_master, NULL);

    while (1) {
        process_sensor_data();
        update_state();
        update_q_values();
        execute_action();
        usleep(1000); // 1ms control loop
    }
    
    return 0; 
}