# Dungeon Keeper Remake
A modern remake of Dungeon Keeper

Let me start of by saying that I know of the existance of KeeperFX, however the source code is so unmanageable and unreadable to the point where I believe there's literally files that were copy pasted from the dissassembly code (There's a file with over 10K lines of ASM in it) with as extra that it's nearly all C.

So this will be a remake made in C++, using DirectX 11 and XAudio2. The goal will be to have a single executable file that you paste into the game folder which you can run. Therefore I will not distribute the games assets in any way (adjusted or not) keeping this project in the legal area.

In order to compile this project you'll need the Windows 10 SDK and Visual Studio 2017.

So far I have used code from RNCTools (changed into a library) to decompress all of the RNC compressed assets and ripped out the video decoder from ScummVM to handle the Smacker video files (Intromix.smk, Outromix.smk and Drag.smk).

The "engine" I'm using is based of my previous project ThempX11 (also on my github) which is heavily adjusted to only do forward rendering (instead of deferred) and to support whatever else the game needs (such as Audio and Video)

Providing you have the original game at hand the folder structure should look like:


KEEPER\\   
|----SOUND\\   
|----LDATA\\   
|----DATA\\   
|----SAVE\\   
|----LEVELS\\   
|----Dungeon Keeper Remake.exe

![Screenshot](https://github.com/Themperror/Dungeon-Keeper-Remake/blob/master/DKImage1.png)
![Screenshot](https://github.com/Themperror/Dungeon-Keeper-Remake/blob/master/DKImage2.png)

This will NOT be a commercial project, I will not sell it or make money out of it in any form or shape.
