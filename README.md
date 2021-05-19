# dining-philosophers
dining philosophers problem
## notes
- <ins>pthread_mutex_lock(&N_mutex)</ins> ensures that exactly ONE thread is sucessful in locking the mutex variable N_mutex.
This particular thread will then be the only thread that will update the variable N, thus ensuring that N is updated sequential (one thread after another)
- So <ins>Thread Serialization</ins> means to make sure that a certain set of events occurs in a sequence not at the same time [(src)](https://stackoverflow.com/questions/14654230/what-does-it-mean-by-thread-serialization-in-c#:~:text=So%20Thread%20Serialization%20means%20to,occur%20at%20the%20same%20time.)

