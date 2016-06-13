cryfs-gui
=========

cryfs-gui[3] is a Qt/C++ front end to cryfs[1][2]

[1] https://www.cryfs.org/

[2] https://github.com/cryfs

[3] https://github.com/mhogomchungu/cryfs-gui/blob/master/images/cryfs-gui.jpeg

FAQ:
===+

1.0 How do i change the default mount point prefix?
===================================================
By default,cryfs-gui uses a mount point prefix of "~/.cryfs-gui.tmp".

You can change it to something else by creating a file at "~/.cryfs-gui/mountPrefix"
and add the path you want to be used as mount point prefix in the file on the first line.


2.0 How do i set custom mount point path for my volume?
=======================================================

Unique custom mount point path can be used through adding a volume to the favorite list and then
set custom mount point path in the "mount point path" field.The volume should then be accessed
through the favorites list.

Favorites list is access through Menu->Favorites.
