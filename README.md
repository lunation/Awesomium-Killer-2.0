# Awesomium-Killer-2.0
[![Stories in Ready](https://badge.waffle.io/lunation/Awesomium-Killer-2.0.png?label=ready&title=Ready)](https://waffle.io/lunation/Awesomium-Killer-2.0)

[![forthebadge](http://forthebadge.com/images/badges/fuck-it-ship-it.svg)](http://forthebadge.com)
[![forthebadge](http://forthebadge.com/images/badges/just-plain-nasty.svg)](http://forthebadge.com)
[![forthebadge](http://forthebadge.com/images/badges/pretty-risque.svg)](http://forthebadge.com)

Awesomium replacement that uses CEF internally.

## Credits
Killing/Replacing Awesomium - Leystryku, Parakeet

CEF - Marshall A. Greenblatt, Google

## Installation
Download a build from the releases section, throw everything in the folder into `C:\Program Files (x86)\Steam\steamapps\common\GarrysMod\bin\`

You will need to re-install this after updates.

## Removal
Just verify the game cache.

## Contributing
**Warning: Source code is a total shitshow. Code at your own risk.**

**Software**
- VS2015
- CMake

**Instructions**

1. Clone the repo.
2. Run the premake script.
3. Get a CEF build from https://cefbuilds.com/ and put it in the "cef" directory of this repo. I'm currently using build 2704. If you use another build you might have to change the path of the dll wrapper shit. 
4. Run cmake on CEF, this is needed to build the dll wrapper. Disable 
5. Open up the CEF solution and do a release build.
6. Should be ready to build the module. The solution can be found in the "project" directory of the repo.
**Note: If the GPU process starts crashing a lot, replace the GL libraries with the ones from build 2526. You'll know if the GPU process is crashing, the screen will flicker like a crazy bitch.**

**Configurations**

Test - Enables logging, copies output to garrysmod directory, which you may need to change on your machine.
Release - Enables optimizations. Copies to the "release" directory in the repo.

**Project Setup**

I recommend that you read about CEF's architecture if you want to contribute, as it is a bit complex: https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-important-concepts

The project consists of two parts: The replacement for the awesomium module, and the subprocess executable. The module (and garrysmod, by extension) act as the "main" process, while the subprocess is used for rendering and javascript shenanigans.
