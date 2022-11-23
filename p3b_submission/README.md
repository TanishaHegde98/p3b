We have created two new system calls to implement multithreading in xv6.
The new system calls created are: sys_clone() and sys_join() which are defined in the sysproc.c file.
sys_clone() internally calls the function int clone(void(*fcn)(void *, void *), void *arg1, void *arg2, void *stack) and int join(void ** stack) which are defined in the proc.c file.
We have also created 4 new user functions int thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2),int thread_join()
void lock_acquire(lock_t *) and void lock_release(lock_t *) which are defined in the ulib.c
In order to implement multi-threading, we also had to make a few changes in the growproc(int n), exit() and wait() functions defined in the proc.c file.
