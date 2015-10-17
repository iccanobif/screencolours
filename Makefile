gayColours.exe: gayColours.c
	cl gayColours.c user32.lib gdi32.lib user32.lib
    del gayColours.obj

clean:
	rm gayColours.exe