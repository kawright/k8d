// Copyright (C) 2024  KA Wright

// argv.h - Argument vector parsing.

#ifndef __ARGV_H__
#define __ARGV_H__

#include <stdbool.h>
#include <stdint.h>

#include "err.h"

#define ARGV_DFLT_ABOUT     false
#define ARGV_DFLT_FNAME     NULL
#define ARGV_DFLT_HELP      false
#define ARGV_DFLT_OUT       NULL
#define ARGV_OPTSTR         "aho:"

// Stores parsed command-line args as fields.
typedef struct __ARGV__ {
    bool    about;
    char    *fname;
    bool    help;
    char    *out;
} Argv;

// Initialize an Argv.
void init_argv(Argv *this);

// Parse an argument vector. Throws ERR_ARGV.
void parse_argv(Argv *this, uint16_t argc, char *argv[], Err *err);

#endif
