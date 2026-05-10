# soniccddecompilation

This is a version of BenoitRen's [Sonic CD decompilation](https://git.sr.ht/~benoitren/soniccddecompilation), edited to be buildable and to match the assembly code and data from the original PS2 version. BenoitRen's original version was largely accurate but had issues with missing or mistyped data that caused glitches in projects such as source ports that used it as a basis.

Currently, the source files compile to code/data that's identical to the PS2 ELF files, but the debug information does not match so the ELF files are not byte-for-byte identical.

## Code explanation

If you look at the code, you will probably have a few questions about why stuff is the way it is. The original version of Sonic CD is a port of the Sonic the Hedgehog game engine to the Sega CD. Because of the limited Sega CD RAM and the amount of content the developers wanted to include, the decision was made to make each individual level into its own fully contained executable.

The Windows 95 port of Sonic CD is more or less a direct translation of the original game's assembly code into C. I'm pretty sure it was done by hand (they fixed some bugs and the code doesn't read as machine generated to me), but they did stuff like using 68000 registers as variable names a lot. There's lots large gaps in the code indicated by the debug symbols, which is likely them pasting in a section of the Sega CD version's assembly code to use as a reference. Because it's a direct translation, the game keeps the same "one level = one executable" architecture, except because it's on Windows there's one frontend executable that loads DLLs for each level.

~9 years after the Windows 95 version, Sega released Sonic Gems Collection on GameCube and PS2. The version of Sonic CD included in Gems Collection is a port of the Windows 95 version to consoles. For the PS2 version of Gems Collection, Sega did not strip the debug symbols from the backend DLLs, which is why BenoitRen decided to use it for his decompilation project.