#include "WheelControllerPlugin.h"

class IRBRGame;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
};

IPlugin* RBR_CreatePlugin(IRBRGame* pGame)
{
	return new WheelControllerPlugin(pGame);
};
