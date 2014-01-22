# nFlood

    | |\ | | |_  | |   / / \ / / \ | | \
    |_| \| |_|   |_|__ \_\_/ \_\_/ |_|_/

nFlood is a simple clone of the *Flood-It* puzzle game.

You need to fill the whole board with a single color.
To do that, alternate between them, flooding with the
minimum number of moves possible.

It's played on the terminal, with text-based interface
via nCurses.

Heavily based on [those](http://unixpapa.com/floodit/) [three](http://floodit.appspot.com) online [games](http://cdn.labpixies.com/campaigns/flood/flood.html).

## Usage

Make sure you have `libncurses5-dev`.

Then, simply download (or `git clone` it) and `make` it

    $ make
    $ make run

## Screenshots

![screen1](http://alexdantas.net/projects/nflood/images/game.png)

![screen2](http://alexdantas.net/projects/nflood/images/game2.png)

## Controls

| Keys          | Actions |
| ------------- | ------- |
| Numbers (1~6) | Changes the flood color |
| q             | Quits the game at any time |
| r             | Resets the game board |

You can also use mouse to click on the board to flood it.

## Authors

* Copyright (C) 2011-2014  Alexandre Dantas <eu@alexdantas.net>
* Copyright (C) 2014 Yu-Jie Lin <livibetter@gmail.com>

## License

 This is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

