// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Cafe com Solda / Cristiano Santana
//
// This file is part of ChaosID.
// ChaosID is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version. See <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "../chaos_id_app.h"
#include "chaos_id_scene.h"

void chaos_id_scene_result_on_enter(void* context) {
    ChaosIdApp* app = context;
    widget_reset(app->widget);

    if(!app->last_result) {
        widget_add_string_element(
            app->widget, 64, 30, AlignCenter, AlignCenter, FontPrimary, "Sem resultado");
        view_dispatcher_switch_to_view(app->view_dispatcher, ChaosIdViewWidget);
        return;
    }

    const CardProfile* c = app->last_result;
    FuriString* body = furi_string_alloc();

    furi_string_printf(
        body,
        "Freq: %s\n"
        "UID:  %s\n"
        "Dados: %s\n"
        "\n"
        "Uso tipico:\n%s\n"
        "\n"
        "Cripto:\n%s\n"
        "\n"
        "Ataques:\n%s\n"
        "\n"
        "Quebrada em: %s\n"
        "\n"
        "Risco: %s\n"
        "\n"
        "Notas:\n%s",
        card_frequency_label(c->frequency),
        app->uid_buffer,
        app->card_data_buffer,
        c->typical_use,
        c->crypto,
        c->attack_vector,
        c->year_broken,
        card_risk_label(c->risk),
        c->notes);

    widget_add_string_element(
        app->widget, 64, 2, AlignCenter, AlignTop, FontPrimary, c->protocol);
    widget_add_text_scroll_element(
        app->widget, 0, 14, 128, 50, furi_string_get_cstr(body));

    furi_string_free(body);
    view_dispatcher_switch_to_view(app->view_dispatcher, ChaosIdViewWidget);
}

bool chaos_id_scene_result_on_event(void* context, SceneManagerEvent event) {
    ChaosIdApp* app = context;
    bool consumed = false;
    if(event.type == SceneManagerEventTypeBack) {
        // Back limpa o pipeline e volta pro splash
        scene_manager_search_and_switch_to_previous_scene(
            app->scene_manager, ChaosIdSceneSplash);
        consumed = true;
    }
    return consumed;
}

void chaos_id_scene_result_on_exit(void* context) {
    ChaosIdApp* app = context;
    widget_reset(app->widget);
}
