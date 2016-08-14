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

#include "dbconn.h"
#include <sqlite3.h>
#include <stdexcept>

namespace woale {

    DbConn::DbConn(const std::string file_name) :
        db_(nullptr)
    {
        if (sqlite3_open_v2(file_name.c_str(), &db_, SQLITE_OPEN_READWRITE, 0) != 0) {
            throw std::runtime_error("Failed to open the sqlite database file");
        }
    }


    DbConn::~DbConn()
    {
        sqlite3_close(db_);
    }


    const std::string DbConn::get_page(const std::string page_name) const
    {
        sqlite3_stmt *stmt;
        const char *pzTail;
        int rc;

        const std::string sql_request = "select content from pages where path = ?1 order by date desc limit 1;";
        rc = sqlite3_prepare_v2(db_, sql_request.c_str(), -1, &stmt, &pzTail);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare sqlite statement");
        }

        rc = sqlite3_bind_text(stmt, 1, page_name.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare bind variable");
        }

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return "";  // "# Empty page.\nPress [edit](?edit=1) to edit this page.";
        } else if (rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Failed to fetch row");
        }

        std::string str;
        const char *content = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        if (content == NULL) {
            str = "";
        } else {
            str = content;
        }
        sqlite3_finalize(stmt);

        return str;
    }

    void DbConn::save_page(const std::string page_name, const std::string content)
    {
        sqlite3_stmt *stmt;
        const char *pzTail;
        int rc;

        const std::string sql_request = "insert into pages(date, path, content) values (datetime(),?1,?2);";
        rc = sqlite3_prepare(db_, sql_request.c_str(), sql_request.length(), &stmt, &pzTail);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare sqlite statement");
        }
        sqlite3_bind_text(stmt, 1, page_name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            throw std::runtime_error("Failed to insert data");
        }
        sqlite3_finalize(stmt);
    }
};
