# Installation

In order to build woale you need:
- SQLite 3 development files
- cgicc development files
- cmake

Build woale and install it:

    INSTALL_DIR=/some/install/prefix
    ./configure --prefix=$INSTALL_DIR
    cd build
    make && make install


## Create SQLite database:

    sqlite3 $INSTALL_DIR/var/lib/woale/pages.sqlite3 --batch < data/doc/schema-sqlite.sql

Make sure the web server has read-write access to the database:

    chown www-data: $INSTALL_DIR/var/lib/woale/pages.sqlite3


## Install the markdown JavaScript library

Get the [marked] markdown parser and compiler from your distribution or from
GitHub and copy `marked.min.js` to a location reachable by your web server.

Configure `js_render_lib` in `/ets/woale.conf` to point to your *http*
*location* of `marked.min.js`.

## Configure the web server to server the CGI

Finally, configure the web server to use woale.cgi, e.g. in Apache:

    RedirectMatch ^/wiki$ /wiki/
    ScriptAlias /wiki/ /usr/lib/woale/woale.cgi/
    <Location /wiki/>
            Allow from all
            Options ExecCGI FollowSymlinks
    </Location>

    Alias /wiki-files/ /usr/share/woale/
    <Location /wiki-files/>
            Allow from all
    </Location>


# Testing

The easiest way to test woale.cgi without installing a web server is to start a
HTTP server using Python.  Build and install the code as above, e.g. using
`INSTALL_DIR=/tmp/woale-test`.

Then create an empty SQLite database (see above).
Create a symlink to the metadata such as the CSS and the JavaScript library:

    ln -s $INSTALL_DIR/share/woale wiki-files

Finally start the HTTP server:

    #!/usr/bin/env python3

    import http.server
    import socketserver
    import os

    def main():
        port = 8000
        host = 'localhost'
        handler = http.server.CGIHTTPRequestHandler
        handler.cgi_directories = ['/src']
        print('http://{}:{}/src/woale.cgi'.format(host, port))
        server = socketserver.TCPServer(('', port), handler)
        server.server_name = host
        server.server_port = port
        try:
            server.serve_forever()
        except KeyboardInterrupt:
            server.socket.close()

    if __name__ == '__main__':
        main()


[marked]: https://github.com/chjj/marked
