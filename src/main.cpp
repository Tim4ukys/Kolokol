#include <Windows.h>
#include "main.h"

SAMPFUNCS *SF = new SAMPFUNCS();

bool CALLBACK incomingRPC(stRakNetHookParams* params) 
{
	if (params->packetId == RPCEnumeration::RPC_GiveTakeDamage) 
	{
		bool bGiveOrTake;
		unsigned short wPlayerID;
		float damage_amount;
		unsigned long dWeaponID;
		unsigned long dBodypart;

		//params->bitStream.
		params->bitStream->ResetReadPointer(); 
		params->bitStream->Read(bGiveOrTake); 
		params->bitStream->Read(wPlayerID); 
		params->bitStream->Read(damage_amount); 
		params->bitStream->Read(dWeaponID);
		params->bitStream->Read(dBodypart); 
		params->bitStream->ResetReadPointer();

		BitStream bsClass; 
		if (!bGiveOrTake)
			bsClass.Write(17802);
		else 
			bsClass.Write(5201);
		bsClass.Write(0.0f);
		bsClass.Write(0.0f);
		bsClass.Write(0.0f);
		SF->getRakNet()->emulateRecvRPC(RPC_ScrPlaySound, &bsClass);
	};

	return true;
};

void __stdcall mainloop()
{
	static bool initialized = false;
	if (!initialized)
	{
		if (GAME && GAME->GetSystemState() == eSystemState::GS_PLAYING_GAME && SF->getSAMP()->IsInitialized())
		{
			initialized = true;
			SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(0x99, 0, 0), "[{FF9900}Kolokol{990000}] {FF9900}SF Plugin инициализирован{990000}!");
			SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(0x99, 0, 0), "Автор: {FF9900}Tim4ukys {606060}| {FF9900}vk.com/petrov_team");
			SF->getRakNet()->registerRakNetCallback(RakNetScriptHookType::RAKHOOK_TYPE_OUTCOMING_RPC, incomingRPC);
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	if (dwReasonForCall == DLL_PROCESS_ATTACH)
		SF->initPlugin(mainloop, hModule);
	return TRUE;
}
