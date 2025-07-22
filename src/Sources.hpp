#pragma once

#ifdef __G1
#include "Plugin.hpp"
#include <ZenGin/Gothic_I_Classic/API/zString.h>
#include <ZenGin/Gothic_I_Classic/API/oNpc.h>

void HOOKFUNC_AS(oCNpc, InitByScript)(Gothic_I_Classic::oCNpc* _this, Gothic_I_Classic::zSTRING& name, Gothic_I_Classic::zSTRING& slot)
{
    // Zavoláme původní funkci
    THISCALL(Hook_oCNpc_InitByScript)(_this, name, slot);

    if (!_this)
        return;

    Gothic_I_Classic::zSTRING worldNum = "";

    if (Gothic_I_Classic::ogame && Gothic_I_Classic::ogame->GetGameWorld() && Gothic_I_Classic::ogame->GetGameWorld()->worldName) {
        worldNum = Gothic_I_Classic::ogame->GetGameWorld()->worldName;
    }

    // Přidáme k jménu NPC číslo světa
    _this->name[0] += " [World " + Gothic_I_Classic::zSTRING(worldNum) + "]";
}
#endif