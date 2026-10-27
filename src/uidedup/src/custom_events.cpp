// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Sashko
//
// Created for custom events in Deduplicator::Ui::Events namespace

#include <custom_events.h>

namespace Deduplicator::Ui::Events
{
wxDEFINE_EVENT(wxEVT_CHECKED_DUPLICATES_AVAILABLE, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CHECKED_DUPLICATES_ABSENT, wxCommandEvent);
} // namespace Deduplicator::Ui::Events
