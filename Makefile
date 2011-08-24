# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include Qwt_qt_config.makefile

LDFLAGS		=  $(OS_LDFLAGS)

INCLUDE		= -I. $(BS_INCLUDE)
LIBS		= -LRenal -LGraphics -LSensitiveProtocol -lgraphics -lrenal -lsnsprotocol $(SN_LIBS)

ifeq ($(OS),Windows_NT)
	RESOURCES	= Sensitive.res
endif

UTILS		= Config.h

OBJECTS		= Sensitive.o


# Windows stuff
NSIFILE		= Sensitive.nsi


# Unix stuff
DESTDIR		= /usr/local/sensitive

BINDIR		= $(DESTDIR)/bin
LIBDIR		= $(DESTDIR)/lib
INCLUDEDIR	= $(DESTDIR)/include
DESKTOPDIR	= $(DESTDIR)/share/applications
ICONDIR		= $(DESTDIR)/share/icons/hicolor/scalable/apps

SCRIPTFILES	= sensitive.sh inversitive.sh
DESKTOP		= sensitive.desktop inversitive.desktop

ICON_SENS	= Graphics/images/Icon.svg
ICON_INVE	= Inversitive/images/Icon.svg


# Targets
LIBRENAL		= librenal.$(LIB_EXT)
LIBSNSPROTOCOL	= libsnsprotocol.$(LIB_EXT)
LIBGRAPHICS		= libgraphics.$(LIB_EXT)
INVERSITIVE		= Inversitive_UI$(EXE_EXT)
SENSITIVE		= Sensitive$(EXE_EXT)


.PHONY: all clean $(UTILS) $(INVERSITIVE) $(LIBRENAL) $(LIBSNSPROTOCOL) $(LIBGRAPHICS) regen


all: $(UTILS) $(INVERSITIVE) $(SENSITIVE)

$(LIBRENAL):
	cd Renal && $(MAKE)

$(LIBSNSPROTOCOL):
	cd SensitiveProtocol && $(MAKE)
	
$(LIBGRAPHICS): $(LIBRENAL) $(LIBSNSPROTOCOL) 
	cd Graphics && $(MAKE)

$(INVERSITIVE): $(UTILS) $(LIBSNSPROTOCOL)
	cd Inversitive && $(MAKE)

Sensitive.res: Resources.rc
	$(MINGW_WRES) -O coff -o $(RESOURCES) Resources.rc

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) $(QWT_CFLAGS) -c $< -o $@
	
$(SENSITIVE): $(UTILS) $(OBJECTS) $(LIBRENAL) $(LIBSNSPROTOCOL) $(LIBGRAPHICS) $(RESOURCES)
	$(CC) $(LDFLAGS) -o $(SENSITIVE) $(OBJECTS) $(RESOURCES) $(QT_LDFLAGS) $(QWT_LDFLAGS) $(LIBS)
	
$(NSIFILE): $(NSIFILE:.nsi=_in.nsi)
	@echo "Generating $@"
	@sed "s:VERSION_CHANGE_ME:$(VERSION):g" $< > $@

win32installer: $(SENSITIVE) $(INVERSITIVE) $(NSIFILE)
	$(NSIS_MAKE) $(NSIFILE)

$(UTILS): $(UTILS:.h=_in.h)
	@echo "Generating $@"
	@sed "s:VERSION_CHANGE_ME:$(VERSION):g" $< > $@

%.desktop: %_in.desktop
	@echo "Generating $@"
	@sed "s:TRYXEC_$(subst _in.desktop,,$<)_CHANGE_ME:$(BINDIR)/$(subst _in.desktop,,$<):g" $< > $@
	@sed -i "s:EXEC_$(subst _in.desktop,,$<)_CHANGE_ME:$(BINDIR)/$(subst _in.desktop,,$<):g" $@
	@sed -i "s:ICON_$(subst _in.desktop,,$<)_CHANGE_ME:$(ICONDIR)/$(subst _in.desktop,.svg,$<):g" $@

%.sh: %_in.sh
	@echo "Generating $@"
	@sed "s:QWT_CHANGE_ME:$(QWT_PATH)/lib:g" $< > $@
	@sed -i "s:SNS_CHANGE_ME:$(DESTDIR):g" $@
	@sed -i "s:SN_CHANGE_ME:$(SN_DIR)/lib:g" $@

regen:
	rm -fr $(SCRIPTFILES) $(DESKTOP)

install: all regen $(SCRIPTFILES) $(DESKTOP)
	mkdir -p $(DESTDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(INCLUDEDIR)
	mkdir -p $(INCLUDEDIR)/Renal
	mkdir -p $(DESKTOPDIR)
	mkdir -p $(ICONDIR)
	install -m 0777 $(SENSITIVE) $(BINDIR)
	install -m 0777 $(INVERSITIVE) $(BINDIR)
	install -m 0555 $(LIBRENAL) $(LIBSNSPROTOCOL) $(LIBGRAPHICS) $(LIBDIR)
	install -m 0444 Renal/*.h $(INCLUDEDIR)/Renal
	install -m 0777 sensitive.sh $(BINDIR)/sensitive
	install -m 0777 inversitive.sh $(BINDIR)/inversitive
	install -m 0644 $(DESKTOP) $(DESKTOPDIR)
	install -m 0644 $(ICON_SENS) $(ICONDIR)/sensitive.svg
	install -m 0644 $(ICON_INVE) $(ICONDIR)/inversitive.svg

release: clean
	mkdir -p Sensitive-$(VERSION)
	cp -r Graphics Sensitive-$(VERSION)
	cp -r Inversitive Sensitive-$(VERSION)
	cp -r SensitiveProtocol Sensitive-$(VERSION)
	cp -r Renal Sensitive-$(VERSION)
	cp Makefile Sensitive-$(VERSION)
	cp Sensitive.cpp Sensitive-$(VERSION)
	cp *_in.* Sensitive-$(VERSION)
	cp AUTHORS LICENSE README Sensitive-$(VERSION)
	cp Resources.rc Sensitive-$(VERSION)
	cp Qwt_qt_config.makefile Sensitive-$(VERSION)
	cp sensitive.ico Sensitive-$(VERSION)
	tar czf Sensitive-$(VERSION).tgz Sensitive-$(VERSION)
	rm -fr Sensitive-$(VERSION)

uninstall:
	rm -f $(BINDIR)/$(SENSITIVE)
	rm -f $(BINDIR)/$(INVERSITIVE)
	rm -f $(DESKTOPDIR)/sensitive.desktop
	rm -f $(DESKTOPDIR)/inversitive.desktop
	rm -f $(ICONDIR)/sensitive.svg
	rm -f $(ICONDIR)/inversitive.svg
	rm -f $(LIBDIR)/$(LIBRENAL) $(LIBDIR)/$(LIBSNSPROTOCOL) $(LIBDIR)/$(LIBGRAPHICS)
	rm -fr $(INCLUDEDIR)/Renal
	rm -f $(BINDIR)/sensitive
	rm -f $(BINDIR)/inversitive

clean:
	rm -fr $(SENSITIVE) $(OBJECTS) $(LIBRENAL) $(LIBGRAPHICS) $(LIBSNSPROTOCOL) $(INVERSITIVE) 
	rm -fr $(RESOURCES) $(SCRIPTFILES) $(DESKTOP) $(UTILS) $(NSIFILE)
	cd Renal && $(MAKE) clean
	cd Graphics && $(MAKE) clean
	cd SensitiveProtocol && $(MAKE) clean
	cd Inversitive && $(MAKE) clean
