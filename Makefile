debug = #-ggdb
exe = hfm 
objects = hfm-main.o hfm-window.o hfm-view-directory.o hfm-view-mime.o hfm-view-file.o icon-hash.o
lib-flags = `gnome-config gnome --libs` `gnome-config gtk --libs` `gnome-config bonobox --libs`
include-flags = `gnome-config gnome --cflags` `gnome-config gtk --cflags` `gnome-config bonobox --cflags`

hfm : $(objects)
	gcc $(debug) -Wall -O2 $(lib-flags) $(include-flags) -o $(exe) $(objects) 

hfm-main.o : hfm-main.c
	gcc $(debug) -Wall -O2 -c $(include-flags) hfm-main.c 

hfm-window.o : hfm-window.c hfm-window.h
	gcc $(debug) -Wall -O2 -c $(include-flags) hfm-window.c

hfm-view-directory.o : hfm-view-directory.h hfm-view-directory.c
	gcc $(debug) -Wall -O2 -c $(include-flags) hfm-view-directory.c

hfm-view-mime.o : hfm-view-mime.h hfm-view-mime.c
	gcc $(debug) -Wall -O2 -c $(include-flags) hfm-view-mime.c

hfm-view-file.o : hfm-view-file.h hfm-view-file.c
	gcc $(debug) -Wall -O2 -c $(include-flags) hfm-view-file.c

icon-hash.o : icon-hash.h icon-hash.c
	gcc $(debug) -Wall -O2 -c $(include-flags) icon-hash.c

%.c %.h : %.gob
	/usr/bin/gob $<

clean:
	rm -fv $(exe) $(objects)
