// This file is included separately for each engine version
#pragma once

#include "Union/HookProto.h"
#include "ZenGin/Gothic_I_Classic/API/oNpc.h"
#include <Union/Hook.h>     // základní hlavičky Union frameworku
#include <ZenGin/Gothic_I_Classic/API/zTypes.h>   // typy jako zSTRING, zCView, atd.
#include <ZenGin/Gothic_I_Classic/API/zView.h>    // konkrétně pro zCView
#include <ZenGin/Gothic_I_Classic/API/zString.h>  // konkrétně pro zSTRING
#include <ZenGin/Gothic_I_Classic/API/oNpc.h>
#include <ZenGin/zGothicAPI.h>  // konkrétně pro zSTRING
#include <ZenGin/Gothic_I_Classic/API/zAiPlayer.h>

#include "ZenGin/Gothic_I_Classic/API/oGame.h"
#include <fstream>
/*
void DebugLog(const std::string& msg) {
	std::ofstream log("FastFoodDebug.log", std::ios::app);
	log << msg << "\n";
}
*/
namespace GOTHIC_NAMESPACE
{
	// NOTE! Callbacks won't be called by default, you need to uncomment
	// hooks that will call specific callback

	zCViewDialogChoice* gActiveChoice = nullptr;
	oCMobInter* gActivePan = nullptr;
	oCNpc* gActiveNpc = nullptr;

	//void __fastcall Hook_oCMobInter_StartInteraction(oCMobInter* self, void* vtable, oCNpc* npc);
	void __fastcall Hook_oCMobInter_StopInteraction(oCMobInter* self, void* vtable, oCNpc* npc);
/*
	auto Hook_oCMobInter_StartInteraction_Original = Union::CreateHook(
		reinterpret_cast<void*>(zSwitch(
			0x0067FCA0,  // G1
			0x006AEDE0,  // G1A
			0x006C34F0,  // G2
			0x00721580   // G2A
		)),
		&Hook_oCMobInter_StartInteraction,
		Union::HookType::Hook_Detours
		);
*/
	auto Hook_oCMobInter_StopInteraction_Original = Union::CreateHook(
		reinterpret_cast<void*>(zSwitch(
			0x00680250,  // G1
			0x006AF4B0,  // G1A
			0x006C3B90,  // G2
			0x00721C20   // G2A
		)),
		&Hook_oCMobInter_StopInteraction,
		Union::HookType::Hook_Detours
	);


	bool IsHeroeCookingOnPan(oCMobInter* object, oCNpc* npc) {
		if (npc->npcType == NPCTYPE_MAIN) {
			if (auto mob = object->GetObjectName(); mob == zSTRING("OC_MOB_PAN")) {
				return true;
			}
		}
		return false;
	}

	int GetRawMeatAmount(oCNpc* npc) {
		oCNpcInventory *inv = &npc->inventory2;
		if (const oCItem *rawMeat = inv->IsIn(3851, 0)) {
			return rawMeat->amount;
		}
		return 0;
	}

	bool HasRawMeatInInventory(oCNpc* npc) {
		return GetRawMeatAmount(npc) > 0;
	}

	void CookMeatOnPan( oCNpc* npc, int amount = 1) {
		oCNpcInventory *inv = &npc->inventory2;

		int count = GetRawMeatAmount(npc);

		if (amount > count || amount < 0) {
			amount = count;
		}

		if (HasRawMeatInInventory(npc)) {
			const oCItem *rawMeat = inv->IsIn(3851, 0);

			if (oCItem *cookedMeat = inv->IsIn(3849, 0)) {
				cookedMeat->amount += amount;
				inv->Remove(rawMeat->instanz, amount);
			} else {
				if (int cookedMeatIndex = parser->GetIndex("ITFOMUTTON")) {
					npc->PutInInv(cookedMeatIndex, amount);
					inv->Remove(rawMeat->instanz, amount);
				}
			}
		}
	}

	void AskMeatCount(oCMobInter* self, oCNpc* npc, int totalRaw) {

		oCInformationManager* infoMan = &oCInformationManager::GetInformationManager();
		infoMan->SetNpc(npc);
		gActiveChoice = infoMan->DlgChoice;
		gActiveChoice->RemoveAllChoices();
		gActivePan = self;
		gActiveNpc = npc;
		//infoMan->SetNpc(npc);

		zSTRING choice1 = "1x";
		zSTRING choice5 = "5x";
		zSTRING choice10 = "10x";
		zSTRING choice20 = "20x";
		zSTRING choiceX = "All";
		zSTRING choice0 = "None";

		gActiveChoice->AddChoice(choice0, 0);
		gActiveChoice->AddChoice(choice1, 1);
		gActiveChoice->AddChoice(choice5, 2);
		gActiveChoice->AddChoice(choice10, 3);
		gActiveChoice->AddChoice(choice20, 4);
		gActiveChoice->AddChoice(choiceX, 5);

		gActiveChoice->StartSelection();
		gActiveChoice->Activate(1);
		gActiveChoice->Render();

/*
		ShowMenu({
			"1x",
			"5x",
			"10x",
			"20x",
			"All",
			"None"
		}, [self, npc, totalRaw](int choice) {
			npc->SetBodyState(BS_STAND);
			switch (choice) {
				case 0:
					CookMeatOnPan(npc, 1);

				break;
				case 1:
					CookMeatOnPan(npc, 5);
				break;
				case 2: CookMeatOnPan(npc, 10); break;
				case 3: CookMeatOnPan(npc, 20); break;
				case 4: CookMeatOnPan(npc, totalRaw); break;
				default:
					self->EndInteraction(npc, 0);
                    npc->ResetToHumanAI();
				break;
			}
		});*/

	}

	void HandleChoicePan() {
		if (gActiveChoice && gActiveNpc && gActivePan) {
			if (gActiveChoice->ChoiceSelected >= 0) {
				gActiveChoice->StopSelection();
				switch (gActiveChoice->ChoiceSelected) {
					case 1:
						CookMeatOnPan(gActiveNpc, 1);

						break;
					case 2:
						CookMeatOnPan(gActiveNpc, 5);
						break;
					case 3: CookMeatOnPan(gActiveNpc, 10); break;
					case 4: CookMeatOnPan(gActiveNpc, 20); break;
					case 5: CookMeatOnPan(gActiveNpc, -1); break;
					default:
						gActivePan->EndInteraction(gActiveNpc, 0);
						gActiveNpc->ResetToHumanAI();
						break;
				}
			}
		}
	}
/*
	void __fastcall Hook_oCMobInter_StartInteraction(oCMobInter* self, void* vtable, oCNpc* npc) {

		if (IsHeroeCookingOnPan(self, npc)) {
			if (HasRawMeatInInventory(npc)) {
				//Hook_oCMobInter_StartInteraction_Original(self, vtable, npc);
				AskMeatCount(self, npc, GetRawMeatAmount(npc));
			}
			//CookMeatOnPan(npc);
		}

		//Hook_oCMobInter_StartInteraction_Original(self, vtable, npc);
	}*/

	void __fastcall Hook_oCMobInter_StopInteraction(oCMobInter* self, void* vtable, oCNpc* npc) {

		if (IsHeroeCookingOnPan(self, npc)) {
			if (HasRawMeatInInventory(npc)) {
				//Hook_oCMobInter_StopInteraction_Original(self, vtable, npc);
				AskMeatCount(self, npc, GetRawMeatAmount(npc));
			}
			//CookMeatOnPan(npc);
		}

		//Hook_oCMobInter_StopInteraction_Original(self, vtable, npc);
	}


	void Game_EntryPoint()
	{

	}

	void Game_Init()
	{
	}

	void Game_Exit()
	{

	}

	inline void Game_PreLoop()
	{

	}

	inline void Game_Loop()
	{
		//HandleMenuInput();
		HandleChoicePan();
	}

	void Game_PostLoop()
	{

	}

	void Game_MenuLoop()
	{

	}

	void Game_SaveBegin()
	{

	}

	void Game_SaveEnd()
	{

	}

	void LoadBegin()
	{

	}

	void LoadEnd()
	{

	}

	void Game_LoadBegin_NewGame()
	{
		LoadBegin();
	}

	void Game_LoadEnd_NewGame()
	{
		LoadEnd();
	}

	void Game_LoadBegin_SaveGame()
	{
		LoadBegin();
	}

	void Game_LoadEnd_SaveGame()
	{
		LoadEnd();
	}

	void Game_LoadBegin_ChangeLevel()
	{
		LoadBegin();
	}

	void Game_LoadEnd_ChangeLevel()
	{
		LoadEnd();
	}

	void Game_LoadBegin_TriggerChangeLevel()
	{

	}

	void Game_LoadEnd_TriggerChangeLevel()
	{

	}

	void Game_Pause()
	{

	}

	void Game_Unpause()
	{

	}

	void Game_DefineExternals()
	{

	}

	void Game_ApplySettings()
	{

	}

	// Names
	//void __fastcall oCNpc_InitByScript(Gothic_I_Classic::oCNpc* self, void* vtable, int instance, int savegame );

	//auto Hook_CNpc_InitByScript = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0068C840, 0x006BCFB0, 0x006D0C10, 0x0072EE70)), &oCNpc_InitByScript, Union::HookType::Hook_Detours);

	/*
	 * g1c 0x0068C840
	 * g1a 0x006BCFB0
	 * g2c 0x006D0C10
	 * g2a 0x0072EE70
	 */
	/*void __fastcall oCNpc_InitByScript(Gothic_I_Classic::oCNpc* self, void* vtable, int instance, int savegame ) {
		Hook_CNpc_InitByScript( self, vtable, instance, savegame );
		self->name[0] = self->name[0] + " " + Gothic_I_Classic::zSTRING( instance );
	}*/
/*
	void __fastcall oCGame_Init(oCGame* self, void* vtable);
	auto Hook_oCGame_Init = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x00636F50, 0x0065D480, 0x006646D0, 0x006C1060)), &oCGame_Init, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Init(oCGame* self, void* vtable)
	{
		Hook_oCGame_Init(self, vtable);
		Game_Init();
	}*/

	/*
	int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	auto Hook_WinMain = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x004F3E10, 0x00506810, 0x005000F0, 0x00502D70)), &WinMain, Union::HookType::Hook_Detours);
	int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
	{
		Game_EntryPoint();
		return Hook_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	}
*/

	/*void __fastcall CGameManager_Done(CGameManager* self, void* vtable);
	auto Hook_CGameManager_Done = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x00424850, 0x00427310, 0x004251A0, 0x004254E0)), &CGameManager_Done, Union::HookType::Hook_Detours);
	void __fastcall CGameManager_Done(CGameManager* self, void* vtable)
	{
		Game_Exit();
		Hook_CGameManager_Done(self, vtable);
	}*/

	/*void __fastcall oCGame_Render(oCGame* self, void* vtable);
	auto Hook_oCGame_Render = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063DBE0, 0x006648F0, 0x0066B930, 0x006C86A0)), &oCGame_Render, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Render(oCGame* self, void* vtable)
	{
		Game_PreLoop();
		Hook_oCGame_Render(self, vtable);
		Game_PostLoop();
	}*/


	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg);
	auto Partial_zCWorld_Render = Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0063DC76, 0x0066498B, 0x0066BA76, 0x006C87EB)), &oCGame_MainWorld_Render);
	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg)
	{
		Game_Loop();
	}

	/*void __fastcall zCMenu_Render(zCMenu* self, void* vtable);
	auto Hook_zCMenu_Render = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x004D0DA0, 0x004E14E0, 0x004DB270, 0x004DDC20)), &zCMenu_Render, Union::HookType::Hook_Detours);
	void __fastcall zCMenu_Render(zCMenu* self, void* vtable)
	{
		Hook_zCMenu_Render(self, vtable);
		Game_MenuLoop();
	}*/

	/*void __fastcall oCGame_WriteSaveGame(oCGame* self, void* vtable, int slot, zBOOL saveGlobals);
	auto Hook_oCGame_WriteSaveGame = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063AD80, 0x00661680, 0x006685D0, 0x006C5250)), &oCGame_WriteSaveGame, Union::HookType::Hook_Detours);
	void __fastcall oCGame_WriteSaveGame(oCGame* self, void* vtable, int slot, zBOOL saveGlobals)
	{
		Game_SaveBegin();
		Hook_oCGame_WriteSaveGame(self, vtable, slot, saveGlobals);
		Game_SaveEnd();
	}*/

	/*void __fastcall oCGame_LoadGame(oCGame* self, void* vtable, int slot, const zSTRING& levelPath);
	auto Hook_oCGame_LoadGame = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063C070, 0x00662B20, 0x00669970, 0x006C65A0)), &oCGame_LoadGame, Union::HookType::Hook_Detours);
	void __fastcall oCGame_LoadGame(oCGame* self, void* vtable, int slot, const zSTRING& levelPath)
	{
		Game_LoadBegin_NewGame();
		Hook_oCGame_LoadGame(self, vtable, slot, levelPath);
		Game_LoadEnd_NewGame();
	}*/

	/*void __fastcall oCGame_LoadSaveGame(oCGame* self, void* vtable, int slot, zBOOL loadGlobals);
	auto Hook_oCGame_LoadSaveGame = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063C2A0, 0x00662D60, 0x00669BA0, 0x006C67D0)), &oCGame_LoadSaveGame, Union::HookType::Hook_Detours);
	void __fastcall oCGame_LoadSaveGame(oCGame* self, void* vtable, int slot, zBOOL loadGlobals)
	{
		Game_LoadBegin_SaveGame();
		Hook_oCGame_LoadSaveGame(self, vtable, slot, loadGlobals);
		Game_LoadEnd_SaveGame();
	}*/

	/*void __fastcall oCGame_ChangeLevel(oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint);
	auto Hook_Game_Load_ChangeLevel = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063CD60, 0x00663950, 0x0066A660, 0x006C7290)), &oCGame_ChangeLevel, Union::HookType::Hook_Detours);
	void __fastcall oCGame_ChangeLevel(oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint)
	{
		Game_LoadBegin_ChangeLevel();
		Hook_Game_Load_ChangeLevel(self, vtable, levelpath, startpoint);
		Game_LoadEnd_ChangeLevel();
	}*/

	/*void __fastcall oCGame_TriggerChangeLevel(oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint);
	auto Hook_oCGame_TriggerChangeLevel = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063D480, 0x00664100, 0x0066AD80, 0x006C7AF0)), &oCGame_TriggerChangeLevel, Union::HookType::Hook_Detours);
	void __fastcall oCGame_TriggerChangeLevel(oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint)
	{
		Game_LoadBegin_TriggerChangeLevel();
		Hook_oCGame_TriggerChangeLevel(self, vtable, levelpath, startpoint);
		Game_LoadEnd_TriggerChangeLevel();
	}*/

/*#if ENGINE <= Engine_G1A
	void __fastcall oCGame_Pause_G1(oCGame* self, void* vtable);
	auto Hook_oCGame_Pause = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063DF50, 0x00664CD0, 0, 0)), &oCGame_Pause_G1, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Pause_G1(oCGame* self, void* vtable)
	{
		Hook_oCGame_Pause(self, vtable);
		Game_Pause();
	}
#else
	void __fastcall oCGame_Pause_G2(oCGame* self, void* vtable, zBOOL sessionPaused);
	auto Hook_oCGame_Pause = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0, 0, 0x0066BD50, 0x006C8AD0)), &oCGame_Pause_G2, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Pause_G2(oCGame* self, void* vtable, zBOOL sessionPaused)
	{
		Hook_oCGame_Pause(self, vtable, sessionPaused);
		Game_Pause();
	}
#endif*/

	/*void __fastcall oCGame_Unpause(oCGame* self, void* vtable);
	auto Hook_oCGame_Unpause = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063E1D0, 0x00664F80, 0x0066BFD0, 0x006C8D50)), &oCGame_Unpause, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Unpause(oCGame* self, void* vtable)
	{
		Hook_oCGame_Unpause(self, vtable);
		Game_Unpause();
	}*/

	/*void __fastcall oCGame_DefineExternals_Ulfi(oCGame* self, void* vtable, zCParser* parser);
	auto Hook_oCGame_DefineExternals_Ulfi = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x006495B0, 0x006715F0, 0x00677A00, 0x006D4780)), &oCGame_DefineExternals_Ulfi, Union::HookType::Hook_Detours);
	void __fastcall oCGame_DefineExternals_Ulfi(oCGame* self, void* vtable, zCParser* parser)
	{
		Hook_oCGame_DefineExternals_Ulfi(self, vtable, parser);
		Game_DefineExternals();
	}*/

	/*void __fastcall CGameManager_ApplySomeSettings(CGameManager* self, void* vtable);
	auto Hook_CGameManager_ApplySomeSettings = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x004267C0, 0x004291E0, 0x00427370, 0x004276B0)), &CGameManager_ApplySomeSettings, Union::HookType::Hook_Detours);
	void __fastcall CGameManager_ApplySomeSettings(CGameManager* self, void* vtable)
	{
		Hook_CGameManager_ApplySomeSettings(self, vtable);
		Game_ApplySettings();
	}*/
}
