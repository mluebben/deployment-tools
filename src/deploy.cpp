// SPDX-License-Identifier: GPL-3.0-or-later
//
// Copyright 2022 Matthias Lübben <ml81@gmx.de>
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Deployment shell script.
char const* program = "/etc/deploy/deploy.sh";

// Entry point
int main(int argc, char* argv[], char* envp[])
{
	// Copy over program arguments for execve
	char** new_argv = new char*[argc + 1];
        new_argv[0] = const_cast<char*>(program);
        for (int i = 1; i < argc; i++) {
                new_argv[i] = argv[i];
        }
        new_argv[argc] = nullptr;

	// Gain full root priviliges
	if (setuid(0) == -1) {
		perror("setuid failed");
		return 1;
	}
	if (seteuid(0) == -1) {
		perror("seteuid failed");
		return 1;
	}
	if (setgid(0) == -1) {
		perror("setgid failed");
		return 1;
	}
	if (setegid(0) == -1) {
		perror("setegid failed");
		return 1;
	}

	// Replace this process with the deployment shell script.
        if (execve(program, new_argv, envp) == -1) {
		perror("execve failed");
		return 1;
	}

	// Should be never reached. Program failed if it does.
	return 1;
}

