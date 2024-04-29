// Copyright (C) 2024  KA Wright

// main.c - Entry point.

#include <stdbool.h>
#include <stdio.h>

#include "argv.h"
#include "asset.h"
#include "dasm.h"
#include "err.h"

// Check for error conditions; alert and exit if applicable.
#define CHK_ERR();              \
    if (is_err(&err)) {         \
        err_alert(&err);        \
        return err.code;        \
    }

int main(int argc, char *argv[]) {
    Err err;
    init_err(&err);

    Argv argv_obj;
    init_argv(&argv_obj);

    Dasm dasm;
    init_dasm(&dasm); 

    char curr_ln[MAX_DASM_LN_LEN];

    char about[MAX_ASSET_ABOUT_SZ + 1];
    char help[MAX_ASSET_HELP_SZ + 1];

    FILE *out = NULL;

    parse_argv(&argv_obj, argc, argv, &err);
    CHK_ERR();

    if (argv_obj.out != NULL) {
        out = fopen(argv_obj.out, "w");
        if (out == NULL) {
            err.code = ERR_IO;
            snprintf(err.msg, MAX_ERR_MSG_LEN, "Could not open file %s",
                argv_obj.out);
            err_alert(&err);
            return err.code;
        }
    } else {
        out = stdout;
    }

    if (argv_obj.about) {
        load_asset_about(about);
        printf("\n%s\n", about);
        return 0;
    }
    if (argv_obj.help) {
        load_asset_help(help);
        printf("\n%s\n", help);
        return 0;
    }

    load_dasm(&dasm, argv_obj.fname, &err);
    CHK_ERR();

    fprintf(out, "%s\n", "ADDR     INSTR     NAME    ARGS");
    fprintf(out, "%s\n", "====     =====     ====    ====");

    // MAIN LOOP
    while (!dasm_eof(&dasm)) {
        dasm_instr(&dasm, curr_ln);
        fputs(curr_ln, out); 
    }

    return 0;
}
