
#ifndef _HAKA_THREAD_H
#define _HAKA_THREAD_H

#include <stddef.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include <haka/types.h>
#include <haka/compiler.h>


/* Thread functions */

int      thread_get_packet_capture_cpu_count();
void     thread_set_packet_capture_cpu_count(int count);
int      thread_get_cpu_count();

typedef pthread_t thread_t;

#define THREAD_CANCELED  PTHREAD_CANCELED

bool     thread_create(thread_t *thread, void *(*main)(void*), void *param);
bool     thread_join(thread_t thread, void **ret);
bool     thread_cancel(thread_t thread);

int      thread_getid();
void     thread_setid(int id);

bool     thread_sigmask(int how, sigset_t *set, sigset_t *oldset);

enum thread_cancel_t {
	THREAD_CANCEL_DEFERRED,
	THREAD_CANCEL_ASYNCHRONOUS
};

bool     thread_setcancelstate(bool enable);
bool     thread_setcanceltype(enum thread_cancel_t type);
void     thread_testcancel();


/* Atomic counter */

typedef volatile uint32 atomic_t;

INLINE uint32 atomic_inc(atomic_t *v)
{
	return __sync_add_and_fetch(v, 1);
}

INLINE uint32 atomic_dec(atomic_t *v)
{
	return __sync_sub_and_fetch(v, 1);
}

INLINE uint32 atomic_get(atomic_t *v)
{
	return *v;
}

INLINE void atomic_set(atomic_t *v, uint32 x)
{
	*v = x;
}


/* Mutex */

typedef pthread_mutex_t mutex_t;

#define MUTEX_INIT PTHREAD_MUTEX_INITIALIZER

bool mutex_init(mutex_t *mutex, bool recursive);
bool mutex_destroy(mutex_t *mutex);
bool mutex_lock(mutex_t *mutex);
bool mutex_trylock(mutex_t *mutex);
bool mutex_unlock(mutex_t *mutex);


/* Read-Write Locks */

typedef pthread_rwlock_t rwlock_t;

#define RWLOCK_INIT PTHREAD_RWLOCK_INITIALIZER

bool rwlock_init(rwlock_t *rwlock);
bool rwlock_destroy(rwlock_t *rwlock);
bool rwlock_readlock(rwlock_t *rwlock);
bool rwlock_tryreadlock(rwlock_t *rwlock);
bool rwlock_writelock(rwlock_t *rwlock);
bool rwlock_trywritelock(rwlock_t *rwlock);
bool rwlock_unlock(rwlock_t *rwlock);


/* Semaphore */

typedef sem_t semaphore_t;

bool semaphore_init(semaphore_t *semaphore, uint32 initial);
bool semaphore_destroy(semaphore_t *semaphore);
bool semaphore_wait(semaphore_t *semaphore);
bool semaphore_post(semaphore_t *semaphore);


/* Barrier */

typedef pthread_barrier_t barrier_t;

bool barrier_init(barrier_t *barrier, uint32 count);
bool barrier_destroy(barrier_t *barrier);
bool barrier_wait(barrier_t *barrier);


/* Thread local storage */

typedef pthread_key_t local_storage_t;

bool local_storage_init(local_storage_t *key, void (*destructor)(void *));
bool local_storage_destroy(local_storage_t *key);
void *local_storage_get(local_storage_t *key);
bool local_storage_set(local_storage_t *key, const void *value);


#endif /* _HAKA_THREAD_H */
