/** @file   proj2.c
 *  @author Kristian Luptak (xluptak00)
 *  @date   25.4.2025
 *  @brief  simulates processes of cars, trucks, carriage
 */


#include "proj2.h"

#define TRUCK_SIZE 3
#define POSITIONS 2

// SEMAPHORES
// semaphores for writing / changing globals
sem_t* mutex;
sem_t* write_file;

// semaphores waiting lines for different positions
sem_t** trucks;
sem_t** cars;

// signaling semaphores
sem_t* unload_car;
sem_t* unloaded;
sem_t* loaded;

// SHARED VARIABLES
int* waiting_trucks; // stores how many trucks are waiting on each position (array)
int* waiting_cars; // stores how many car are waiting on each position (array)
int* command_num; // stores command number which will be writen in file
int* transport_count; // stores how many vehicles need to be loaded
int* current_load; // stores how many cars are currently loaded on the carriage

FILE* file; // file in which will be information written


int main(int argc, char*argv []) {
    if(argc != 6) {
        fprintf(stderr, "Invalid number of parameters, please use ./proj2 N K O TA TP\n");
        return 1;
    }
    
    int truck_count = check_input(argv[1], 0, 10000); // stores how many trucks need to be transported
    int car_count = check_input(argv[2], 0, 10000); // stores how many cars need to be transported
    int carriage_capacity = check_input(argv[3], 3, 100); // stores maximum capacity of carriage
    int car_time = check_input(argv[4], 0, 10000); // stores time it takes for car to travel to position
    int carriage_time = check_input(argv[5], 0, 1000);  // stores time  it takes for carriage to travel to position

    // open the file
    file = fopen("proj2.out", "w");
    if(file == NULL){ 
        fprintf(stderr, "Failed to open file proj2.out\n");
        return 1;
    }

    init(); // initiate semaphores
    
    // default values
    *command_num = 1;
    *current_load = 0;
    *transport_count = 0;
    waiting_trucks[0] = waiting_trucks[1]= 0;
    waiting_cars[0] = waiting_cars[1]= 0;

    // carriage process
    pid_t id = fork();
    if(id == 0){
        carriage(carriage_time);
    }
    else if (id == -1) {
       err();
    }

    // car processes
    for(int i = 1; i <= car_count; i++) {
        pid_t id = fork();
        if(id == 0) {
            car(car_time, i, carriage_capacity);
        }
        else if(id == -1) {
            err();
        }
    }

    //truck processes
    for(int i = 1; i <= truck_count; i++) {
        pid_t id = fork();
        if(id == 0) {
            truck(car_time, i, carriage_capacity);
        }
        else if(id == -1) {
            err();
        }
    }
    // wait for all processes to finish
    while(wait(NULL) > 0);

    // unload everything
    destroy();
    fclose(file);
    return 0;
}


/** @brief converts input argument into 'int', checks the bounds for the argument
 *  @param max maximum the value 'input' can reach
 *  @param min minimum the value 'input' can reach
 *  @param input argument to be processed
 *  @return argument 'input' converted to int
 */
int check_input(char *input, int min, int max) {
    char* err;
    int value =(int) strtol(input, &err, 10);
    // exit on errors
    if(*err != '\0') {
        fprintf(stderr, "Parameter '%s' is not a valid parameter\n", input);
        exit(1);
    }
    if(value < min || value > max) {
        fprintf(stderr, "Parameter '%d' is outside of range <%d, %d>\n", value, min, max);
        exit(1);
    }
    return value;
}


/** @brief function for processing car 'life', taking it from 'start pos' to 'end pos'
 *  @param car_time time it takes car to arrive to 'start pos'
 *  @param car_idx id of the car
 *  @param max_load 'maximum load of carriage'
 *  @return nothing, exit success on end
 */
void car(int car_time, int car_idx, int max_load) {
    sem_wait(mutex);
    (*transport_count)++;
    sem_post(mutex);
    srand(time(NULL) * getpid());
    int start_pos = rand() % POSITIONS; // random starting pos

    // car starts
    write_f("O %d: started\n", car_idx);

    // wait random time 
    usleep(rand() % (car_time + 1));

    // car has arrived to destination, waiting
    sem_wait(mutex);
    waiting_cars[start_pos]++;
    write_f("O %d: arrived to %d\n", car_idx, start_pos);
    sem_post(mutex);

    // wait for singal to be loaded
    sem_wait(cars[start_pos]);

    // load the car
    sem_wait(mutex);
    waiting_cars[start_pos]--;
    (*current_load)++;
    write_f("O %d: boarding\n", car_idx);

    //if theres still space for truck board truck
    if (*current_load + TRUCK_SIZE <= max_load && waiting_trucks[start_pos] > 0) {
        sem_post(trucks[start_pos]);
    // otherwise board car
    } else if (waiting_cars[start_pos] > 0 && *current_load < max_load) {
        sem_post(cars[start_pos]);
    // everything is loaded
    } else {
        sem_post(loaded);
    }
    sem_post(mutex);


    // wait for unload signal
    sem_wait(unload_car);

    //unload car 
    sem_wait(mutex);
    (*current_load)--;
    (*transport_count)--;
    write_f("O %d: leaving in %d\n", car_idx, (start_pos + 1) % POSITIONS);

    if(*current_load != 0) {

        sem_post(unload_car);
    }
    else{
        // all cars have finished unloading
        sem_post(unloaded);
    }
    sem_post(mutex);
    exit(0);
}


/** @brief function for processing truck 'life', taking it from 'start pos' to 'end pos'
 *  @param truck_time time it takes truck to arrive to 'start pos'
 *  @param truck_idx id of the truck
 *  @param max_load maximum load of carriage
 *  @return nothing, exit success on end
 */
void truck(int truck_time, int truck_idx, int max_load) {
    sem_wait(mutex);
    (*transport_count)++;
    sem_post(mutex);
    srand(time(NULL) * getpid());
    int start_pos = rand() % POSITIONS;

    // truck starts
    write_f("N %d: started\n", truck_idx);

    // wait random time 
    usleep(rand() % (truck_time + 1));

    // truck has arrived to destination, waiting
    sem_wait(mutex);
    waiting_trucks[start_pos]++;
    write_f("N %d: arrived to %d\n",truck_idx, start_pos);
    sem_post(mutex);
    
    sem_wait(trucks[start_pos]);

    // boarding truck
    sem_wait(mutex);
    write_f("N %d: boarding\n", truck_idx);
    (*current_load) += TRUCK_SIZE;
    waiting_trucks[start_pos]--;
    sem_post(mutex);

    sem_wait(mutex);
    // while there is still space on carriage
    if (*current_load < max_load) {
        // if there are still cars we can load them alternatively
        if (waiting_cars[start_pos] > 0) {
            sem_post(cars[start_pos]);
        }
        // or try to load truck
        else if (*current_load + TRUCK_SIZE <= max_load && waiting_trucks[start_pos] > 0) {
            sem_post(trucks[start_pos]);
        } 
        // otherwise therese nothing more to load
        else {
            sem_post(loaded);
        }
    } else {
        sem_post(loaded);
    }
    sem_post(mutex);

    // wait for unload signal
    sem_wait(unload_car);

    //unload all loaded wehicles
    sem_wait(mutex);
    (*current_load) -= TRUCK_SIZE;
    (*transport_count)--;
    write_f("N %d: leaving in %d\n", truck_idx, ((start_pos + 1) % POSITIONS));

    if(*current_load != 0) {

        sem_post(unload_car);
    }
    else{
        // all cars have finished unloading
        sem_post(unloaded);
    }
    sem_post(mutex);

    exit(0); // car process finished
}


/** @brief function for processing carriage 'life'. Carriage transports cars and trucks
 *         from their starting positions to destinations
 *  @param carriage_time time it takes for carriage to travel
 *  @return nothing, exit success on end
 */
void carriage(int carriage_time) {
    srand(time(NULL));
    // cariage process has started
    write_f("P: started\n");
    int PR = 0; // carriage starts at pos 0

    while(true){
        // travel to dock num (0 or 1)
        usleep(rand() % (carriage_time + 1));

        write_f("P: arrived to %d\n", PR);
        
        // firstly unload cars if there are any
        if(*current_load != 0){
            sem_post(unload_car);
            sem_wait(unloaded);
        }

        // there are no more wehicles to be transported
        sem_wait(mutex);
        if(*transport_count == 0) {
            sem_post(mutex);
            break;
        }
        sem_post(mutex);

        // loading cars if there are any, starting with trucks
        if(waiting_trucks[PR] > 0) {
            sem_post(trucks[PR]);
            sem_wait(loaded);
        }
        else if(waiting_cars[PR] > 0){
            sem_post(cars[PR]);
            sem_wait(loaded);
        }

        // going to the next stop
        write_f("P: leaving %d\n", PR);
        PR = (PR + 1) % POSITIONS; // PR = 1 - PR could be used here 
    }
    // all cars are transported
    write_f("P: leaving %d\n", PR);

    //sleep random time, (going to docks)
    usleep(rand() % (carriage_time + 1));

    //finished
    write_f("P: finish\n");
    exit(0);
}


/** @brief Iniatilizes all shared memories and semaphores
 */
void init() {
    // semaphores for writing, changing globals
    if((mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    if((sem_init(mutex, 1, 1)) == -1) err();

    if((write_file = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    if((sem_init(write_file, 1, 1)) == -1) err();

    // semaphores waiting lines for different positions
    if((trucks = mmap(NULL, sizeof(sem_t*) * POSITIONS, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    for(int i = 0; i < POSITIONS; i++) {
        if((trucks[i] = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    }

    if((cars = mmap(NULL, sizeof(sem_t*) * POSITIONS, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    for(int i = 0; i < POSITIONS; i++) {
        if((cars[i] = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    }

    for(int i = 0; i < POSITIONS; i++) {
        if((sem_init(cars[i], 1, 0)) == -1) err();
        if((sem_init(trucks[i], 1, 0)) == -1) err();
    }

    // semaphores for signaling
    if((unload_car = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    if((sem_init(unload_car, 1, 0)) == -1) err();

    if((unloaded = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    if((sem_init(unloaded, 1, 0)) == -1) err();

    if((loaded = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    if((sem_init(loaded, 1, 0)) == -1) err();

    // shared memory variables
    if((transport_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    if((command_num = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    if((current_load = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();

    if((waiting_trucks = mmap(NULL, sizeof(int) * POSITIONS, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
    if((waiting_cars = mmap(NULL, sizeof(int) * POSITIONS, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED) err();
}


/** @brief destroys all shared memories, semaphores
 */
void destroy() {
    // semaphores for writing, changing globals
    sem_destroy(mutex);
    munmap(mutex, sizeof(sem_t));

    sem_destroy(write_file);
    munmap(write_file, sizeof(sem_t));

    // semaphores waiting lines for different positions
    for(int i = 0; i< POSITIONS;i++){
        sem_destroy(trucks[i]);
        sem_destroy(cars[i]);
        munmap(trucks[i], sizeof(sem_t));
        munmap(cars[i], sizeof(sem_t));
    }

    munmap(cars, sizeof(sem_t*) * POSITIONS);
    munmap(trucks, sizeof(sem_t*) * POSITIONS);

    // signalling semaphores
    sem_destroy(unload_car);
    munmap(unload_car, sizeof(sem_t));

    sem_destroy(unloaded);
    munmap(unloaded, sizeof(sem_t));

    sem_destroy(loaded);
    munmap(loaded, sizeof(sem_t));

    // shared memory variables
    munmap(current_load, sizeof(int));
    munmap(transport_count, sizeof(int));
    munmap(command_num, sizeof(int));
    munmap(waiting_trucks, sizeof(int) * POSITIONS);
    munmap(waiting_cars, sizeof(int) * POSITIONS);
}


/** @brief writes message into file 
 *  @param fmt message to be writen into the file
 *  @returns nothing
 */
void write_f(const char *fmt, ...) {
    sem_wait(write_file);
    va_list args;
    va_start(args, fmt);
    fprintf(file, "%d: ", *command_num);
    (*command_num)++;
    vfprintf(file, fmt, args);
    fflush(file);
    va_end(args);
    sem_post(write_file);
}

void err() {
    fclose(file);
    destroy();
    exit(1);
}