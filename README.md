# IsaacGrimoire

## Introduction
IsaacGrimoire is a real-time cheat sheet for The Binding of Isaac: Afterbirth.
It will read the game's memory and automatically display every items' usage.
I hope it can save a player's time of searching item in cheat sheet and
memorizing pill effects in every run.

## Install
Currently, you can only compile it on OSX. I will support windows compilation
later.
First of all, make sure FLTK is installed. Then use the following instructions.

	mkdir build
	cd build
	cmake .. -DCMAKE_INSTALL_PREFIX=IsaacGrimoire
	make install

If nothing went wrong, you will get a directory IsaacGrimoire in build and an
IsaacGrimoire.app in it.

##How to Use
Open the app and run The Binding of Isaac: Afterbirth, it will automatically
show the names and descriptions of every items inside the room and on the
player.
**Note**: If you have not tasted a pill, the app will not show its effect.
If you are under the blind curse, the collectibles' info will be hidden, too.

##Cheat Table
I found most of the Mac version's address with Bit-Slicer. When I was writting
windows version, Cheat Engine helped a lot.
