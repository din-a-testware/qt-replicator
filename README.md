# REPLICATOR

## This is a program written in Qt/C++ for the purpose of a kitchen-computer.
Mainly it's intended to view recipes, for people who don't like to have much paper lying nearby while cooking.
It's in the style of the **StarTrek TNG LCARS Panels**.
The project name is obvious.
Despite of that it can be ported to severals platforms (thanks to Qt) it's mainly made for the raspberry Pi with it's official touchscreen.
It's written in Qt 5.10. A prebuild cross-compile-toolchain will be available soon and as well a working raspbian jessie image with the replicator preinstalled.

It's splitted into a server, a client and a recipe-editor.
The client gets the recipe data from the server via websockets. The reason for this is, that multiple clients can connect to the server and the server does not have to be installed on the client device.
An Android-version is also a work-in-progress.

In the ***build***-folder there are prebuild binaries for *linux-x64* and *raspbian-armv7*.

And if you like it, you can buy me a cup of Earl Grey or a little ebook or something like that ;)

[![paypal](https://www.paypalobjects.com/en_GB/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=8K9FHDF3E76XN)

https://www.amazon.de/hz/wishlist/ls/266KAWC8W2W57?

***General notes:*** I've restructured the Music-Database, so the internal creation of the database does not work right now. I will fix it soon.

***Note when building:*** There are some files in the qresource file, which I did not upload, due to license issues or unnecessity, but I did not have the leisure to update the qrc file yet. So, when the compiler complains about missing files, just delete them from the qrc.

***And than it has some additional features:***

***The client***:
**Musicplayer**: You can choose between a database (sqlite) or browse the local files.

**Videoplayer**: Same as the music player, though the database function is not written yet.

**Notes**: Some notes can be written into the database.

**Mark recipes**: You can mark the recipes you like or want to use in the (near) future.

**Timer**: Countdown timer, so you don't have to look at the clock. When it has timed out, the red alert will start. Presets are also available. There can be run multiple timers at the same time.

**Ambient sounds**: Maybe you like to have an ambient warpcore-sound, so it can be enabled. You can also add your own sounds too.
Lock system: opens a form where you have to pass a code with the four buttons. At the moment it is hardcodes alpha-beta-gamma-delta but as soon as possible I will add the function to change it in the system settings menu.

**Settings**: Various system and server settings.

**Language support**: A translation system is also implemented. At the moment there are German and English translations available. Feel free to contribute more :)

***Not yet 100% implemented, but started***

**Equalizer**: An audio equalizer. This will be implemented, when switched to QtAv

**Color themes**: Started work on it. 


***The server:***
The server has a command line option to activate a user-input console, where you can set/get settings or execute a SQLite or system-command. It is in an early stage so it is a *little* bit buggy.

I'm using the font "**Helvetica Ultra Compressed**". Sadly it isn't under a free license, so I cannot provide it in this repository, but I'm sure you'll get it by yourself. ;)

Though it is at a good usable stage, much work has to be done. The code in general needs a **big cleanup**. In the future I will switch from QMultimedia to QtAV. QMultimedia is ok, but has not the best performance and reliability. The creation of the music database is also not perfect. I'm using the c++ interface from Exiftool, but curiously it doesn't parse the cover art correct.

So far, have fun with it. :)

And now some screenshots and a video:

[![Youtube video](http://img.youtube.com/vi/p8D_BUBRod4/0.jpg)](http://www.youtube.com/watch?v=p8D_BUBRod4)

![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/media_database1.png "Music Database Folders")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/media_database2.png "Music Database Artists")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/media_start.png "Media Tab")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/music_player1.png "Music Player Details")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/music_player_playlist.png "Music Player Playlist")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/music_player_mode.png "Music Player Playback Mode")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/media_database1.png "Music Database Folders")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/notes.png "Notes")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/recipe_read.png "The recipe itself")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/recipe_search.png "Search recipe")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/recipe_start.png "Recipe start with image")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/serversettings.png "Server settings")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/settings_1.png "Some settings")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/settings_display.png "Display Settings")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/system_ambient.png "Ambient sounds")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/system_audio.png "Audio settings")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/system_equalizer.png "Equalizer")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/system_language.png "Language chooser")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/system_locked.png "Lock screen")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/system_network.png "Network interfaces")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/system_run.png "System command")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/system.png "System")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/timer.png "Timer")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/database_settings.png "Database settings")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/keyboard1.png "Keyboard")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/keyboard2.png "Keyboard")
![alt text](https://raw.githubusercontent.com/din-a-testware/qt-replicator/master/screenshots/keyboard3.png "Keyboard")

