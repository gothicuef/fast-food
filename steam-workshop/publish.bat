@echo off
setlocal
set "HERE=%~dp0"
call "%HERE%GothicSteamWorkshopUploader.exe" publish --game=g1 --title="Example mod" --tags="Addon,Visual,Fun" --content="%HERE%\\Content" --preview="%HERE%\\preview.jpg"