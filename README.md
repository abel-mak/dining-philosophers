# dining-philosophers
dining philosophers problem
## notes
1 -pthread_mutex_lock(&N_mutex); ensures that exactly ONE thread is sucessful in locking the mutex variable N_mutex.
This particular thread will then be the only thread that will update the variable N, thus ensuring that N is updated sequential (one thread after another)

