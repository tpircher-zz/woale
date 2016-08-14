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
 * The interface to the database backend.
 */

#ifndef WOALE_DBCONN_H
#define WOALE_DBCONN_H


#include <string>


struct sqlite3;


namespace woale {


    /**
     * Database interface class.
     *
     * This is a very simple wrapper to the SQLite database. It supports
     * querying and inserting pages to a table.
     */
    class DbConn {
        public:
            /**
             * Open a database connection.
             * \param[in] file_name the path to the database file.
             */
            DbConn(const std::string file_name);

            /* This calss has pointer data members. Therefore we disable the
             * copy constructor and the copy operator. */
            DbConn(const DbConn&) = delete;
            DbConn& operator=(const DbConn&) = delete;

            /**
             * Cclean up the database.
             */
            ~DbConn();


            /**
             * Return the content of a page.
             * \param[in] page_name the name of the page to return.
             * \return the content of a page.
             */
            const std::string get_page(const std::string page_name) const;

            /**
             * Save a new version of a page.
             * \param[in] page_name the name of the page to return.
             * \param[in] content the content of the page to save.
             */
            void save_page(const std::string page_name, const std::string content);

        private:
            /**
             * A pointer to a sqlite structure.
             * This pointer is initialised in the calss constructor and closed
             * in the destructor.
             */
            sqlite3 *db_;
    };

};

#endif  // WOALE_DBCONN_H
