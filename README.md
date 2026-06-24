_This project has been created as part of the 42 curriculum by abroslav._ 

## Description
This project is a simulation of the "Dining Philosophers" problem, designed to explore the fundamentals of multi-threading and synchronization in C. 

The scenario involves philosophers sitting at a round table with a bowl of spaghetti. They alternate between eating, thinking, and sleeping. To eat, a philosopher must hold two forks—one from their left and one from their right. In this implementation, each philosopher is a separate thread, and each fork is protected by a mutex to prevent data races. The simulation stops if a philosopher dies of starvation. 

## Instructions

### Compilation
The project is compiled using the provided Makefile. To compile the program, navigate to the `philo/` directory and run:

```bash
make
```

This will generate the `philo` executable using the flags `-Wall -Wextra -Werror`.

## Execution

The program accepts the following arguments:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```
* **number_of_philosophers**: The number of philosophers and also the number of forks.
* **time_to_die** (in milliseconds): If a philosopher hasn't started eating within this time since their last meal, they die.
* **time_to_eat** (in milliseconds): The time it takes for a philosopher to eat while holding two forks.
* **time_to_sleep** (in milliseconds): The time a philosopher spends sleeping.
* **number_of_times_each_philosopher_must_eat** (optional): If all philosophers have eaten at least this many times, the simulation stops.

## Resources

### References
* **Pthreads**: Documentation for thread creation and management (`pthread_create`, `pthread_join`).
* **Mutexes**: Documentation for data synchronization (`pthread_mutex_lock`, `pthread_mutex_unlock`).
* **Time**: Documentation for `gettimeofday` to handle precise timestamps.

#### AI was used in this project for the following tasks:

* **Logic Validation**: Ensuring the parity logic (odd/even IDs) effectively prevents deadlocks.
* **Refinement**: Assistance in structuring the `smart_sleep` function to improve simulation responsiveness.

