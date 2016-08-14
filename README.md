# Woale Wiki Engine

woale is a small and fast cgi wiki engine written in C++.


## Installation

In order to build woale you need:
- SQLite3 development files
- cgicc development files
- cmake

Build woale:
    INSTALL_DIR=/some/install/prefix
    ./configure --prefix=$INSTALL_DIR
    cd build
    make && make install

### Create sqlite database:
    sqlite3 $INSTALL_DIR/var/lib/woale/pages.sqlite3 \
            'create table if not exists pages (id integer primary key, date timestamp, path varchar(255), content varchar);'
    sqlite3 $INSTALL_DIR/var/lib/woale/pages.sqlite3 \
            'insert into pages(date, path, content) values (datetime(), "/", "# Main\nThis is the top-level page.");'

## Testing

The easiest way to test woale.cgi is to start a HTTP server using Python.
Build and install the code as above, e.g. with `INSTALL_DIR=/tmp/woale-test`.

Then create an empty sqlite database (see above).
Create a symlink to the metadata such as the CSS and the javascript library:
    ln -s $INSTALL_DIR/share/woale wiki-files

Finally start the HTTP server:
    #!/usr/bin/env python3

    import http.server
    import socketserver
    import os

    def main():
        port = 8000
        handler = http.server.CGIHTTPRequestHandler
        handler.cgi_directories = ['/src']
        print('http://localhost:{}/src/woale.cgi'.format(port))
        server = socketserver.TCPServer(('', port), handler)
        server.server_name = 'wusel.tty1.net'
        server.server_port = port
        try:
            server.serve_forever()
        except KeyboardInterrupt:
            server.socket.close()

    if __name__ == '__main__':
        main()
