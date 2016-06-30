# Awesomium-Killer-2.0
[![Stories in Ready](https://badge.waffle.io/lunation/Awesomium-Killer-2.0.png?label=ready&title=Ready)](https://waffle.io/lunation/Awesomium-Killer-2.0)

[![forthebadge](http://forthebadge.com/images/badges/fuck-it-ship-it.svg)](http://forthebadge.com)

[![forthebadge](http://forthebadge.com/images/badges/just-plain-nasty.svg)](http://forthebadge.com)

[![forthebadge](http://forthebadge.com/images/badges/pretty-risque.svg)](http://forthebadge.com)

Awesomium replacement that uses CEF internally.

CEF BUILD **2704.1431**

USING OPENGL LIBS FROM **2526.1373**

#Credits
Killing/Replacing Awesomium - Leystryku, Parakeet
CEF - Marshall A. Greenblatt, Google

#Installing (OLD)
Put everything from the dieawesomium folder into C:\Program Files (x86)\Steam\steamapps\common\GarrysMod or if not on x64 C:\Program Files\Steam\steamapps\common\GarrysMod, or wherever you installed gmod.
Do the same with everything from the Textures folder.
If you want to, then you can also do it with dieawesomium_mcorerender for some more speed.

If asked to, click yes on every replace.
It should replace Awesomium.dll and some other stuff.
If not, check that you installed it to the right directory.

In case it doesn't work out of the box ( crash with a MessageBox on startup ), try installing https://www.microsoft.com/en-us/download/details.aspx?id=48145 ( 32 bit aka x86 ),
and then reinstall this package.

If you're updating, make sure you delete the old files and verify the games integrity before doing so!

Screenshot:
http://i.imgur.com/C3O2Yjp.jpg

install + showcase ( crash@options is from playing around with sprays aka - not related to this ):
https://www.copy.com/s/t%3Ax7IN31vbQiIs%3Bp%3A%252F2015-09-20_02-39-15.mp4%3Boid%3A165

#Compiling (OLD)
If you wish to compile this yourself, head to the src/ directory, open premake5.lua in your favourite text editor, edit the `sdk_dir` variable to point to the 'src' directory of Valve's Source SDK. Save the file and run `premake5 <target>` within that directory; a `projects` folder will then be created. The only supported platform right now is Windows. Feel free to contribute!
