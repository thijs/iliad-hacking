/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file system.c
 * \brief connectionMgr - system utilities
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <glib.h>

#include "connectionMgrLog.h"

static pid_t  g_forkexec_child_pid = 0;

pid_t get_forkexec_child_pid()
{
    return g_forkexec_child_pid;
}


// Execute specified command with arguments in child process
pid_t fork_exec_no_wait(const int argc, char *const argv[])
{
    pid_t pid = 0;
    int   rc;

    CN_LOGPRINTF("entry: [%s]", argv[0]);
    g_assert(argv[argc] == NULL);
    
    // spawn child process
    switch (pid = fork())
    {
        case 0:
            // child process: execute command
            rc = execvp(argv[0], argv);
            CN_ERRORPRINTF("execvp [%s] returns [%d] errno [%d] - %s", 
                           argv[0], rc, errno, strerror(errno));
            g_assert_not_reached();
            exit(1);

        case -1:
            // error: fork failed
            CN_ERRORPRINTF("fork returns [%d] errno [%d] - %s", 
                           pid, errno, strerror(errno));
            g_assert_not_reached();
            exit(1);

        default:
            // parent process: report child's pid
            CN_WARNPRINTF("my pid [%d] child pid [%d]", getpid(), pid);
            return pid;
    }
}


// Execute specified command with arguments in child process
// and wait for child process to end
int fork_exec(const int argc, char *const argv[])
{
    int   ret = -1;
    pid_t pid;
    int   status;

    CN_LOGPRINTF("entry: [%s]", argv[0]);
    g_assert(g_forkexec_child_pid == 0);

    // start child process
    pid = fork_exec_no_wait(argc, argv);
    if (pid > 0)
    {
        // child process started, wait for child and return its exit value
        g_forkexec_child_pid = pid;
        waitpid(pid, &status, 0);
        CN_WARNPRINTF("WIFEXITED [%d]", WIFEXITED(status));
        if (WIFEXITED(status))
        {
            CN_WARNPRINTF("WEXITSTATUS [%d]", WEXITSTATUS(status));
            ret = WEXITSTATUS(status);
        }
        g_forkexec_child_pid = 0;
    }

    return ret;
}

