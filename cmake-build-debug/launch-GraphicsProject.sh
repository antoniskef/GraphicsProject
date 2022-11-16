#!/bin/sh
bindir=$(pwd)
cd /Users/antoniskefallonitis/GraphicsProject/GraphicsProject/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/antoniskefallonitis/GraphicsProject/cmake-build-debug/GraphicsProject 
	else
		"/Users/antoniskefallonitis/GraphicsProject/cmake-build-debug/GraphicsProject"  
	fi
else
	"/Users/antoniskefallonitis/GraphicsProject/cmake-build-debug/GraphicsProject"  
fi
