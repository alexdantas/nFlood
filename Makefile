# nflood Makefile
# (2014) Alexandre Dantas <eu@alexdantas.net>
#
# This is a rather complex Makefile, sorry about that.
# It supports the following targets:
#
# make all        Builds the package
# make run        Builds and runs the program
# make install    Installs the package on your system
# make uninstall  Uninstalls the package from your system
# make clean      Cleans results of building process
# make dist       Creates source code "tarball"
# make doc        Generates the documentation with doxygen
# make docclean   Removes the documentation
#
# Also, the following commandline arguments customize
# default actions:
#
#  V       Verbose mode, off by default.
#          To turn on for the current command,
#          add `V=1` when calling `make`.
#          To turn on permanently, uncomment the line
#          specified below
#  DESTDIR Installs the package on a custom root directory
#          (other than `/`). For example `DESTDIR=~/`.
#  PREFIX  Installs the package on a custom directory
#          (overwrites root)
#  CFLAGS  Changes the C flags used on compilation
#  CDEBUG  If you wish to build on debug mode, add 'CDEBUG=-g'
#

# Uncomment line below to tun on verbose mode permanently
#V = 1;

# General Info
PACKAGE = nflood
VERSION = 0.5.0
DATE    = $(shell date "+%b%Y")

# Local source code information
FILES = COPYING Makefile README.md TODO

# Install dirs
DESTDIR =
PREFIX  = $(DESTDIR)/usr

EXEC_PREFIX = $(PREFIX)
DATAROOTDIR = $(PREFIX)/share
MANROOT     = $(DATAROOTDIR)/man

# 6 for games
MANNUMBER = 6

BINDIR = $(EXEC_PREFIX)/games
MANDIR = $(MANROOT)/man$(MANNUMBER)

MANFILE = $(PACKAGE).$(MANNUMBER)
MANPAGE = man/$(MANFILE)

CONFIG_DIR = ~/.local/share/$(PACKAGE)

# Build info
EXE         = $(PACKAGE)
CDEBUG      = -O2
CFLAGS      = $(CDEBUG) -Wall -Wextra
LDFLAGS     = -lncurses
INCLUDESDIR =
LIBSDIR     =

# Dependency
DEPEND = .depend

# All source files
CFILES  = $(shell find src -type f -name '*.c')
OBJECTS = $(CFILES:.c=.o)

DEFINES = -DVERSION=\""$(VERSION)"\" \
          -DPACKAGE=\""$(PACKAGE)"\" \
          -DDATE=\""$(DATE)"\"

# Distribution tarball
TARNAME = $(PACKAGE)
DISTDIR = $(TARNAME)-$(VERSION)

# Verbose mode check
ifdef V
MUTE =
VTAG = -v
else
MUTE = @
endif

# Make targets
all: $(EXE) .depend
	# Build successful!

depend: $(DEPEND)
$(DEPEND): $(CFILES)
	# Making .depend...
	$(MUTE)rm -f $(DEPEND)
	$(MUTE)for src in $^; do \
		$(CC) $(CFLAGS) -MM -MT $${src%.c}.o $$src >> $(DEPEND); \
	done
-include $(DEPEND)

$(EXE): $(OBJECTS)
	# Linking...
	$(MUTE)$(CC) $(OBJECTS) -o bin/$(EXE) $(LIBSDIR) $(LDFLAGS)

src/%.o: src/%.c
	# Compiling $<...
	$(MUTE)$(CC) $(CFLAGS) $(CDEBUG) $< -c -o $@ $(DEFINES) $(INCLUDESDIR)

install: all
	# Installing...
	$(MUTE)install -d -m 755 $(BINDIR)
	$(MUTE)install -m 755 bin/$(EXE) $(BINDIR)

	-$(MUTE)cat $(MANPAGE) | sed -e "s|DATE|$(DATE)|g" -e "s|VERSION|$(VERSION)|g" >$(MANFILE)

	$(MUTE)install -d $(MANDIR)
	$(MUTE)install $(MANFILE) $(MANDIR)
	$(MUTE)rm -f $(MANFILE)
	# $(PACKAGE) successfuly installed!

uninstall:
	# Uninstalling...
	$(MUTE)rm -f $(BINDIR)/$(EXE)
	$(MUTE)rm -f $(MANDIR)/$(MANFILE)

purge: uninstall
	# Purging configuration files...
	$(MUTE)rm -rf $(CONFIG_DIR)

dist: clean $(DISTDIR).tar.gz

$(DISTDIR).tar.gz: $(DISTDIR)
	$(MUTE)tar czf $(DISTDIR).tar.gz $(DISTDIR)
	$(MUTE)rm -rf $(DISTDIR)
	$(MUTE)cp $(DISTDIR).tar.gz ..
	$(MUTE)rm -f $(DISTDIR).tar.gz

$(DISTDIR):
	$(MUTE)mkdir -p $(DISTDIR)/src $(DISTDIR)/doc $(DISTDIR)/bin
	-$(MUTE)cp $(FILES) -t $(DISTDIR)
	-$(MUTE)cp -r src/* $(DISTDIR)/src
	-$(MUTE)cp -r doc/* $(DISTDIR)/doc

run: all
	# Running...
	$(MUTE)./bin/$(EXE)

clean:
	# Cleaning files...
	$(MUTE)rm $(VTAG) -f $(DEPEND)
	$(MUTE)rm $(VTAG) -f $(OBJECTS)
	$(MUTE)rm $(VTAG) -f bin/$(EXE)

doc:
	# Generating documentation...
	$(MUTE)doxygen Doxyfile

docclean:
	# Removing documentation...
	-$(MUTE)rm $(VTAG) -rf doc/html

.PHONY: clean doc docclean uninstall purge

