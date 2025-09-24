# Ransomware

As the name suggests, this is a malicous program to encrypt all your files and
demand a password to unencrypt them. This was made as a small fun project to pass the time, and not for bad purposes. I might work on hardening this to make it more difficult to get around whenever I'm bored enough.

## Compiling

All you need is clang and an unix system. Typing `make` will compile it, then
`make run` to run it. A backup of the source gets created in the
`/tmp/RANSOM_BACKUP` directory.

## Behavior

Once ran, it will move itself to the users home directory. From there it will
create a key the size of `KEY_SIZE`, and encrypt every file recursively
(ignoring dot files)

To unencrypt them, you need to enter the password stored in the macro
`PASSWORD` (default is just "password.") If you close the program, you can run it again to unencrypt
your files

Once you enter the correct password, the program will decrypt all your files
and delete itself and the key
