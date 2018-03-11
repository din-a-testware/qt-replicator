# REPLICATOR

## This is a program written in Qt/C++ for the purpose of a kitchen-computer.
Mainly it's intended to view recipes, for people who don't like to have much paper lying nearby while cooking.
It's in the style of the **StarTrek TNG LCARS Panels**.
The project name is obvious.
Despite of that it can be ported to severals platforms (thanks to Qt) it's mainly made for the raspberry Pi with it's official touchscreen.
It's written in Qt 5.10. A prebuild cross-compile-toolchain will be available soon and as well a working raspbian jessie image with the replicator preinstalled.

It's splitted into a server and a client.
The client gets the recipe data from the server via websockets. The reason for this is, that multiple clients can connect to the server and the server does not have to be installed on the client device.
An Android-version is also a work-in-progress.

***And than it has some additional features:***

***The client***:
**Musicplayer**: You can choose between a database (sqlite) or browse the local files.

**Videoplayer**: Same as the music player, though the database function is not written yet.

**Notes**: Some notes can be written into the database.

**Mark recipes**: You can mark the recipes you like or want to use in the (near) future.

**Timer**: Countdown timer, so you don't have to look at the clock. When it has timed out, the red alert will start. Presets are also available.

**Ambient sounds**: Maybe you like to have an ambient warpcore-sound, so it can be enabled. You can also add your own sounds too.
Lock system: opens a form where you have to pass a code with the four buttons. At the moment it is hardcodes alpha-beta-gamma-delta but as soon as possible I will add the function to change it in the system settings menu.

**Settings**: Various system and server settings.

**Language support**: A translation system is also implemented. At the moment there are German and English translations available. Feel free to contribute more :)

***The server:***
The server has a command line option to activate a user-input console, where you can set/get settings or execute a SQLite or system-command. It is in an early stage so it is a *little* bit buggy.

I'm using the font "**Helvetica Ultra Compressed**". Sadly it isn't under a free license, so I cannot provide it in this repository, but I'm sure you'll get it by yourself. ;)

Though it is at a good usable stage, much work has to be done. The code in general needs a **big cleanup**. In the future I will switch from QMultimedia to QtAV. QMultimedia is ok, but has not the best performance and liability. The creation of the music database is also not perfect. I'm using the c++ interface from Exiftool, but curiously it doesn't parse the cover art correct.

A Video will be available soon too.

So far, have fun with it. :)
