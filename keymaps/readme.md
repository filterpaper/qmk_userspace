Copyright (C) 2021 @filterpaper

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

# QMK layout files in JSON format

Keyboard, keymap name and layout are defined in each JSON file. They will be used by `qmk compile <keyboard>.json` to build its firmware. Each layer in the JSON content are preprocessor macro references to the actual layout documented in `layout.h`.
