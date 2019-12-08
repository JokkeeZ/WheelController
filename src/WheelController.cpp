#include "stdafx.h"
#include "WheelControllerPlugin.h"

class IRBRGame;

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	return TRUE;
}

IPlugin* RBR_CreatePlugin(IRBRGame* pGame)
{
	return new WheelControllerPlugin(pGame);
}
