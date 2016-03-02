/* Copyright (C) 2015-2016  Thomas Pircher <tehpeh@gmx.net>
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
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "config.h"
#include "fileparser.h"


extern int handle_cgi(void);


char *normalize_path(const char *path)
{
    size_t norm_path_len = strlen(path) + sizeof("main.md") + 1;
    char *norm_path = malloc(norm_path_len);
    if (*path == '/')
    {
        path++;
    }
    char *dst = norm_path;
    for (const char *src = path; *src; src++)
    {
        if (*src == '?')
        {
            break;
        }
        else if (isalnum(*src) || (*src == '/'))
        {
            *dst++ = *src;
        }
        else
        {
            *dst++ = '_';
        }
    }
    *dst = '\0';

//    if (norm_path[0] == '\0')
//    {
//        snprintf(norm_path, norm_path_len, "main");
//    }
//    else if (dst[-1] == '/')
//    {
//        snprintf(dst, norm_path_len, "main");
//    }

    return norm_path;
}


void js_escape(char *dst, const char *src)
{
    while (*src)
    {
        if (*src == '\n')
        {
            *dst++ = '\\';
            *dst++ = 'n';
        }
        else if (*src == '\'')
        {
            *dst++ = '\\';
            *dst++ = *src;
        }
        else
        {
            *dst++ = *src;
        }
        src++;
    }
    *dst++ = '\0';
}


int cgiMain(void)
{
    config_t cfg;
    if (!config_init(&cfg, CONFIG_FILE_PATH))
    {
        cgiHeaderStatus(500, "Failed to farse config file");
        return 1;
    }

    char *path_info = normalize_path(cgiPathInfo);

    char local_path[1024];
    size_t local_path_len = snprintf(local_path, sizeof(local_path), "%s/%s", cfg.data_dir, path_info);
    if (local_path_len >= sizeof(local_path))
    {
        cgiHeaderStatus(500, "Failed to format destination path");
        return 0;
    }
    size_t sep_pos = 0;
    for (size_t i = 0; i < local_path_len; i++)
    {
        if (local_path[i] == '/')
        {
            sep_pos = i;
        }
    }
    if (sep_pos + 1 == local_path_len)
    {
        snprintf(local_path + sep_pos + 1, sizeof(local_path) - sep_pos, "%s", "main.md");
    }
    else
    {
        snprintf(local_path + local_path_len, sizeof(local_path) - sep_pos, "%s", ".md");
    }


    if (cgiFormSubmitClicked("savepage") == cgiFormSuccess)
    {
        int buf_len, ret;
        if (cgiFormStringSpaceNeeded("wikitext", &buf_len) != cgiFormSuccess)
        {
            buf_len = 1;
        }
        char *buf = malloc(buf_len);
        if (cgiFormString("wikitext", buf, buf_len) != cgiFormSuccess)
        {
            buf[0] = '\0';
        }
        buf_len = strlen(buf);

        local_path[sep_pos] = '\0';
        struct stat st;
        if (stat(local_path, &st) < 0)
        {
            ret = mkdir(local_path, 0755);
            if (ret != 0)
            {
                cgiHeaderStatus(500, "Failed to create wiki path");
                return 0;
            }
        }
        local_path[sep_pos] = '/';
        int fd = open(local_path, O_WRONLY | O_CREAT | O_NOCTTY | O_TRUNC, 0666);
        if (fd < 0)
        {
            cgiHeaderStatus(500, "Failed to create wiki file");
            return 0;
        }
        ret = write(fd, buf, buf_len);
        if (ret != buf_len)
        {
            cgiHeaderStatus(500, "Failed to write wiki file");
            return 0;
        }
        close(fd);
        free(buf);
    }

    char *buf = NULL;
    size_t buf_len = 0;
    struct stat st;
    if (stat(local_path, &st) < 0)
    {
        buf_len = 1;
        buf = malloc(buf_len);
        *buf = '\0';
    }
    else
    {
        buf_len = st.st_size;
        buf = malloc(buf_len);

        int fd = open(local_path, O_RDONLY);
        if (fd >= 0)
        {
            memset(buf, 0, buf_len);
            read(fd, buf, st.st_size);
        }
    }

    cgiHeaderContentType("text/html");
    fprintf(cgiOut, "<!doctype html>\n");
    fprintf(cgiOut, "<html><head>\n");
    fprintf(cgiOut, "<meta charset=\"utf-8\"/>");
    fprintf(cgiOut, "<title>"),
        cgiHtmlEscape(path_info),
        fprintf(cgiOut, "</title>\n");
    fprintf(cgiOut, "<link href=\"/wiki-files/main.css\" rel=\"stylesheet\" type=\"text/css\">\n");
    fprintf(cgiOut, "<script src=\"/wiki-files/marked.min.js\"></script>\n");
    fprintf(cgiOut, "</head>\n");
    fprintf(cgiOut, "<body>\n");

    int edit;
    cgiFormInteger("edit", &edit, 0);
    if (!edit)
    {
        char *js_buf = malloc(2 * buf_len + 1);
        js_escape(js_buf, buf);
        fprintf(cgiOut, "<header><a href=\"?edit=1\">edit</a></header>\n");

        fprintf(cgiOut, "<div id=\"content\"></div>\n");
        fprintf(cgiOut, "<script>\n");
        fprintf(cgiOut, "document.getElementById('content').innerHTML = marked('%s');\n", js_buf);
        fprintf(cgiOut, "</script>\n");

        free(js_buf);
    }
    else
    {
        fprintf(cgiOut, "<form method=\"post\" enctype=\"multipart/form-data\" ");
        fprintf(cgiOut, "action=\""),
            cgiValueEscape(cgiScriptName),
            fprintf(cgiOut, "/"),
            cgiValueEscape(path_info),
            fprintf(cgiOut, "\">\n");
        fprintf(cgiOut, "<textarea name=\"wikitext\" autofocus=\"autofocus\" rows=\"20\" style=\"width: 90%%\">\n"),
            cgiValueEscape(buf),
            fprintf(cgiOut, "</textarea>\n");
        fprintf(cgiOut, "<br/>");
        fprintf(cgiOut, "<input type=\"submit\" name=\"savepage\" value=\"Save Page\">\n");
        fprintf(cgiOut, "<input type=\"button\" value=\"Back\" onclick=\"location.href='"),
            cgiHtmlEscape(cgiScriptName),
            cgiHtmlEscape(path_info),
            fprintf(cgiOut, "';\">\n");
        fprintf(cgiOut, "</form>\n");
    }

    fprintf(cgiOut, "</body></html>\n");

    free(path_info);
    free(buf);
    return 0;
}
