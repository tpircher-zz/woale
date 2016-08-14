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
 * A woale config file parser.
 */

#include "config.h"
#include "woale_confparser.h"
#include <sstream>


namespace woale {

    WoaleFileParser::WoaleFileParser() :
        FileParser(),
        data_dir_(INSTALL_LOCALSTATE_DIR "/lib/woale"),
        http_data_path_("/wiki-files")
    {
    }


    void WoaleFileParser::add_key_val(const std::string& key, const std::string &val)
    {
        if (key == "data_dir") {
            data_dir_ = val;
        } else if (key == "http_data_path") {
            http_data_path_ = val;
        } else {
            std::stringstream err;
            err << "unknown configuration key '" << key << "'.";
            throw std::runtime_error(err.str());
        }
    }

};
