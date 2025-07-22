//
// Created by warfacez on 22.7.25.
//

#include "fastFood.hpp"

#include "Union/Hook.h"
#include <Union/Signature.h>
#include "ZenGin/Gothic_I_Classic/API/oNpc.h"
#include "ZenGin/Gothic_I_Classic/API/zPrototypes.h"

namespace Gothic_I_Classic {

    using namespace Union;

    void* FindEngineAddress(Signature* baseSign) {
        auto compSign = Signature::GetCompatibleSignature(baseSign, "Gothic_I_Classic_Names.txt");
        return compSign ? compSign->GetAddress() : nullptr;
    }

#define ENGINE_ADDRESS_OF(what) FindEngineAddress(SIGNATURE_OF(what))

    auto hook = CreateHook(ENGINE_ADDRESS_OF(&oCNpc::Enable), &oCNpc::Enable_Hooked);

    void oCNpc::Enable_Hooked(zVEC3& position)
    {
        // TODO
        (this->*FindEngineAddress)(position);
    }
}