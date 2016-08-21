-- create table if not exists pages (id integer primary key, date timestamp, path varchar(255), content varchar);
-- insert into pages(date, path, content) values (datetime(), "/", "# Main\nThis is the top-level page.");

pragma foreign_keys = on;

create table if not exists page (
    id integer primary key,
    name varchar unique);

create index page_idx on page(name);

create table if not exists entry (
    id integer primary key,
    page_id integer not null references page,
    date timestamp,
    content text);

create index entry_idx on entry(page_id);
