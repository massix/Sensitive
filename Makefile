# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include Qwt_qt_config.makefile

LDFLAGS		=  $(OS_LDFLAGS)

INCLUDE		= -I.
LIBS		= -LRenal -LGraphics -LSensitiveProtocol -lgraphics -lrenal -lsnsprotocol

ifeq ($(OS),Windows_NT)
	RESOURCES	= Sensitive.res
endif

TARGET		= Sensitive$(EXE_EXT)
OBJECTS		= Sensitive.o


# Unix stuff
DESTDIR		= /usr/local/sensitive

BINDIR		= $(DESTDIR)/bin
LIBDIR		= $(DESTDIR)/lib
INCLUDEDIR	= $(DESTDIR)/include

GLOBALBIN	= /usr/bin

SCRIPTFILES	= sensitive.sh inversitive.sh
DESKTOP		= sensitive.desktop inversitive.desktop

ICONDIR		= /usr/share/icons/hicolor/scalable/apps
ICON_SENS	= Graphics/images/Icon.svg
ICON_INVE	= Inversitive/images/Icon.svg

# Targets
LIBRENAL		= librenal.$(LIB_EXT)
LIBSNSPROTOCOL	= libsnsprotocol.$(LIB_EXT)
LIBGRAPHICS		= libgraphics.$(LIB_EXT)
INVERSITIVE		= Inversitive_UI$(EXE_EXT)

.PHONY: all clean $(INVERSITIVE) $(LIBRENAL) $(LIBSNSPROTOCOL) $(LIBGRAPHICS)


all: $(INVERSITIVE) $(TARGET)

$(LIBRENAL):
	cd Renal && $(MAKE)

$(LIBSNSPROTOCOL):
	cd SensitiveProtocol && $(MAKE)
	
$(LIBGRAPHICS): $(LIBRENAL) $(LIBSNSPROTOCOL) 
	cd Graphics && $(MAKE)

$(INVERSITIVE): $(LIBSNSPROTOCOL)
	cd Inversitive && $(MAKE)

Sensitive.res: Resources.rc
	$(MINGW_WRES) -O coff -o $(RESOURCES) Resources.rc

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) $(QWT_CFLAGS) -c $< -o $@
	
$(TARGET): $(OBJECTS) $(LIBRENAL) $(LIBSNSPROTOCOL) $(LIBGRAPHICS) $(RESOURCES)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(QT_LDFLAGS) $(QWT_LDFLAGS) $(LIBS)
	
win32installer: $(TARGET) $(INVERSITIVE) Sensitive.nsi
	$(NSIS_MAKE) Sensitive.nsi

%.desktop: %_in.desktop
	@echo "Generating $@"
	@sed "s:TRYXEC_$(subst _in.desktop,,$<)_CHANGE_ME:$(GLOBALBIN)/$(subst _in.desktop,,$<):g" $< > $@
	@sed -i "s:EXEC_$(subst _in.desktop,,$<)_CHANGE_ME:$(GLOBALBIN)/$(subst _in.desktop,,$<):g" $@
	@sed -i "s:ICON_$(subst _in.desktop,,$<)_CHANGE_ME:$(ICONDIR)/$(subst _in.desktop,.svg,$<):g" $@

%.sh: %_in.sh
	@echo "Generating $@"
	@sed "s:QWT_CHANGE_ME:$(QWT_PATH)/lib:g" $< > $@
	@sed -i "s:SNS_CHANGE_ME:$(DESTDIR):g" $@

install: $(SCRIPTFILES) $(DESKTOP)
	mkdir -p $(DESTDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(INCLUDEDIR)
	mkdir -p $(INCLUDEDIR)/Renal
	install -m 0777 $(TARGET) $(BINDIR)
	install -m 0777 $(INVERSITIVE) $(BINDIR)
	install -m 0555 $(LIBRENAL) $(LIBSNSPROTOCOL) $(LIBGRAPHICS) $(LIBDIR)
	install -m 0444 Renal/*.h $(INCLUDEDIR)/Renal
	install -m 0777 sensitive.sh $(GLOBALBIN)/sensitive
	install -m 0777 inversitive.sh $(GLOBALBIN)/inversitive
	install -m 0644 $(DESKTOP) /usr/share/applications
	install -m 0644 $(ICON_SENS) $(ICONDIR)/sensitive.svg
	install -m 0644 $(ICON_INVE) $(ICONDIR)/inversitive.svg

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(BINDIR)/$(INVERSITIVE)
	rm -f /usr/share/applications/sensitive.desktop
	rm -f /usr/share/applications/inversitive.desktop
	rm -f $(ICONDIR)/sensitive.svg
	rm -f $(ICONDIR)/inversitive.svg
	rm -f $(LIBDIR)/$(LIBRENAL) $(LIBDIR)/$(LIBSNSPROTOCOL) $(LIBDIR)/$(LIBGRAPHICS)
	rm -fr $(INCLUDEDIR)/Renal
	rm -f $(GLOBALBIN)/sensitive
	rm -f $(GLOBALBIN)/inversitive


clean:
	rm -fr $(TARGET) $(OBJECTS) $(LIBRENAL) $(LIBGRAPHICS) $(LIBSNSPROTOCOL) $(INVERSITIVE) $(RESOURCES) $(SCRIPTFILES) $(DESKTOP)
	cd Renal && $(MAKE) clean
	cd Graphics && $(MAKE) clean
	cd SensitiveProtocol && $(MAKE) clean
	cd Inversitive && $(MAKE) clean
