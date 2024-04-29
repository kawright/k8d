// Copyright (C) 2024  KA Wright

// argv.c - Argument vector parsing

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argv.h"
#include "err.h"

void init_argv(Argv *this) {
    this->about     = ARGV_DFLT_ABOUT;
    this->fname     = ARGV_DFLT_FNAME;
    this->help      = ARGV_DFLT_HELP;
    this->out       = ARGV_DFLT_OUT;
}

void parse_argv(Argv *this, uint16_t argc, char *argv[], Err *err) {
    int curr_opt;
    while ((curr_opt = getopt(argc, argv, ARGV_OPTSTR)) != -1) {
        switch (curr_opt) {
            
            case 'a':
            this->about = true;
            break;

            case 'h':
            this->help = true;
            break;

            case 'o':
            this->out = optarg;
            break;
            
            case '?':
            err->code = ERR_ARGV;
            if (optopt == 'o') {
                strcpy(err->msg, "Option -o requires argument");
                return;
            }
            sprintf(err->msg, "Unknown option: -%c", optopt);
            return;

            default:
            return;
        }
    }
    if (optind != argc - 1 && !(this->about || this->help)) {
        err->code = ERR_ARGV;
        sprintf(err->msg, "1 positional argument expected, but %d given", 
            optind - argc);
        return;
    }
    this->fname = argv[optind];
}
