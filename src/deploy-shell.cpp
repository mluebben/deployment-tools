// SPDX-License-Identifier: GPL-3.0-or-later
//
// Copyright 2022 Matthias Lübben <ml81@gmx.de>
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

char const* program = "/usr/local/bin/deploy";

void usage() 
{
	fprintf(stderr, "usage: deploy-shell -c \"deploy <target>\"\n");
}

int main(int argc, char* argv[], char* envp[])
{
	// Parse arguments
	if (argc != 3) {
		usage();
		return 1;
	}

	if (strcmp(argv[1], "-c") != 0) {
		usage();
		return 1;
	}

	char* tmp;
	tmp = strtok(argv[2], " ");
	if (tmp == nullptr || strcmp(tmp, "deploy") != 0) {
		usage();
		return 1;
	}

	tmp = strtok(nullptr, " ");
	if (tmp == nullptr || strlen(tmp) > 250) {
		usage();
		return 1;
	}

	char configuration[251];
	memset(configuration, '0', sizeof(configuration));
	strcpy(configuration, tmp);

	tmp = strtok(nullptr, " ");
	if (tmp != nullptr) {
		usage();
		return 1;
	}

	// Prepare arguments for deploy program
	char** new_argv = new char*[6];
	new_argv[0] = const_cast<char*>(program);
	new_argv[1] = const_cast<char*>("-c");
	new_argv[2] = configuration;
	new_argv[3] = const_cast<char*>("-i");
	new_argv[4] = const_cast<char*>("-");
	new_argv[5] = nullptr;


	// Replace process with deploy program
	if (execve(program, new_argv, envp) == -1) {
		perror("execve failed");
		return 1;
	}

	// Should be never reached
	return 1;
}

