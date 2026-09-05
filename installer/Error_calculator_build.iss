[Setup]
AppName=Error Calculator
AppVersion=0.1
WizardStyle=modern dynamic
DefaultDirName={autopf}\Error_calculator
DefaultGroupName=Error Calculator
; Додає чекбокс "Не створювати папку в меню «Пуск»":
AllowNoIcons=yes
UninstallDisplayIcon={app}\bin\Error_calculator.exe
Compression=lzma2
SolidCompression=yes
OutputDir=E:\MyProjects\Error_calculator\Error_calculator\releases
OutputBaseFilename=ErrorCalculator_Setup_v0.1
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
; Галочка ВИМКНЕНА за замовчуванням:
Name: "vcredist"; Description: "Встановити Microsoft Visual C++ v14 Redistributable (x64)"; GroupDescription: "Необхідний софт:"; Flags: unchecked

[Files]
Source: "E:\MyProjects\Error_calculator\Error_calculator\dist\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\Error Calculator"; Filename: "{app}\bin\Error_calculator.exe"
Name: "{autodesktop}\Error Calculator"; Filename: "{app}\bin\Error_calculator.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\vc_redist.x64.exe"; Parameters: "/q /norestart"; Flags: waituntilterminated skipifdoesntexist; Tasks: vcredist; Description: "Встановлення системних компонентів Visual C++"
Filename: "{app}\bin\Error_calculator.exe"; Description: "{cm:LaunchProgram,Error Calculator}"; Flags: nowait postinstall skipifsilent