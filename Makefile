setup:
	gcc -shared -fPIC -Wall -O -I/usr/include/libusb-1.0 byteconverter.c bb2d.c -lusb-1.0 -o libbb2d.so

install: libbb2d.so
	cp bb2d.h /usr/local/include/
	mv libbb2d.so /usr/local/lib/
	ldconfig
