rpi-tools
=========

A few utilities I've made for my Raspberry Pi.

tmux-stats
----------

Prints a rather minimalist list with the load-average, the cpu usage, the
temperature and the memory usage.  
I use it in my tmux status bar.

Example:

	$ echo `tmux-stats`
	L:0.27 C:2% M:93% T:46C

