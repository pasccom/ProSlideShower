REPOSITORY DESCRIPTION
----------------------

This repository contains the code of a light-weight PDF viewer
based on libPoppler, especially designed for presentations.


FEATURES
--------

When run on a single monitor system (or a system cloning a single display),
it renders the PDF file, with maximum size without changing the aspect ratio.
Options for the display are accessible via context menu and
navigation is possble with arrow keys:
- Go to next slide: Right arrow, down arrow, page down.
- Go to previous slide: Left arrow, up arrow, page up.
This should be compatible with presentation remote controls.

When run on multiple monitors system, it renders PDF files on all secondary
screens, while a presentation controller is shown on the primary screen.
The displays are configurable via context menu. In addition to display
up to three different slides, the controller features:
- Progress bars: One give the slide (or frame if Beamer `*.aux`
or `*.nav` file can be found) progress; the other displays the time ellapsed
over the total presentation time (which can be configured in the configure menu).
The color of the progress bar tells whether the presentation is too fast or too slow
as follows: Blue is too fast, green is fast, white is OK, yellow is slow, and red very slow.
The presentation starts automatically when the next slide is shown. However, it can also
be managed with \ref shortcuts.
- Miniature slide view: It is shown when the mouse approaches the controller window top
and allows to go to the desired slide directly by double clicking. The miniature gallery is
synchronized with the presentation.
- Tool buttons: These buttons are also located in the auto-hide pane. There is the open master
button, which loads a master document on all screens, a configure button and a close button
to close the presentation.

KEYBOARD SHOTCUTS
-----------------
- Navigation shortcuts
    - **Right arrow**: Go to next slide
    - **Left arrow**: Go to previous slide
    - **Down arrow**: Go to next slide
    - **Up arrow**: Go to previous slide
    - **Page down**: Go to next slide
    - **Page up**: Go to previous slide
- Presentation management shortcuts
    - **Alt + F4**: Closes the application (all windows)
    - **Ctrl + S**: Starts presentation, i.e., starts presentation timer
    - **Ctrl + P**: Pauses presentation, i.e. stops presentation timer but do not reset it
    - **Ctrl + X**: Stops presentation, i.e. stops presentation timer and reset it

INSTALLATION
------------

To compile this project, you may need to give some information to `qMake`:
It requires libPoppler to compile.

On Unix-based system, you just have to install the development package
for libPoppler and everything should be setup for you.

On Windows systems, you will have to tell `qMake` where libPoppler is located.
You can do this by passing the variables `LIB_POPPLER_INCLUDE_PATH` and `LIB_POPPLER_LIBRARY_PATH`
on the command line or write a file called `ProSlideShower.local.pri` containing the following code:
```qmake
win32 {
    isEmpty(LIB_POPPLER_INCLUDE_PATH): LIB_POPPLER_INCLUDE_PATH="C:/Path/to/Poppler/include"
    isEmpty(LIB_POPPLER_LIBRARY_PATH): LIB_POPPLER_LIBRARY_PATH="C:/Path/to/Poppler/library"
}
```
An example file (`ProSlideShower.local.pri.example`) is provided along with the souces.

PLANNED DEVELOPMENTS
--------------------

Here are some ideas I plan to implement later:
- Support for hyperlinks in presentation
- Better style implementation so that it can be reused in other applications.
- Improvement of the gallery view code (and testing).
- Create a plugin system to display other file types.

If you have any other feature you will be interested in, please let me know.
I will be pleased to develop it if I think it is a must have.

If you want to implement extension, also tell me please. Admittedly you
can do what you desire with the code (under the constraints stated in the
section LICENSING INFORMATION below), but this will avoid double work.

LICENSING INFORMATION
---------------------

Pro SlideShower is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Pro SlideShower is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Pro SlideShower. If not, see http://www.gnu.org/licenses/
