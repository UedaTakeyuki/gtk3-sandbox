%: %.c
	gcc -g -pg `pkg-config --cflags --libs gtk+-3.0` -o $@ $<
