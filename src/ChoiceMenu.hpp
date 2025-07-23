//
// Created by warfacez on 23.7.25.
//
#pragma once

#include <functional>
#include <string>
#include <ZenGin/Gothic_UserAPI/zCView.inl>
#include <ZenGin/Gothic_UserAPI/zCInput.inl>
#include <ZenGin/Gothic_UserAPI/zCFont.inl>
#include <ZenGin/zGothicAPI.h>
#include "ZenGin/Gothic_I_Classic/API/zView.h"

namespace GOTHIC_NAMESPACE {

    struct SimpleMenu {
        zCView* view;
        std::vector<std::string> options;
        int selected = 0;
        bool active = false;
        std::function<void(int)> onSelect;
    };
    SimpleMenu gMenu;


    void PrintMenu() {
        if (!gMenu.active || !gMenu.view) return;

        gMenu.view->ClrPrintwin(); // vyčistit text
        gMenu.view->PrintCXY("=== VYBER MOZNOST ===");

        int startY = 1000; // relativní souřadnice (8192 = celý screen)
        int stepY  = 400;  // mezera mezi položkami

        for (size_t i = 0; i < gMenu.options.size(); i++) {
            std::string line = gMenu.options[i];
            if ((int)i == gMenu.selected)
                line = "-> " + line; // zvýraznění vybrané položky

            gMenu.view->Print(4096, startY + (int)i * stepY,line.c_str());
        }
    }

    void ShowMenu(const std::vector<std::string>& options, std::function<void(int)> onSelect) {
        if (gMenu.active) return;

        gMenu.options = options;
        gMenu.selected = 0;
        gMenu.onSelect = onSelect;
        gMenu.active = true;

        int left   = 2000;
        int top    = 1500;
        int right  = 6192;
        int bottom = 5000;

        // Vytvoření overlay view
        gMenu.view = new zCView(left, top, right, bottom);
        //gMenu.view->InsertBack("MENU_BACK.TGA"); // může být prázdné
        gMenu.view->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
        gMenu.view->InsertBack("BLACK.TGA"); // nebo malá textura
        gMenu.view->SetTransparency(200);    // 0=plně průhledné, 255=neprůhledné
        gMenu.view->SetFont("FONT_OLD_10_WHITE_HI.TGA");

        // Přidat view do obrazovky
        screen->InsertItem(gMenu.view);

        // První render
        PrintMenu();
    }

    void CloseMenu() {
        if (!gMenu.active) return;

        screen->RemoveItem(gMenu.view);
        delete gMenu.view;
        gMenu.view = nullptr;
        gMenu.active = false;
    }

    void HandleMenuInput() {
        if (!gMenu.active) return;

        if (zinput->KeyToggled(KEY_UP)) {
            gMenu.selected--;
            if (gMenu.selected < 0) gMenu.selected = (int)gMenu.options.size() - 1;
            PrintMenu();
        }

        if (zinput->KeyToggled(KEY_DOWN)) {
            gMenu.selected++;
            if (gMenu.selected >= (int)gMenu.options.size()) gMenu.selected = 0;
            PrintMenu();
        }

        if (zinput->KeyToggled(KEY_RETURN) || zinput->KeyToggled(KEY_SPACE)) {
            int choice = gMenu.selected;
            auto callback = gMenu.onSelect;
            CloseMenu();
            if (callback) callback(choice);
        }

        if (zinput->KeyToggled(KEY_ESCAPE)) {
            CloseMenu();
        }
    }
}

