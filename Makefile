%: %.c
	gcc -g `pkg-config --cflags --libs gtk+-3.0` -o $@ $<
