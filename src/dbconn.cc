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

        sqlite3_stmt *stmt;
        const char *pzTail;
        int rc;

        const std::string sql_request = "pragma foreign_keys = on;";
        rc = sqlite3_prepare_v2(db_, sql_request.c_str(), -1, &stmt, &pzTail);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare sqlite statement");
        }
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }


    DbConn::~DbConn()
    {
        sqlite3_close(db_);
    }


    const std::string DbConn::get_page(const std::string page_name, unsigned int &page_ver) const
    {
        sqlite3_stmt *stmt;
        const char *pzTail;
        int rc;

        const std::string sql_request = "select entry.id, content from entry inner join page on page_id = page.id "
            "where name = ?1 order by date desc limit 1;";
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
            page_ver = 0;
            return "";  // "# Empty page.\nPress [edit](?edit=1) to edit this page.";
        } else if (rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Failed to fetch row");
        }

        page_ver = sqlite3_column_int(stmt, 0);

        std::string str;
        const char *content = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        if (content == NULL) {
            str = "";
        } else {
            str = content;
        }
        sqlite3_finalize(stmt);

        return str;
    }

    bool DbConn::save_page(const std::string page_name, const std::string content, unsigned int page_ver)
    {
        sqlite3_stmt *stmt;
        const char *pzTail;
        int rc;

        const std::string ins_page = "insert or ignore into page (name) values (?1);";
        rc = sqlite3_prepare(db_, ins_page.c_str(), -1, &stmt, &pzTail);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare sqlite statement");
        }
        sqlite3_bind_text(stmt, 1, page_name.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            throw std::runtime_error("Failed to insert data");
        }
        sqlite3_finalize(stmt);

        const std::string ins_entry = "with ins (page_name, date, content) as (values (?1, datetime(), ?2)) "
            "insert into entry (id, page_id, date, content) select ?3, page.id, ins.date, ins.content "
            "from page join ins on ins.page_name = page.name;";
        rc = sqlite3_prepare(db_, ins_entry.c_str(), -1, &stmt, &pzTail);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare sqlite statement");
        }
        sqlite3_bind_text(stmt, 1, page_name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, page_ver);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            throw std::runtime_error("Failed to insert data");
        }
        sqlite3_finalize(stmt);

        return true;
    }
};
