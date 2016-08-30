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
#include <tuple>
#include <utility>

using namespace std::string_literals;

namespace woale {

    static bool simple_query(sqlite3 *db, const char* request)
    {
        sqlite3_stmt *stmt;
        const char *pzTail;

        auto rc = sqlite3_prepare(db, request, -1, &stmt, &pzTail);
        if (rc != SQLITE_OK) {
            return false;
        }

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }


    DbConn::DbConn(const std::string file_name) :
        db_(nullptr)
    {
        if (sqlite3_open_v2(file_name.c_str(), &db_, SQLITE_OPEN_READWRITE, 0) != 0) {
            throw std::runtime_error("Failed to open the sqlite database file");
        }

        sqlite3_stmt *stmt;
        const char *pzTail;

        const auto sql_request = "pragma foreign_keys = on;";
        auto rc = sqlite3_prepare_v2(db_, sql_request, -1, &stmt, &pzTail);
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


    std::pair<const std::string, unsigned int> DbConn::get_page(const std::string page_name) const
    {
        sqlite3_stmt *stmt;
        const char *pzTail;

        const auto sql_request = "select entry.id, content from entry inner join page on page_id = page.id"
            " where name = ?1 order by date desc limit 1;";
        auto rc = sqlite3_prepare_v2(db_, sql_request, -1, &stmt, &pzTail);
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
            return std::make_pair(""s, 0);
        } else if (rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Failed to fetch row");
        }

        auto page_ver = sqlite3_column_int(stmt, 0);

        auto content = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        auto str = content == NULL ? ""s : content;
        sqlite3_finalize(stmt);

        return std::make_pair(str, page_ver);
    }


    bool DbConn::save_page(const std::string page_name, const std::string content, unsigned int page_ver)
    {
        sqlite3_stmt *stmt;
        const char *pzTail;

        /* Lock the table. */
        if (!simple_query(db_, "begin transaction;")) {
            throw std::runtime_error("Failed to begin transaction");
        }

        std::string dummy;
        unsigned int next_entry_id;
        std::tie(dummy, next_entry_id) = get_page(page_name);

        if (next_entry_id + 1 != page_ver) {
            simple_query(db_, "end transaction;");
            return false;
        }

        /* Insert the page name. */
        const auto ins_page = "insert or ignore into page (name) values (?1);";
        auto rc = sqlite3_prepare(db_, ins_page, -1, &stmt, &pzTail);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare sqlite statement");
        }
        sqlite3_bind_text(stmt, 1, page_name.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            throw std::runtime_error("Failed to insert data");
        }
        sqlite3_finalize(stmt);


        /* Insert the new wiki content. */
        const auto ins_entry = "with ins (page_name, date, content) as (values (?1, datetime(), ?2)) "
            "insert into entry (id, page_id, date, content) select ?3, page.id, ins.date, ins.content "
            "from page join ins on ins.page_name = page.name;";
        rc = sqlite3_prepare(db_, ins_entry, -1, &stmt, &pzTail);
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

        /* Unlock the table. */
        if (!simple_query(db_, "end transaction;")) {
            throw std::runtime_error("Failed to begin transaction");
        }

        return true;
    }
};
