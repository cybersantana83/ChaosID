// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Cafe com Solda / Cristiano Santana

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

#include <storage/storage.h>

#include "database/cards_db.h"

typedef enum {
    ChaosIdSceneSplash,
    ChaosIdSceneScanning,
    ChaosIdSceneResult,
    ChaosIdSceneHistory,
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
    ChaosIdCustomEventShowHistory,
    ChaosIdCustomEventPhaseLfDone,
    ChaosIdCustomEventPhaseHfDone,
    ChaosIdCustomEventHfDetected,
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

    Storage* storage;

    FuriTimer* scan_timer;
    uint8_t scan_phase; // 0 = LF, 1 = HF

    // LF worker state
    LFRFIDWorker* lf_worker;
    ProtocolDict* lf_dict;
    bool lf_done;

    // HF scanner state
    Nfc* nfc;
    NfcScanner* nfc_scanner;
    bool hf_done;
    NfcProtocol detected_protocol;

    const CardProfile* last_result;
    char uid_buffer[48];
    char card_data_buffer[48];
} ChaosIdApp;
