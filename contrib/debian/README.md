
Debian
====================
This directory contains files used to package bitcored/bitcore-qt
for Debian-based Linux systems. If you compile bitcored/bitcore-qt yourself, there are some useful files here.

## bitcore: URI support ##


bitcore-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install bitcore-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your bitcore-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitcore128.png` to `/usr/share/pixmaps`

bitcore-qt.protocol (KDE)

