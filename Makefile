CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -g -D_DEFAULT_SOURCE `pkg-config --cflags gtk4`
LDFLAGS= 

.PHONY: run clean all

all: compile

compile: main.c
	$(CC) $(CFLAGS) main.c `pkg-config --libs gtk4` -o ransomware
	mkdir -p /tmp/RANSOM_BACKUP
	cp -r ./* /tmp/RANSOM_BACKUP

run:
	./ransomware

clean:
	rm -f ~/ransomware
	rm -f ~/RANSOM_KEY

