# Vinace - VINtage Apple Computer Emulator

## Requirements

Vinace needs gtkmm-2.4 and libpulse to be installed.

On Ubuntu :
```
apt install libgtkmm-2.4-dev libpulse-dev
```

## How to build
```
git clone https://github.com/pyrollo/vinace
cd vinace
cmake .
```

## Fetch ROMs

Vinace needs ROM images which are copyrighted and which can not be
distributed with Vinace.

To run vinace, ROM files have to be fetched from
[https://ftp.apple.asimov.net/emulators/rom_images](https://ftp.apple.asimov.net/emulators/rom_images).
Current version uses
[APPLE2E.ROM](https://ftp.apple.asimov.net/emulators/rom_images/apple_iie_rom.zip)
file.

```
cd roms
wget ftp://ftp.apple.asimov.net/pub/apple_II/emulators/rom_images/apple_iie_rom.zip
unzip apple_iie_rom.zip
cd ..
```

## Play

From ``vinace`` directory, launch:

```
bin/vinace
```

## Inside Vinace

Now you should see a color screen and two disk drives. If ROM file is
present, screen should display "Apple //e" and floppy drive #1 should
be on.

Emulated Apple //e is waiting for you to insert a disk. To do it so,
drag and drop a .DSK file onto drive 1. Such files could be found at
[https://ftp.apple.asimov.net/images/](https://ftp.apple.asimov.net/images/).

Issue a Ctrl-Break or Ctrl-Pause if you want to try Applesoft basic
instead of inserting a disk.

### Keyboard

Use the keyboard to type text in. The special keys are :

On your machine           | On the emulated Apple
--------------------------|----------------------------------
Ctrl-Break or Ctrl-Pause  | Ctrl-Reset
Left Alt                  | Open Apple (joystick button 0)
Right Alt (or AltGr)      | Closed Apple (joystick button 1)


### Floppy disks

Vinace accept any file but works only with .dsk and .do files (DOS
ordered disk images). To insert a disk, just drag the file onto the disk
drive 1. The lid should automatically close. Then restart with a
Alt-Ctrl-Break.

Disk drives are emulated with the same original speed which is quite...
slow.

You can look if something is happening on the terminal output (look
after drive's head position changing, which means that the program
is still alive).
