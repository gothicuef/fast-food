#pragma once

#include <ZenGin/zGothicAPI.h>
#include "Plugin.hpp"

HOOKSPACE(GOTHIC_NAMESPACE, ENGINE)
{
    HOOK Hook_oCNpc_InitByScript AS(oCNpc, InitByScript, void(zSTRING &name, zSTRING &slot))
    {
        // Zavoláme původní funkci
        THISCALL(Hook_oCNpc_InitByScript)(name, slot);

        if (this)
        {
            int worldNum = 0;

            if (ogame && ogame->GetGameWorld() && ogame->GetGameWorld()->worldInfo)
            {
                worldNum = ogame->GetGameWorld()->worldInfo->worldVersion;
            }

            this->name += " [World " + zSTRING(worldNum) + "]";
        }
    }
} // <- tady žádný středník

