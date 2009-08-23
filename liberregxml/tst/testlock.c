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
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

/**
 * \file 
 * \brief  - test liberregxml
 *                the functions of rwlock
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "erregapi.h"

#include "erregxsd.h"
#include "erregxml.h"
#include "erregtypes_f.h"
#include "erreglog.h"

static void testThreads(void);
//static void testProcess(void);

int main()
{
    gboolean bRet;

    // initialize rwlock
    bRet = erRegRWLockInit();
    if (FALSE == bRet)
    {
        ERREG_ERRORPRINTF("error");
        return 0;
    }

    testThreads();
    
    // destroy rwlock
    erRegRWLockDestroy();

    return 0;
}

#include <pthread.h>
#define MAX_ARGC    20
static pthread_t tid[MAX_ARGC];
static const char *Okay = "OK";
static const char *Failed = "Failed";

static void *threadFunc(void *private_data)
{
    int okay = 1;
    gboolean bRet;

    // locking write lock
//    bRet = erRegWriteLock();
    bRet = erRegReadLock();
    if (FALSE == bRet)
    {
        ERREG_ERRORPRINTF("error");
        return FALSE;
    }

    while (1)
    {
        ERREG_ERRORPRINTF("HERE");
        usleep(5*1000);
    }

    // unlocking write lock
    erRegUnlock();

    if (okay == 0)
    return((void *) Failed);
    return ((void *) Okay);
}

static void testThreads(void)
{
    unsigned int i;
    unsigned int num_threads = 4;
    void *results[MAX_ARGC];
    int ret;

    for (i = 0; i < num_threads; i++) {
        results[i] = NULL;
        tid[i] = (pthread_t) -1;
    }

    for (i = 0; i < num_threads; i++) {
        ret = pthread_create(&tid[i], NULL, threadFunc,
                 (void *) NULL);
        if (ret != 0) {
        ERREG_ERRORPRINTF("pthread_create");
        perror("pthread_create");
        exit(1);
        }
    }
    for (i = 0; i < num_threads; i++) {
        ret = pthread_join(tid[i], &results[i]);
        if (ret != 0) {
        ERREG_ERRORPRINTF("pthread_join");
        perror("pthread_join");
        exit(1);
        }
    }

    for (i = 0; i < num_threads; i++)
        if (results[i] != (void *) Okay)
        ERREG_ERRORPRINTF("Thread %d handling failed", i);
}
/*
static void testProcess(void)
{
}
*/
