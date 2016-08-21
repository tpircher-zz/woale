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

#ifndef WOALE_CONFFILEPARSER_H
#define WOALE_CONFFILEPARSER_H

#include "fileparser.h"
#include <string>



namespace woale {

    /**
     * Generic configuration file parser class.
     */
    class WoaleFileParser : public FileParser {
        public:
            /**
             * Initialise the woale configuration file parser.
             */
            WoaleFileParser();


            /**
             * Get the value of the data_dir configuration variable.
             * \return The value of data_dir.
             */
            const std::string data_dir() const
            {
                return data_dir_;
            }

            /**
             * Get the value of the http_data_path configuration variable.
             * \return The value of http_data_path.
             */
            const std::string http_data_path() const
            {
                return http_data_path_;
            }

            /**
             * Get the value of the js_render_lib configuration variable.
             * \return The value of js_render_lib.
             */
            const std::string js_render_lib() const
            {
                return js_render_lib_;
            }

        private:
            std::string data_dir_;
            std::string http_data_path_;
            std::string js_render_lib_;

            /**
             * Add a {key, val} tuple.
             */
            virtual void add_key_val(const std::string& key, const std::string &val);
    };

};

#endif  // WOALE_FILEPARSER_H
