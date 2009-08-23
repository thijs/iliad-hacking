/*
 * This file is part of liberregxml.
 *
 * liberregxml is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberregxml is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file 
 * \brief liberreg -  read and write lock routines
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */

/******************************************************************************
how to use:
(1)erRegRWLockInit();

(2)
erRegWriteLock();
write something to registry
erRegUnlock();

(3)do_something_else;

(4)
erRegReadLock();
get something from registry
erRegUnlock();

(5)erRegRWLockDestroy();
******************************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <glib.h>

#include "erregrwlock.h"
#include "erreglog.h"


#define LOCK_FILE "/lockfile"

// local data
static int             g_lock_fd = -1;     // uses it to deal with the access among multi-process
static pthread_mutex_t g_mutex;            // uses it to deal with the access among multi-threads
static lock_state_t    g_lock_state = lock_none;


static int lock_reg(int cmd, int type)
{
    struct flock lock;
    lock.l_type   = type;
    lock.l_start  = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len    = 0;

    return fcntl(g_lock_fd, cmd, &lock);
}

#define read_lock()       lock_reg(F_SETLK,  F_RDLCK)
#define readw_lock()      lock_reg(F_SETLKW, F_RDLCK)
#define write_lock()      lock_reg(F_SETLK,  F_WRLCK)
#define writew_lock()     lock_reg(F_SETLKW, F_WRLCK)
#define un_lock()         lock_reg(F_SETLK,  F_UNLCK)


// Initialise lock mechanism
gboolean erRegRWLockInit(void)
{
    ERREG_RWLOCK_LOGPRINTF("entry");
    g_assert(g_lock_fd < 0);

    gboolean bRet = FALSE;  // FALSE = error
    int rc;

    // initialise mutex for inter-thread locking
    rc = pthread_mutex_init(&(g_mutex), NULL);
    if (rc != 0)
    {
        // mutex error
        ERREG_RWLOCK_ERRORPRINTF("Could not initialize mutex, error [%d] [%s]", rc, strerror(rc));
        g_assert_not_reached();
    }
    else
    {
        // open file for inter-process locking
        g_lock_fd = open(DATADIR LOCK_FILE, O_RDWR | O_CREAT);
        if (g_lock_fd < 0)
        {
            // file error
            ERREG_RWLOCK_ERRORPRINTF("Could not open file [%s], error [%d] [%s]", DATADIR LOCK_FILE, errno, strerror(errno));
            g_assert_not_reached();
        }
        else
        {
            // filed opened ok
            bRet = TRUE;
        }
    }

    g_lock_state = lock_none;
    return bRet;
}

// Destroy lock mechanism
void erRegRWLockDestroy(void)
{
    ERREG_RWLOCK_LOGPRINTF("entry");
    g_assert(g_lock_fd >= 0);

    int rc;

    // close file for inter-process locking, this implicitly releases the lock
    rc = close(g_lock_fd);
    if (rc != 0)
    {
        // file error
        ERREG_RWLOCK_ERRORPRINTF("Could not close file, error [%d] [%s]", errno, strerror(errno));
        g_assert_not_reached();
    }
    g_lock_fd = -1;

    // destroy mutex for inter-thread locking
    rc = pthread_mutex_destroy(&(g_mutex));
    if (rc != 0)
    {
        // mutex error
        ERREG_RWLOCK_ERRORPRINTF("Could not destroy mutex, error [%d] [%s]", rc, strerror(rc));
        g_assert_not_reached();
    }

    g_lock_state = lock_none;
}

// Obtain read lock
gboolean erRegReadLock(void)
{
    ERREG_RWLOCK_LOGPRINTF("entry");
    g_assert(g_lock_fd >= 0);

    gboolean bRet = FALSE;  // FALSE = error
    int rc;

    // get mutex to block other threads
    rc = pthread_mutex_lock(&g_mutex);
    if (rc != 0)
    {
        // mutex error
        ERREG_RWLOCK_ERRORPRINTF("Could not lock mutex, error [%d] [%s]", rc, strerror(rc));
        g_assert_not_reached();
    }
    else
    {
        // get file lock
        rc = readw_lock();
        if (rc == -1)
        {
            // file lock error
            ERREG_RWLOCK_ERRORPRINTF("Could not lock file, error [%d] [%s]", errno, strerror(errno));
            g_assert_not_reached();
        }
        else
        {
            // file lock obtained
            ERREG_RWLOCK_LOGPRINTF("Obtained RLock.");
            g_lock_state = lock_read;
            bRet = TRUE;  // TRUE = ok
        }
    }

    return bRet;
}

// Obtain write lock
gboolean erRegWriteLock(void)
{
    ERREG_RWLOCK_LOGPRINTF("entry");
    g_assert(g_lock_fd >= 0);

    gboolean bRet = FALSE;  // FALSE = error
    int rc;

    // get mutex to block other threads
    rc = pthread_mutex_lock(&g_mutex);
    if (rc != 0)
    {
        // mutex error
        ERREG_RWLOCK_ERRORPRINTF("Could not lock mutex, error [%d] [%s]", rc, strerror(rc));
        g_assert_not_reached();
    }
    else
    {
        // get file lock
        rc = writew_lock();
        if (rc == -1)
        {
            // file lock error
            ERREG_RWLOCK_ERRORPRINTF("Could not lock file, error [%d] [%s]", errno, strerror(errno));
            g_assert_not_reached();
        }
        else
        {
            // file lock obtained
            ERREG_RWLOCK_LOGPRINTF("Obtained WLock");
            g_lock_state = lock_write;
            bRet = TRUE;  // TRUE = ok
        }
    }

    return bRet;
}

// Release lock
gboolean erRegUnlock(void)
{
    ERREG_RWLOCK_LOGPRINTF("entry");
    g_assert(g_lock_fd >= 0);

    gboolean bRet = FALSE;  // FALSE = error
    int rc;

    // release file lock
    rc = un_lock();
    if (rc == -1)
    {
        // file unlock error
        ERREG_RWLOCK_ERRORPRINTF("Could not release file lock, error [%d] [%s]", errno, strerror(errno));
        g_assert_not_reached();
    }
    else
    {
        // release mutex
        rc = pthread_mutex_unlock(&g_mutex);
        if (rc != 0)
        {
            // mutex error
            ERREG_RWLOCK_ERRORPRINTF("Could not release mutex, error [%d] [%s]", rc, strerror(rc));
            g_assert_not_reached();
        }
        else
        {
            // locks released ok
            ERREG_RWLOCK_LOGPRINTF("Released Lock");
            g_lock_state = lock_none;
            bRet = TRUE;  // TRUE = ok
        }
    }

    return bRet;
}

// Report current lock state
lock_state_t erRegGetLockState(void)
{
    return g_lock_state;
}
