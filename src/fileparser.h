/* Copyright (C) 2016  Thomas Pircher <tehpeh@gmx.net>
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
 * Configuration file parser.
 */

#ifndef FILEPARSER_H
#define FILEPARSER_H
#include <stdbool.h>
#include "config.h"


/**
 * A structure containing the configuration options.
 */
typedef struct {
    bool debug;             //!< Whether debug is enabled.
    const char *data_dir;   //!< Path to the data path.
} config_t;


/**
 * Initialise the configuration.
 */
bool config_init(config_t *cfg, const char *file);

#endif
