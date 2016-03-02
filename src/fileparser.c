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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "config.h"
#include "fileparser.h"


static char buf[4096];

typedef enum {
    FS_NONE,
    FS_COMMENT,
    FS_ID,
    FS_SEARCH_EQ,
    FS_SEARCH_VALUE,
    FS_VALUE,
    FS_STR_VALUE,
} file_state_t;


static inline bool isid(const char c)
{
    return isalpha(c) || c == '_';
}


static const char **find_id(config_t *cfg, const char *buf, ssize_t buf_size)
{
    switch (buf_size)
    {
        case 8:
            if (strncmp(buf, "data_dir", buf_size) == 0)
            {
                return &cfg->data_dir;
            }
            break;

        default:
            break;
    }
    return NULL;
}


static bool parse(config_t *cfg, char *buf, ssize_t buf_size)
{
    if (cfg == NULL || buf == NULL || buf_size < 0)
    {
        return false;
    }

    ssize_t buf_pos = 0;
    ssize_t buf_marker = 0;
    const char **id;
    file_state_t state = FS_NONE;

    while (buf_pos < buf_size)
    {
        switch (state)
        {
            case FS_NONE:
                if (isspace(buf[buf_pos]))
                {
                    // ignore
                }
                else if (buf[buf_pos] == '#')
                {
                    state = FS_COMMENT;
                }
                else if (isid(buf[buf_pos]))
                {
                    state = FS_ID;
                    buf_marker = buf_pos;
                }
                else
                {
                    return false;
                }
                break;
            case FS_COMMENT:
                if (buf[buf_pos] == '\n')
                {
                    state = FS_NONE;
                }
                break;
            case FS_ID:
                if (isid(buf[buf_pos]))
                {
                    // OK
                }
                else if (isspace(buf[buf_pos]))
                {
                    if ((id = find_id(cfg, &buf[buf_marker], buf_pos - buf_marker)) == NULL)
                    {
                        return false;
                    }
                    state = FS_SEARCH_EQ;
                }
                else if (buf[buf_pos] == '=')
                {
                    if ((id = find_id(cfg, &buf[buf_marker], buf_pos - buf_marker)) == NULL)
                    {
                        return false;
                    }
                    state = FS_SEARCH_VALUE;
                }
                else
                {
                    return false;
                }
                break;
            case FS_SEARCH_EQ:
                if (isspace(buf[buf_pos]))
                {
                    // OK
                }
                else if (buf[buf_pos] == '=')
                {
                    state = FS_SEARCH_VALUE;
                }
                else
                {
                    return false;
                }
                break;
            case FS_SEARCH_VALUE:
                if (isspace(buf[buf_pos]))
                {
                    // OK
                }
                else if (buf[buf_pos] == '"')
                {
                    state = FS_STR_VALUE;
                    buf_marker = buf_pos;
                    *id = &buf[buf_marker + 1];
                }
                else if (isid(buf[buf_pos]))
                {
                    state = FS_VALUE;
                    buf_marker = buf_pos;
                    *id = &buf[buf_marker];
                }
                else
                {
                    return false;
                }
                break;
            case FS_VALUE:
                if (isid(buf[buf_pos]))
                {
                    // OK
                }
                else if (isspace(buf[buf_pos]))
                {
                    buf[buf_pos] = '\0';
                    state = FS_NONE;
                }
                else if (buf[buf_pos] == '#')
                {
                    buf[buf_pos] = '\0';
                    state = FS_COMMENT;
                }
                else
                {
                    return false;
                }
                break;
            case FS_STR_VALUE:
                if (buf[buf_pos] == '"')
                {
                    buf[buf_pos] = '\0';
                    state = FS_NONE;
                }
                else if (buf[buf_pos] == '\n' || buf[buf_pos] == '\\')
                {
                    return false;
                }
                else
                {
                    // OK
                }
                break;
            default:
                return false;

        }
        buf_pos++;
    }
    return state == FS_NONE;
}


bool config_init(config_t *cfg, const char *file)
{
    bool ret = true;

    cfg->debug = false;
    cfg->data_dir = INSTALL_LOCALSTATE_DIR "/lib/woale";

    if (file)
    {
        int fd = open(file, O_RDONLY);

        if (fd >= 0)
        {
            ssize_t read_size = read(fd, buf, sizeof(buf));
            close(fd);

            ret = parse(cfg, buf, read_size);
        }
        else
        {
            ret = false;
        }
    }

    return ret;
}
