colours.exe: colours.c
	cl colours.c user32.lib gdi32.lib user32.lib
    del colours.obj

clean:
	rm colours.exe