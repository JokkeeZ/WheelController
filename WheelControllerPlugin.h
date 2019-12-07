#ifndef __TESTPLUGIN_H_INCLUDED
#define __TESTPLUGIN_H_INCLUDED

// You need to define this before the includes, or use /D __SUPPORT_PLUGIN__=1 
// in the C/C++ / Commandline project configuration
#define __SUPPORT_PLUGIN__ 1

#include "IPlugin.h"
#include "IRBRGame.h"
#include "PluginHelper.h"

#include "Gendef.h"
#include <stdio.h>
#include <string>
#include <vector>

class WheelControllerPlugin : public IPlugin
{
public:
	WheelControllerPlugin(IRBRGame* pGame) : m_pGame(pGame)
	{
		const char* directoryPath = "./Plugins/WheelController/";
		if (directoryExists(directoryPath) != 0)
		{
			startWheelController();
		}
	}

	virtual ~WheelControllerPlugin() {}

	virtual const char* GetName()
	{
		return "WheelController";
	}


	virtual void DrawFrontEndPage()
	{
		// Draw blackout (coordinates specify the 'window' where you don't want black)
		m_pGame->DrawBlackOut(300.0f, 0.0f, 300.0f, 480.0f);

		// Draw heading
		m_pGame->SetMenuColor(IRBRGame::MENU_HEADING);
		m_pGame->SetFont(IRBRGame::FONT_BIG);
		m_pGame->WriteText(73.0f, 49.0f, "Wheel Controller");

		std::vector<std::string> items;
		items.push_back("Plugin version: 0.0.1");
		items.push_back("Plugin author: JokkeeZ");
		items.push_back("");
		items.push_back("Open-source libaries used:");
		items.push_back("SharpDX.DirectInput:");
		items.push_back("https://github.com/sharpdx/SharpDX/");

		m_pGame->SetMenuColor(IRBRGame::MENU_TEXT);
		m_pGame->SetFont(IRBRGame::FONT_SMALL);

		for (int i = 0; i < items.size(); ++i)
		{
			if (i == 3)
			{
				m_pGame->SetMenuColor(IRBRGame::MENU_HEADING);
				m_pGame->SetFont(IRBRGame::FONT_BIG);
				m_pGame->WriteText(73.0f, 70.0f + (static_cast<float>(i) * 21.0f), items[i].c_str());

				m_pGame->SetMenuColor(IRBRGame::MENU_TEXT);
				m_pGame->SetFont(IRBRGame::FONT_SMALL);
				continue;
			}

			if (i == 5)
			{
				m_pGame->SetMenuColor(IRBRGame::MENU_SELECTION);
				m_pGame->WriteText(73.0f, 70.0f + (static_cast<float>(i) * 21.0f), items[i].c_str());

				m_pGame->SetMenuColor(IRBRGame::MENU_TEXT);
				continue;
			}

			m_pGame->WriteText(73.0f, 70.0f + (static_cast<float>(i) * 21.0f), items[i].c_str());
		}
	}

	virtual void HandleFrontEndEvents(char txtKeyboard, bool bUp, bool bDown, bool bLeft, bool bRight, bool bSelect){}
	virtual void DrawResultsUI() {}
	virtual void TickFrontEndPage(float fTimeDelta) {}
	virtual void StageStarted(int iMap, const char* ptxtPlayerName, bool bWasFalseStart) {}
	virtual void HandleResults(float fCheckPoint1, float fCheckPoint2, float fFinishTime, const char* ptxtPlayerName){}
	virtual void CheckPoint(float fCheckPointTime, int iCheckPointID, const char* ptxtPlayerName) {}

private:
	IRBRGame* m_pGame;
};
#endif
