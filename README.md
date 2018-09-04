# Dungeon Keeper Remake
A modern remake of Dungeon Keeper

Let me start of by saying that I know of the existance of KeeperFX, however the source code is so unmanageable and unreadable to the point where I believe there's literally files that were copy pasted from the dissassembly code (There's a file with over 10K lines of ASM in it) with as extra that it's nearly all C.

So this will be a remake made in C++, using DirectX 11 and XAudio2. The goal will be to have a single executable file that you paste into the game folder which you can run. Therefore I will not distribute the games assets in any way (adjusted or not) keeping this project in the legal area.

So far I have used code from RNCTools (changed into a library) to decompress all of the RNC compressed assets and ripped out the video decoder from ScummVM to handle the Smacker video files (Intromix.smk, Outromix.smk and Drag.smk). I had originally looked into Libsmacker for decoding but it failed at properly decoding the video (there were artifacts like wrong colors which I believe is due to some issue with the palette swapping) and LibAV, but that just failed to initialise with the Smacker codec not to mention the nonexistance of documentation on how to use it as a video player.

Let me start of by saying this will NOT be a commercial project, I will not sell it or make money out of it in any form or shape. And in all honesty I haven't even looked at the licenses of the code I have used from the projects I mentioned above, but know that I mean no harm.

The "engine" I'm using is based of my previous project ThempX11 (also on my github) which is heavily adjusted to only do forward rendering (instead of deferred) and to support whatever else the game needs (such as Audio and Video)

At the current state (seeing as this project is not far in development yet):
The current build relies on the data folder that will be created once the project is compiled, this will change as development continues
So providing you have the original game at hand the folder structure should look like:


KEEPER   
|----SOUND   
|----LDATA   
|----DATA   
|----SAVE   
|----LEVELS   
|----Dungeon Keeper Remake.exe   
DATA   
|----TEXTURES  - defaultdiffuse.dds   
|----SHADERS - (with all the .cso's)   


