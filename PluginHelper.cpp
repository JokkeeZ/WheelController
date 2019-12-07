#include "PluginHelper.h"

int directoryExists(const char* path)
{
	struct stat info;

	if (stat(path, &info) != 0)
	{
		return 0;
	}
	else if (info.st_mode & S_IFDIR)
	{
		return 1;
	}

	return 0;
};

void startWheelController()
{
	system("start /B ./Plugins/WheelController/RBRWC.exe");
};