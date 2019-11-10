!include nsDialogs.nsh

Outfile LiC-installer.exe
Name "L in Cubeland"
RequestExecutionLevel admin
AllowRootDirInstall true
XPStyle on
WindowIcon off
Icon LiC.ico
UninstallIcon LiC.ico
ShowInstDetails show
ShowUninstDetails show
Caption "$(^Name) - Installer"
UninstallCaption "$(^Name) - Uninstaller"
SubCaption 1 " "
SubCaption 2 " "
SubCaption 3 " "
SubCaption 4 " "
UninstallSubCaption 1 " "
UninstallSubCaption 2 " "
DirText "Select the folder where $\"$(^Name)$\" will be installed." "Installation Folder" Browse "Select the folder where $\"$(^Name)$\" will be installed."
ComponentText "Select the additional items that will be installed." "" "To select an item mark its box."
BrandingText " "
SpaceTexts none
MiscButtonText Previous Next Cancel Exit
SetOverwrite try

Page directory
Page components "" CompPageShow
Page custom ExtPageCreate
Page instfiles
UninstPage instfiles

Var start_sc
Var desktop_sc
Var ext_assoc
Var installer
Var ext
Var button_hwnd

Section
    CreateDirectory $INSTDIR\LiC
    AccessControl::GrantOnFile $INSTDIR\LiC (BU) FullAccess
    SetOutPath $INSTDIR\LiC\information
    File ..\information\LiC.pdf
    SetOutPath $INSTDIR\LiC\executable
    File ..\executable\LiC.exe
    File ..\executable\freeglut.dll
    File ..\executable\SDL2.dll
    File ..\executable\SDL2_mixer.dll
    File ..\executable\libmpg123-0.dll
    File ..\executable\libogg-0.dll
    File ..\executable\libvorbis-0.dll
    File ..\executable\libvorbisfile-3.dll
    SetOutPath $INSTDIR\LiC\source
    File LiC.cpp
    File LiC.h
    File LiC.rc
    File LiC.nsi
    File LiC.ico
    SetOutPath $INSTDIR\LiC\sound
    File ..\sound\0.mp3
    File ..\sound\day-time.mp3
    File ..\sound\night-time.mp3
    File ..\sound\finished.mp3
    File ..\sound\0.ogg
    File ..\sound\creation.ogg
    File ..\sound\stock.ogg
    File ..\sound\lamp.ogg
    File ..\sound\collapse.ogg
    File ..\sound\fall.ogg
    File ..\sound\disaster.ogg
    File ..\sound\simple.ogg
    CreateDirectory $INSTDIR\LiC\storage
SectionEnd

Section /o "start menu shortcuts" start_sc_section
    CreateDirectory $SMPROGRAMS\LiC
    CreateShortcut $SMPROGRAMS\LiC\LiC.lnk $INSTDIR\LiC\executable\LiC.exe
    CreateShortcut $SMPROGRAMS\LiC\information.lnk $INSTDIR\LiC\information\LiC.pdf
    CreateShortcut $SMPROGRAMS\LiC\folder.lnk $INSTDIR\LiC
SectionEnd

Section /o "desktop shortcut" desktop_sc_section
    CreateShortcut $DESKTOP\LiC.lnk $INSTDIR\LiC\executable\LiC.exe
SectionEnd

Section /o "file extension association" ext_assoc_section
    StrCmp $ext "" 0 +2
        StrCpy $ext lic
    WriteRegStr HKCR .$ext "" LiC
    WriteRegStr HKCR LiC\DefaultIcon "" $INSTDIR\LiC\executable\LiC.exe
    WriteRegStr HKCR LiC\shell\open\command "" "$\"$INSTDIR\LiC\executable\LiC.exe$\" $\"s=%1$\""
SectionEnd

Section /o installer installer_section
    CopyFiles /SILENT $EXEPATH $INSTDIR\LiC\executable
    StrCmpS $EXEFILE LiC-installer.exe +2
        Rename $INSTDIR\LiC\executable\$EXEFILE $INSTDIR\LiC\executable\LiC-installer.exe
SectionEnd

Section /o uninstaller
    WriteUninstaller $INSTDIR\LiC\executable\LiC-uninstaller.exe
    WriteRegStr HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC InstallLocation $INSTDIR\LiC
    WriteRegStr HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC UninstallString $INSTDIR\LiC\executable\LiC-uninstaller.exe
    WriteRegStr HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC DisplayIcon $INSTDIR\LiC\executable\LiC.exe
    WriteRegStr HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC DisplayName $(^Name)
    WriteRegStr HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC DisplayVersion 0.0.9
    WriteRegStr HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC Publisher "poulos poulopoulos"
    WriteRegDWORD HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC NoModify 1
    WriteRegDWORD HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC NoRepair 1
    WriteRegDWORD HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC StartMenuShortcuts $start_sc
    WriteRegDWORD HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC DesktopShortcut $desktop_sc
    StrCmp $ext_assoc 1 +2
        StrCpy $ext ""
    WriteRegStr HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC FileExtension $ext
    WriteRegDWORD HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC Installer $installer
SectionEnd

Section Uninstall
    SetOutPath C:
    ReadRegStr $0 HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC InstallLocation
    Delete $0\information\LiC.pdf
    RMDir $0\information
    Delete $0\executable\LiC.exe
    Delete $0\executable\freeglut.dll
    Delete $0\executable\SDL2.dll
    Delete $0\executable\SDL2_mixer.dll
    Delete $0\executable\libmpg123-0.dll
    Delete $0\executable\libogg-0.dll
    Delete $0\executable\libvorbis-0.dll
    Delete $0\executable\libvorbisfile-3.dll
    Delete $0\executable\LiC-uninstaller.exe
    ReadRegDWORD $1 HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC Installer
    IntCmpU $1 0 +2
        Delete $0\executable\LiC-installer.exe
    RMDir $0\executable
    Delete $0\source\LiC.cpp
    Delete $0\source\LiC.h
    Delete $0\source\LiC.rc
    Delete $0\source\LiC.nsi
    Delete $0\source\LiC.ico
    RMDir $0\source
    Delete $0\sound\0.mp3
    Delete $0\sound\day-time.mp3
    Delete $0\sound\night-time.mp3
    Delete $0\sound\finished.mp3
    Delete $0\sound\0.ogg
    Delete $0\sound\creation.ogg
    Delete $0\sound\stock.ogg
    Delete $0\sound\lamp.ogg
    Delete $0\sound\collapse.ogg
    Delete $0\sound\fall.ogg
    Delete $0\sound\disaster.ogg
    Delete $0\sound\simple.ogg
    RMDir $0\sound
    Delete $0\storage\configuration
    Delete $0\storage\state
    RMDir $0\storage
    RMDir $0
    ReadRegDWORD $1 HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC StartMenuShortcuts
    IntCmpU $1 0 +5
        Delete $SMPROGRAMS\LiC\LiC.lnk
        Delete $SMPROGRAMS\LiC\information.lnk
        Delete $SMPROGRAMS\LiC\folder.lnk
        RMDir $SMPROGRAMS\LiC
    ReadRegDWORD $1 HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC DesktopShortcut
    IntCmpU $1 0 +2
        Delete $DESKTOP\LiC.lnk
    ReadRegStr $1 HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC FileExtension
    StrCmp $1 "" +4
        DeleteRegKey HKCR .$1
        DeleteRegKey HKCR LiC
        DeleteRegKey HKCU SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.$1
    DeleteRegKey HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\LiC
SectionEnd

Function .onInit
    StrCpy $start_sc 0
    StrCpy $desktop_sc 0
    StrCpy $ext_assoc 0
    StrCpy $installer 0
    StrCpy $ext lic
FunctionEnd

Function .onSelChange
    StrCmp $0 ${start_sc_section} 0 +3
        IntOp $start_sc $start_sc !
        Goto end
    StrCmp $0 ${desktop_sc_section} 0 +3
        IntOp $desktop_sc $desktop_sc !
        Goto end
    StrCmp $0 ${ext_assoc_section} 0 +7
        IntOp $ext_assoc $ext_assoc !
        StrCmp $ext_assoc 1 +3
            ${NSD_SetText} $button_hwnd Install
            Goto +2
            ${NSD_SetText} $button_hwnd Next
        Goto end
    StrCmp $0 ${installer_section} 0 +3
        IntOp $installer $installer !
        Goto end
    end:
FunctionEnd

Function CompPageShow
    GetDlgItem $button_hwnd $HWNDPARENT 1
    StrCmp $ext_assoc 1 +2
        ${NSD_SetText} $button_hwnd Install
FunctionEnd

Function ExtPageCreate
    StrCmp $ext_assoc 0 +8
        nsDialogs::Create 1018
        Pop $0
        ${NSD_CreateLabel} 0 0 100% 12u "Select the file extension that will be associated with $\"$(^Name)$\"."
        Pop $0
        ${NSD_CreateText} 1 39u -2u 12u $ext
        Pop $0
        ${NSD_OnChange} $0 ExtTextChange
        nsDialogs::Show
FunctionEnd

Function ExtTextChange
    Pop $0
    ${NSD_GetText} $0 $ext
FunctionEnd
