CC=clang
CFLAGS=-std=c99 -Wall -Wextra -g -D_DEFAULT_SOURCE

.PHONY: run clean all

all: compile

compile: main.c
	$(CC) $(CFLAGS) main.c -o ransomware
	mkdir -p /tmp/RANSOM_BACKUP
	cp -r ./* /tmp/RANSOM_BACKUP

run:
	./ransomware

clean:
	rm -f ~/ransomware
	rm -f ~/RANSOM_KEY

