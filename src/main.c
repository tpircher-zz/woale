/*
 * Copyright (C) 2015-2016  Thomas Pircher <tehpeh@gmx.net>
 *
 * woale -- a wiki engine.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file
 * Main file for the woale application.
 */

#include <cgic.h>
#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include <stdbool.h>
#include "config.h"

typedef struct cfg {
    const char *cfg_file;
    const char *server_name;
} cfg_t;


int handle_cgi(void);
bool load_config(cfg_t *cfg);


bool load_config(cfg_t *cfg)
{
    cfg->cfg_file = INSTALL_SYSCONFIG_DIR "/woale.conf";
    cfg->server_name = "test.example.com";
    return true;
}

int main(void)
{
    cfg_t cfg;
    load_config(&cfg);
    return handle_cgi();
}


int handle_cgi(void)
{
    return 0;
}
