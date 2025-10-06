CC=clang
CFLAGS=-std=c11 -Wall -Wextra -g -D_DEFAULT_SOURCE
LDFLAGS=`pkg-config --cflags gtk4` `pkg-config --libs gtk4`

.PHONY: run clean all

all: compile

compile: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) main.c -o ransomware
	mkdir -p /tmp/RANSOM_BACKUP
	cp -r ./* /tmp/RANSOM_BACKUP

run:
	./ransomware

clean:
	rm -f ~/ransomware
	rm -f ~/RANSOM_KEY

