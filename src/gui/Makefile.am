
SUBDIRS = \
	resources

bin_PROGRAMS = \
	vinace

vinace_SOURCES = \
    ../hardware/libapplehardware.a \
    ../core/libapplecore.a \
	c-gui-color-monitor.cpp \
	c-gui-color-monitor.hpp \
	c-gui-green-monitor.cpp \
	c-gui-green-monitor.hpp \
	c-gui-monitor.cpp \
	c-gui-monitor.hpp \
	c-gui-keyboard.hpp \
	c-gui-keyboard.cpp \
	c-gui-numpad-joystick.hpp \
	c-gui-numpad-joystick.cpp \
	c-gui-disk-drive.hpp \
	c-gui-disk-drive.cpp \
	c-sound.hpp \
	c-sound.cpp \
	c-pulse-audio-sound.hpp \
	c-pulse-audio-sound.cpp \
	main.cc

vinace_LDADD = \
	$(GUI_LIBS)\
	$(PULSE_LIBS)

vinace_LDFLAGS = \
	-L../core \
	-L../hardware \
	-lapplehardware \
	-lapplecore \
	-lrt	

AM_CPPFLAGS = \
	$(GUI_CFLAGS)\
	$(PULSE_CFLAGS)

## File created by the gnome-build tools

