// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Cafe com Solda / Cristiano Santana
//
// This file is part of ChaosID.
// ChaosID is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version. See <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "chaos_id_scene.h"
#include "../chaos_id_app.h"

static void (*const chaos_id_on_enter[])(void*) = {
    chaos_id_scene_splash_on_enter,
    chaos_id_scene_scanning_on_enter,
    chaos_id_scene_result_on_enter,
    chaos_id_scene_about_on_enter,
};

static bool (*const chaos_id_on_event[])(void*, SceneManagerEvent) = {
    chaos_id_scene_splash_on_event,
    chaos_id_scene_scanning_on_event,
    chaos_id_scene_result_on_event,
    chaos_id_scene_about_on_event,
};

static void (*const chaos_id_on_exit[])(void*) = {
    chaos_id_scene_splash_on_exit,
    chaos_id_scene_scanning_on_exit,
    chaos_id_scene_result_on_exit,
    chaos_id_scene_about_on_exit,
};

const SceneManagerHandlers chaos_id_scene_handlers = {
    .on_enter_handlers = chaos_id_on_enter,
    .on_event_handlers = chaos_id_on_event,
    .on_exit_handlers = chaos_id_on_exit,
    .scene_num = ChaosIdSceneCount,
};
