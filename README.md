KArrowSwitch
=========

KArrowSwitch adds support for arrow keys in KDE default (Breeze) alt+tab/grave-accent task switcher.
it is very simply implemented, it detects alt+tab/grave-accent (hence an active task-switcher state)
then it emulates "tab/grave-accent" key when right/down arrows are pressed, and "shift+tab/grave-accent" for left/up arrows.

KArrowSwitch is a small desktop-agnostic application that runs in the background
as a daemon. It borrows its core engine from _ksuperkey_ by _Hans Chen_:
https://github.com/hanschen/ksuperkey

Installation
------------

There is a binary (compiled on Kubuntu 18.04) in the [releases section](https://github.com/nimatrueway/karrowswitch/releases).
Below are some generic instructions for how to compile KArrowSwitch from source.

1. Install dependencies. 

   Ubuntu:
    ```
    $ sudo apt-get install gcc cmake libx11-dev libxtst-dev
    ```

    CentOS:
    ```
    $ sudo yum install gcc cmake libX11-devel libXtst-devel
    ```


2. Clone project and compile:

    ```
    $ git clone https://github.com/nimatrueway/karrowswitch.git
    $ cd karrowswitch
    $ cmake .
    $ make
    ```

Usage
-----

    $ karrowswitch [-d] [--no-left-right]

`-d`

Debug mode. Does not fork into the background and logs keyboard events and
task-switcher detections and key emulations.

`--no-left-right`

Disables detection of left/right arrow keys, so only up/down arrows will be effective.

Contact
-------

The author can be reached at
nima.trueway at gmail dot com
