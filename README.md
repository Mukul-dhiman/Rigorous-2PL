# Rigorous-2PL
OS assignment on implementing rigorous 2 phase locking.

## Usage:
1. Use command `make all` to generate the object file and binary.
1. `./main input_file_address output_file_address` for running the binary.
1. Use command `make clean` to delete the object file and binary.

## Header files:
* **transaction:** Transaction class creates objects to store the compononents of a transaction. transaction_id is stored as `int`, operations as a `vector<string>` and outcome as an `enum` with values either t_commit or t_abort.
* **lockmanager:** Lockmanager class is used to create a lock manager(monitor) object and it implements various methods related to acquiring and releasing of read-write locks. It has a lock_manager_lock of type `pthread_mutex_t`, list of condition variables of type `pthread_cond_t` for each state variable, a list of write lock denoting flags of type `bool` for each state variable and a list of `set` denoting the set of transactions holding read lock also for each state variable. It also has a list for storing logs related to all the threads of the process.