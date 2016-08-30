/* Copyright (C) 2015-2016  Thomas Pircher <tehpeh-web@tty1.net>
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
#include "woale_confparser.h"
#include "dbconn.h"
#include "config.h"

#include <cgicc/Cgicc.h>
#include <cgicc/HTTPStatusHeader.h>
#include <cgicc/HTTPRedirectHeader.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

using namespace std::string_literals;


/**
 * Escape a string to be embedded as Javascript code.
 * \param[in] src the string to escape.
 * \return the resulting string.
 */
static const std::string escape(const std::string src)
{
    auto dst = ""s;

    for (auto& c : src) {
        if (c == '\n') {
            dst += "\\n";
        } else if (c == '\r') {
            // ignore.
        } else if (c == '\'') {
            dst += "\\'";
        } else if (c == '\\') {
            dst += "\\\\";
        } else {
            dst += c;
        }
    }
    return dst;
}


/**
 * Un-escape a string to be embedded as Javascript code.
 * \param[in] src the string to un-escape.
 * \return the resulting string.
 */
static const std::string unescape(const std::string src)
{
    auto escaping = false;
    auto dst = ""s;

    for (auto& c : src) {
        if (escaping) {
            if (c == 'n') {
                dst += '\n';
            } else {
                dst += c;
            }
            escaping = false;
        } else if (c == '\\') {
            escaping = true;
        } else {
            dst += c;
        }
    }
    return dst;
}


/**
 * Normalise a path.
 * \param[in] src the input path.
 * \return the resulting path.
 */
static const std::string normalize_path(const std::string src)
{
    auto dst = ""s;

    auto slash_seen = true;
    for (auto& c : src) {
        if (c == '/') {
            slash_seen = true;
        } else if (slash_seen) {
            if (c != '/' && c != '.') {
                dst += '/';
                slash_seen = false;
            }
        }
        if (!slash_seen) {
            dst += c;
        }
    }

    if (dst == "") {
        dst = "/";
    }

    return dst;
}


/**
 * Create a diff of two texts.
 * \param[in] mine the user's version of the page.
 * \param[in] theirs the newest version of the page in the database.
 * \return a diffed version of the two pages.
 *
 * This is the simplest possbile implementation of the diff, by simply
 * returning both versions as entirely conflicting. This should be refined to
 * generate a better diff.
 */
static const std::string get_diff(const std::string mine, const std::string theirs)
{
    return "<<<<<<< Mine\n"s + mine + "\n=======\n"s + theirs + "\n>>>>>>> Theirs\n"s;
}


/**
 * Main program entry.
 * \param[in] argc the number of arguments.
 * \param[in] argv a vector of pointers to string.
 * \retval 0 on success.
 * \retval nonzero on failure.
 */
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    try {
        woale::WoaleFileParser fp;
        fp.parse(CONFIG_FILE_PATH);

        woale::DbConn db(fp.data_dir() + "/pages.sqlite3");

        cgicc::Cgicc cgi;
        auto env = cgi.getEnvironment();

        const auto page_path = normalize_path(env.getPathInfo());

        auto conflict = false;
        auto unsaved_page = ""s;
        auto submit = cgi.getElement("savepage");
        if (submit != cgi.getElements().end()) {
            auto elem = cgi.getElement("page_ver");
            if (elem != cgi.getElements().end() && !elem->isEmpty()) {
                auto page_ver = std::stoi(elem->getValue());
                auto page = cgi("wikitext");
                if (!db.save_page(page_path, escape(page), page_ver)) {
                    conflict = true;
                    unsaved_page = page;
                }
            }
        }

        unsigned int page_ver;
        std::string page;
        std::tie(page, page_ver) = db.get_page(page_path);

        // Send HTTP header
        std::cout << cgicc::HTTPHTMLHeader();

        // Set up the HTML document
        std::cout << cgicc::html().set("lang", "en").set("dir", "ltr") <<
            cgicc::head() <<
            cgicc::title(std::string("woale &ndash; ") + page_path) <<
            cgicc::meta().set("charset", "utf-8") <<
            cgicc::link().set("href", fp.http_data_path() + "/main.css").set("rel", "stylesheet").set("type", "text/css") <<
            cgicc::script().set("src", fp.js_render_lib()) << cgicc::script() <<
            cgicc::head() << "\n";
        std::cout << cgicc::body();

        auto edit = cgi.getElement("edit");
        auto do_edit = edit != cgi.getElements().end() && !edit->isEmpty();
        if (conflict) {
            std::cout << cgicc::h1() << "Resolve conflicts below" << cgicc::h1() << "\n";
            page = get_diff(unsaved_page, unescape(page));
            do_edit = true;
        }
        if (do_edit)
        {
            std::string path_info = env.getScriptName() + page_path;
            std::cout <<
                cgicc::form().set("method", "post").set("enctype", "multipart/form-data").set("action", path_info) <<
                cgicc::textarea().set("name", "wikitext").set("autofocus", "autofocus").set("rows", "20").set("style", "width: 90%") <<
                unescape(page) <<
                cgicc::textarea() << cgicc::br() <<
                cgicc::input().set("type", "submit").set("name", "savepage").set("value", "Save Page") <<
                " &nbsp; &nbsp; " <<
                cgicc::input().set("type", "button").set("name", "back").set("value", "Back").set("onclick", "history.go(-1); return true;") <<
                cgicc::input().set("type", "hidden").set("name", "page_ver").set("value", std::to_string(page_ver + 1)) <<
                cgicc::form() << "\n";
        } else {
            std::cout <<
                cgicc::div().set("id", "header") <<
                cgicc::a().set("href", "?edit=1") << "edit" << cgicc::a() <<
                cgicc::div() <<
                cgicc::div().set("id", "content") << cgicc::div() <<
                cgicc::script() <<
                "page_content = '" << page << "';" <<
                "document.getElementById('content').innerHTML = marked(page_content);" <<
                cgicc::script() << "\n";
        }

        // Close the HTML document
        std::cout << cgicc::body() << cgicc::html() << std::endl;
    } catch (std::exception& e) {
        // std::cerr << "error: " << e.what() << std::endl;
        std::cout << cgicc::HTTPStatusHeader(500, e.what());
        return 1;
    }
    return 0;
}
