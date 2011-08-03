; Sensitive Installer

; Use Modern UI
!include "MUI2.nsh"

; General informations on Sensitive
!define PACKAGE_NAME	"Sensitive"
!define PACKAGE_VERSION	"0.9.4"

Name "${PACKAGE_NAME} ${PACKAGE_VERSION}"
OutFile "${PACKAGE_NAME}-${PACKAGE_VERSION}.exe"


; Since we're gonna write in %%PROGRAM_FILES%% we need to have admin priviledges
RequestExecutionLevel	admin


; Installation Directory
InstallDir "$PROGRAMFILES\${PACKAGE_NAME}"



; Configure Pages
; General headers
!define MUI_PAGE_HEADER_TEXT "Sensitive UI Installer"
!define MUI_PAGE_HEADER_SUBTEXT "This will install Sensitive UI on your computer"
!define MUI_HEADERIMAGE
!define MUI_WELCOMEFINISHPAGE_BITMAP "E:\Workspace\Sensitive\ExternLibs\bitmap.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "E:\Workspace\Sensitive\ExternLibs\bitmap.bmp"

; Welcome page
!define MUI_WELCOMEPAGE_TITLE "Sensitive UI"
!define MUI_WELCOMEPAGE_TITLE_3LINES "This is the installer for Sensitive UI. Read the instructions to install Sensitive UI on your computer"
!define MUI_WELCOMEPAGE_TEXT "Sensitive UI is a small utility for calculating interpolating polynomial given a set of coordinates"

; License page
!define MUI_LICENSEPAGE_TEXT_TOP "Please read and accept the license below"
!define MUI_LICENSEPAGE_TEXT_BOTTOM "Click on Agree if you do agree with the license above"

; Directory page
!define MUI_DIRECTORYPAGE_TEXT_TOP "Please choose where to install Sensitive UI"
!define MUI_DIRECTORYPAGE_TEXT_DESTINATION "Destination directory"

; Installation page
!define MUI_INSTFILESPAGE_FINISHHEADER_TEXT "Sensitive UI Installer"
!define MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT "Installation completed!"
!define MUI_INSTFILESPAGE_ABORTHEADER_TEXT "Sensitive UI Installer"
!define MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT "Installation aborted"

; Finish page
!define MUI_FINISHPAGE_TITLE "Sensitive UI"
!define MUI_FINISHPAGE_TEXT "Sensitive UI is successfully installed on your computer!"
!define MUI_FINISHPAGE_BUTTON "Finish"
!define MUI_FINISHPAGE_RUN $INSTDIR\Sensitive.exe
!define MUI_FINISHPAGE_RUN_TEXT "Run Sensitive UI"
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_LINK "Visit Sensitive's website"
!define MUI_FINISHPAGE_LINK_LOCATION "http://github.com/massix/Sensitive"

; Uninstall page
!define MUI_UNCONFIRMPAGE_TEXT_TOP "Sensitive UI uninstaller"


; Insert Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE LICENSE
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES


; Installation language
!insertmacro MUI_LANGUAGE "English"


; Main section
Section "Install"
	SetOutPath "$INSTDIR"
	File "Sensitive.exe"
	File "libgraphics.dll"
	File "librenal.dll"
	File "ExternLibs\*.dll"
	File "sensitive.ico"
	
	; Menu shortcuts
	CreateDirectory "$SMPROGRAMS\Sensitive"
  	CreateShortcut 	"$SMPROGRAMS\Sensitive\Sensitive.lnk" \
  	               	"$INSTDIR\Sensitive.exe" "" \
                  	"$INSTDIR\Sensitive.ico"
  	CreateShortcut "$SMPROGRAMS\Sensitive\Uninstall Sensitive.lnk" \
                   "$INSTDIR\uninstall.exe"

	WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd


Section "Uninstall"
	RMDir /r "$INSTDIR"
SectionEnd