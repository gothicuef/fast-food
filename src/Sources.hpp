#pragma once

#include "Plugin.hpp"

void HOOKFUNC_AS(oCNpc, InitByScript)(oCNpc* _this, zSTRING& name, zSTRING& slot)
{
    // Zavoláme původní funkci
    THISCALL(Hook_oCNpc_InitByScript)(_this, name, slot);

    if (!_this)
        return;

    int worldNum = 0;

    if (ogame && ogame->GetGameWorld() && ogame->GetGameWorld()->worldInfo) {
        worldNum = ogame->GetGameWorld()->worldInfo->worldVersion;
    }

    // Přidáme k jménu NPC číslo světa
    _this->name += " [World " + zSTRING(worldNum) + "]";
}
