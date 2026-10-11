#ifndef __UIDEDUP_CUSTOM_EVENTS_H__
#define __UIDEDUP_CUSTOM_EVENTS_H__

#include <wx/event.h>

namespace Deduplicator::Ui::Events
{
wxDECLARE_EVENT(wxEVT_CHECKED_DUPLICATES_AVAILABLE, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_CHECKED_DUPLICATES_ABSENT, wxCommandEvent);
} // namespace Deduplicator::Ui::Events

#endif // __UIDEDUP_CUSTOM_EVENTS_H__
