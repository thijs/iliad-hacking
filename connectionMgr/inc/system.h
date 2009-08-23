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
 * \file system.h
 * \brief connectionMgr - eReader system calls
 * 
 * This file provides system call functionality to execute commands and such
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _SYSTEM_T_H_
#define _SYSTEM_T_H_

#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    // Execute specified command with arguments 
    // in child process and report its pid
    pid_t fork_exec_no_wait(const int argc, char *const argv[]);

    // Execute specified command with arguments 
    // in child process and report its exit status
    int fork_exec (const int argc, char *const argv[]);

    // Report child pid from fork_exec
    pid_t get_forkexec_child_pid(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_SYSTEM_T_H_

