// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Cafe com Solda / Cristiano Santana
//
// This file is part of ChaosID.
// ChaosID is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version. See <https://www.gnu.org/licenses/gpl-3.0.html>.

#pragma once

#include <gui/scene_manager.h>

void chaos_id_scene_splash_on_enter(void* context);
bool chaos_id_scene_splash_on_event(void* context, SceneManagerEvent event);
void chaos_id_scene_splash_on_exit(void* context);

void chaos_id_scene_scanning_on_enter(void* context);
bool chaos_id_scene_scanning_on_event(void* context, SceneManagerEvent event);
void chaos_id_scene_scanning_on_exit(void* context);

void chaos_id_scene_result_on_enter(void* context);
bool chaos_id_scene_result_on_event(void* context, SceneManagerEvent event);
void chaos_id_scene_result_on_exit(void* context);

void chaos_id_scene_about_on_enter(void* context);
bool chaos_id_scene_about_on_event(void* context, SceneManagerEvent event);
void chaos_id_scene_about_on_exit(void* context);

extern const SceneManagerHandlers chaos_id_scene_handlers;
