# dwlblocks
A port of dwmblocks, for dwl/wayland, using stdout
# usage
To use dwlblocks first run 'make' and then install it with 'sudo make install'.
Once installed, simply pipe the output of dwlblocks into dwl
# modifying blocks
The statusbar is made from text output from commandline programs.
Blocks are added and removed by editing the blocks.h header file.
By default the blocks.h header file is created the first time you run make which copies the default config from blocks.def.h.
This is so you can edit your status bar commands and they will not get overwritten in a future update.
# known issues
If blocks are signaled fast enough, then a race condition can occur which results in only the block being signaled being returned
(This happens so infrequently and under such specific circumstances that I do not care to fix it)
