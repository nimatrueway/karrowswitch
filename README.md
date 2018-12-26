KARROWSWITCH
=========

karrowswitch adds support for arrow keys in KDE alt+tab task switcher.
it is very simply implemented. if it detects alt+tab (hence an active task switcher)
it emulates "tab" key when right/down arrows are pressed, and "shift+tab" for left/up arrows.

karrowswitch is a small desktop-agnostic application that runs in the background
as a daemon. It borrows its core engine ksuperkey by Hans Chen:
https://github.com/hanschen/ksuperkey

Installation
------------

There is a binary compiled on Kubuntu 18.04 in the release section.
Below are some generic instructions for how to compile karrowswitch from source.

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
    $ cmake
    ```

3. Install system-wide using e.g. `sudo` (optional):

    ```
    $ sudo make install
    ```

Usage
-----

    $ karrowswitch [-d]

`-d`

Debug mode. Does not fork into the background and logs keyboard events and
task-switcher detections and key emulations.

Contact
-------

The author can be reached at
nima.trueway at gmail dot com
