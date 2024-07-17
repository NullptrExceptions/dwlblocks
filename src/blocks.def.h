//Modify this file to change what commands output to your statusbar, and recompile using the make command.
//Update signals must go in order, due to dwl integration
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	{"",            "sb-volume",	       0,	       	1},
	{"",            "sb-clock",	       1,	       	2},
};

//sets delimiter between status commands. NULL character ('\0') means no delimiter.
//Warning: changing this will break dwl integration
static char delim[] = "|";
static unsigned int delimLen = 1;
