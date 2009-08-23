/**
 * \file erregrwlock.h 
 * \brief liberreg -  read and write lock routines
 *                    Before accessing registry, you should set lock on registry.
 *                    using read lock for reading registry
 *                    using write lock for writing registry
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _ERREG_RWLOCK_H_
#define _ERREG_RWLOCK_H_

#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum
        {
            lock_none,
            lock_read,
            lock_write
        } lock_state_t;

/**
 * Initilize resource for rwlock when launching an application which wants to access registry 
 *
 * @returns TRUE means initialization succeeds, otherwise returns FALSE 
 */
gboolean erRegRWLockInit(void);

/**
 * destroy resource for rwlock when exit an application which calls erRegRWLockInit() before 
 *
 */
void erRegRWLockDestroy(void);

/**
 * put read lock on registry, call it when reading registry 
 * notes: don't forget using erRegUnlock() to release the lock afterwards
 *
 */
gboolean erRegReadLock(void);

/**
 * put write lock on registry, call it when writing registry 
 * notes: don't forget using erRegUnlock() to release the lock afterwards
 *
 */
gboolean erRegWriteLock(void);

/**
 * release lock(read or write) on registry
 *
 */
gboolean erRegUnlock(void);

/**
 * inquire current lock state on registry
 *
 */
lock_state_t erRegGetLockState(void);


#ifdef __cplusplus
}
#endif

#endif                          //_ERREG_RWLOCK_H_
