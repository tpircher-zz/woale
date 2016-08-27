# Woale Wiki Engine

woale is a small and fast cgi wiki engine written in C++.

The wiki text is rendered from text to HTML in the user's browser using a
JavaScript library. The wiki has been tested using the [marked] JavaScript
markdown parser, but other parsers could be used as well.
This makes the woale source code simple and straightforward as it does not do
any transformation on the wiki text and only retrieves the wiki text from and
stores it to a SQLite database.

Download the source code from the [woale] GitHub page.

*Note: woale is currently in ALPHA stage and has not been fully tested and/or
hardened. Be warned.*

## Installation

See INSTALL.md for installation instructions.

## Feedback

Please submit feedback, bugs and patches on the [woale] GitHub page.

## Copyright

Copyright (C) 2015-2016  Thomas Pircher <tehpeh-web@tty1.net>

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

See the LICENSE file for the full text of the GPLv3.

[woale]: https://github.com/tpircher/woale
[marked]: https://github.com/chjj/marked
[markdown]: https://en.wikipedia.org/wiki/Markdown
