debug = -ggdb
exe = hfm 
objects = hfm.o 
lib-flags = `gnome-config gnome --libs` `gnome-config gtk --libs` `gnome-config bonobox --libs`
include-flags = `gnome-config gnome --cflags` `gnome-config gtk --cflags` `gnome-config bonobo --cflags`

fm: $(objects)
	gcc $(debug) -o $(exe) $(objects) $(lib-flags)

hfm.o : hfm.c 
	gcc $(debug) -Wall -O2 -c hfm.c $(include-flags)

clean:
	rm -fv $(exe) $(objects)
