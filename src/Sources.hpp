#pragma once

// Zde jsou správné includy z gitlab verzí API
#include <ZenGin/oCNpc.h>
#include <ZenGin/oGame.h>
#include <ZenGin/oWorld.h>
#include <ZenGin/zString.h>
#include <Union/Hook.h>

// Hook pro InitByScript
HOOKSPACE(Gothic_I_Classic, ENGINE)
{
    // Originální funkce
    static auto Hook_oCNpc_InitByScript = Union::CreateHook(
        zSwitch(0x0062D1F0, 0x0064A590, 0x006508E0, 0x006AD240), // offsety pro G1/G1A/G2/G2A
        &Hook_oCNpc_InitByScript_AS,
        Union::HookType::Hook_Detours
    );

    // Hook implementace
    void __fastcall Hook_oCNpc_InitByScript_AS(oCNpc* _this, void* vtable, zSTRING name, zSTRING slot)
    {
        // Zavoláme původní funkci
        Hook_oCNpc_InitByScript(_this, vtable, name, slot);

        // Získáme název aktuálního světa
        oCWorld* world = ogame->GetGameWorld(); // přístup k world
        zSTRING worldName = world ? world->GetWorldName() : "UNKNOWN";

        // Přidáme číslo/ID světa k jménu NPC
        _this->name += " [" + worldName + "]";
    }
}
