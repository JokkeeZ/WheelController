mkdir WheelController
mkdir WheelController\Plugins
copy src\Debug\WheelController.dll .\WheelController\Plugins

cd .\WheelController\
"%ProgramFiles%\7-zip\7z" a "..\WheelController.zip" .\Plugins\
cd ..
@RD /S /Q .\WheelController
