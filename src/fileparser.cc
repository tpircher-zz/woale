/* Copyright (C) 2016  Thomas Pircher <tehpeh-web@tty1.net>
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

#include "fileparser.h"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cctype>

namespace woale {

    /**
     * Check if a character is a valid start of an identifier.
     */
    static inline bool isid_start(const char c)
    {
        return std::isalpha(c);
    }


    /**
     * Check if a character is a valid identifier.
     */
    static inline bool isid(const char c)
    {
        return std::isalnum(c) || c == '_';
    }


    /**
     * Check if a character is a valid value.
     */
    static inline bool isval(const char c)
    {
        return std::isalnum(c) || std::ispunct(c);
    }


    void FileParser::parse(const char *file_name)
    {
        std::ifstream fs;
        fs.open(file_name);
        if (!fs.is_open()) {
            std::stringstream err;
            err << "failed to open config file '" << file_name << "'";
            throw std::runtime_error(err.str());
        }

        State state = NONE;
        std::string id(""), val("");

        unsigned int lineno = 1;
        while (!fs.eof()) {
            char c;

            fs.get(c);

            if (c == '\n') {
                lineno++;
            }
            switch (state) {
                case NONE:
                    if (std::isspace(c)) {
                        // ignore
                    } else if (c == '#') {
                        state = COMMENT;
                    } else if (isid_start(c)) {
                        state = ID;
                        id = c;
                    } else {
                        std::stringstream err;
                        err << "line " << lineno << ": unhandled character '" << c << "'";
                        throw std::runtime_error(err.str());
                    }
                    break;

                case COMMENT:
                    if (c == '\n') {
                        state = NONE;
                    }
                    break;

                case ID:
                    if (isid(c)) {
                        id += c;
                    } else if (std::isspace(c)) {
                        state = SEARCH_EQ;
                    } else if (c == '=') {
                        state = SEARCH_VALUE;
                    } else {
                        std::stringstream err;
                        err << "line " << lineno << ": unhandled character '" << c << "'";
                        throw std::runtime_error(err.str());
                    }
                    break;

                case SEARCH_EQ:
                    if (std::isspace(c)) {
                        // OK
                    } else if (c == '=') {
                        state = SEARCH_VALUE;
                    } else {
                        std::stringstream err;
                        err << "line " << lineno << ": unhandled character '" << c << "'";
                        throw std::runtime_error(err.str());
                    }
                    break;

                case SEARCH_VALUE:
                    if (std::isspace(c)) {
                        // OK
                    } else if (c == '"') {
                        state = STR_VALUE;
                        val = "";
                    } else if (isval(c)) {
                        state = VALUE;
                        val = c;
                    } else {
                        std::stringstream err;
                        err << "line " << lineno << ": unhandled character '" << c << "'";
                        throw std::runtime_error(err.str());
                    }
                    break;

                case VALUE:
                    if (c == '#') {
                        std::stringstream err;
                        err << "line " << lineno << ": comment not allowed in value";
                        throw std::runtime_error(err.str());
                    } else if (std::isspace(c)) {
                        state = SEARCH_EOL;
                        add_key_val(id, val);
                    } else if (isval(c)) {
                        val += c;
                    } else {
                        std::stringstream err;
                        err << "line " << lineno << ": unhandled character '" << c << "'";
                        throw std::runtime_error(err.str());
                    }
                    break;

                case STR_VALUE:
                    if (c == '"') {
                        state = SEARCH_EOL;
                        add_key_val(id, val);
                    } else if (c == '\n' || c == '\\') {
                        std::stringstream err;
                        err << "line " << lineno << ": unexpected end of string";
                        throw std::runtime_error(err.str());
                    } else {
                        val += c;
                    }
                    break;

                case SEARCH_EOL:
                    if (c == '\n') {
                        state = NONE;
                    } else if (std::isspace(c)) {
                        // OK
                    } else {
                        std::stringstream err;
                        err << "line " << lineno << ": unhandled character '" << c << "'";
                        throw std::runtime_error(err.str());
                    }
                    break;

                default:
                    std::stringstream err;
                    err << "line " << lineno << ": unhandled character '" << c << "'";
                    throw std::runtime_error(err.str());
            }
        }
        if (state != NONE) {
            std::stringstream err;
            err << "line " << lineno << ": unterminated assignment";
            throw std::runtime_error(err.str());
        }
    }

};
