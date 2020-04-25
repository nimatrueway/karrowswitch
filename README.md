PauseChangeFixer
=========

PauseChangeFixer is a tiny tool for myself, to make golden-dict popup works while I mapped my new BakkerElkhuizen
keyboard's Pause key to insert. Although through `xmodmap -e "keycode 127 = Insert Insert Insert Insert Insert"` I
changed my `Pause` key to `Insert` the keycode is still unchanged and golden-dict works based on keycode. I created this
tool to make my golden-dict popup shortkey `2 x Ctrl+Ins` work by basically detecting `2 x Ctrl+Pause` and then
simulating the original shortkey. 

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
    $ sudo apt-get install gcc cmake build-essential libx11-dev libxtst-dev
    ```

    CentOS:
    ```
    $ sudo yum install gcc cmake build-essential libX11-devel libXtst-devel
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

    $ pause-change-fixer [-d]

`-d`

Debug mode. Does not fork into the background and logs keyboard events and
task-switcher detections and key emulations.

Contact
-------

The author can be reached at
nima.trueway at gmail dot com
