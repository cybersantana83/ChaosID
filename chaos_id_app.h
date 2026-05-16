// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Cafe com Solda / Cristiano Santana
//
// This file is part of ChaosID.
// ChaosID is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version. See <https://www.gnu.org/licenses/gpl-3.0.html>.

#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/widget.h>
#include <gui/modules/popup.h>

#include <lib/lfrfid/lfrfid_worker.h>
#include <lib/lfrfid/protocols/lfrfid_protocols.h>
#include <toolbox/protocols/protocol_dict.h>

#include <nfc/nfc.h>
#include <nfc/nfc_scanner.h>
#include <nfc/nfc_device.h>
#include <nfc/protocols/nfc_protocol.h>
#include <nfc/protocols/iso14443_3a/iso14443_3a.h>
#include <nfc/protocols/iso14443_3a/iso14443_3a_poller_sync.h>

#include "database/cards_db.h"

typedef enum {
    ChaosIdSceneSplash,
    ChaosIdSceneScanning,
    ChaosIdSceneResult,
    ChaosIdSceneAbout,
    ChaosIdSceneCount,
} ChaosIdScene;

typedef enum {
    ChaosIdViewMenu,
    ChaosIdViewScanning,
    ChaosIdViewWidget,
} ChaosIdView;

typedef enum {
    ChaosIdCustomEventStartScan = 100,
    ChaosIdCustomEventShowAbout,
    ChaosIdCustomEventPhaseLfDone,
    ChaosIdCustomEventPhaseHfDone,
    ChaosIdCustomEventHfDetected, // scanner pegou protocolo, hora de fazer sync_read
    ChaosIdCustomEventCardFound,
    ChaosIdCustomEventScanFailed,
} ChaosIdCustomEvent;

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;

    Submenu* submenu;
    Widget* widget;
    Popup* popup;

    FuriTimer* scan_timer;
    uint8_t scan_phase; // 0 = LF, 1 = HF, 2 = done

    // LF worker state (v0.2 real integration)
    LFRFIDWorker* lf_worker;
    ProtocolDict* lf_dict;
    bool lf_done; // guard: ignora callbacks pos-timeout/transicao

    // HF scanner state (v0.2 real integration)
    Nfc* nfc;
    NfcScanner* nfc_scanner;
    bool hf_done;
    NfcProtocol detected_protocol; // protocolo detectado pelo scanner

    const CardProfile* last_result;
    char uid_buffer[48];          // UID hex bruto (bytes raw via get_data)
    char card_data_buffer[48];    // Dados parseados (render_brief_data: FC, Card, etc)
} ChaosIdApp;
