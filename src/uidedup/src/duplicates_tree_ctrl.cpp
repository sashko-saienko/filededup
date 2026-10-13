//
// Created by sashko on 29.07.26.
//
#include <Poco/Logger.h>
#include <algorithm>
#include <application.h>
#include <custom_events.h>
#include <deduplib/duplicates_finder.h>
#include <duplicates_tree_ctrl.h>
#include <format>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

using Poco::Logger;

namespace Deduplicator::Ui
{

DuplicatesTreeCtrl::DuplicatesTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                                       const wxSize& size, long style)
    : wxTreeCtrl(parent, id, pos, size, style), m_uncheckedImg(-1), m_checkedImg(-1), checked{}
{
    wxImageList* imageList = new wxImageList(16, 16);

    wxImage uncheckedImg = Application::GetUncheckedImage();
    m_uncheckedImg = imageList->Add(uncheckedImg);

    wxImage checkedImg = Application::GetCheckedImage();
    m_checkedImg = imageList->Add(checkedImg);

    AssignImageList(imageList);

    Bind(wxEVT_TREE_ITEM_ACTIVATED, &DuplicatesTreeCtrl::OnItemActivated, this);
    Bind(wxEVT_TREE_SEL_CHANGED, &DuplicatesTreeCtrl::OnSelectionChanged, this);
}

void DuplicatesTreeCtrl::SetDuplicatesTree(const std::string& targetFolder)
{
    try
    {
        Deduplicator::DuplicatesFinder df{targetFolder};
        checksumFilesMap = df.scan();
    }
    catch (const std::exception& e)
    {
        const auto msg = std::format("Scanning a target folder \"{}\" for duplicates: {}",
                                     targetFolder, e.what());
        Logger::get(LOGGER_NAME).error(msg);
        wxMessageBox("Error scanning for duplicates", msg, wxOK | wxICON_ERROR);
        return;
    }

    if (checksumFilesMap.size() > 0)
    {
        Populate(targetFolder);
    }
}

void DuplicatesTreeCtrl::Populate(const std::string& targetFolder)
{
    if (!IsEmpty())
    {
        DeleteAllItems();
    }

    wxTreeItemId rootId = AddRoot(targetFolder.c_str());

    for (const auto& [checksum, files] : checksumFilesMap)
    {
        if (files.empty())
        {
            Logger::get(LOGGER_NAME).error("Checksum without any files.");
            continue;
        }

        wxTreeItemId parent = AppendItem(rootId, wxString::Format("%u", checksum));

        for (const auto& file : files)
        {
            wxTreeItemId fileItem = AppendItem(parent, file.path(), m_uncheckedImg);
        }
    }
}

bool DuplicatesTreeCtrl::IsFileItem(const wxTreeItemId& item) const
{
    if (!item.IsOk() || item == GetRootItem())
    {
        return false;
    }

    return !ItemHasChildren(item);
}

bool DuplicatesTreeCtrl::IsCheckable(const wxTreeItemId& item) const
{
    if (!IsFileItem(item))
    {
        return false;
    }

    wxTreeItemId parent = GetItemParent(item);

    if (!parent.IsOk())
    {
        return false;
    }

    int uncheckedCount = 0;
    wxTreeItemIdValue cookie;
    wxTreeItemId child = GetFirstChild(parent, cookie);

    while (child.IsOk())
    {
        if (GetItemImage(child) != m_checkedImg)
        {
            uncheckedCount++;
        }
        child = GetNextChild(parent, cookie);
    }

    return uncheckedCount > 1;
}

void DuplicatesTreeCtrl::OnItemActivated(::wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    ActivateItem(item);
}

void DuplicatesTreeCtrl::ActivateItem(wxTreeItemId& item)
{
    if (!IsCheckable(item))
    {
        return;
    }

    int currentImage = GetItemImage(item);

    if (currentImage != m_checkedImg)
    {
        SetItemImage(item, m_checkedImg);
        checked++;
        if (checked == 1)
        {
            SendEvent(Deduplicator::Ui::Events::wxEVT_CHECKED_DUPLICATES_AVAILABLE);
        }
    }
    else if (currentImage == m_checkedImg)
    {
        SetItemImage(item, m_uncheckedImg);
        checked--;
        if (checked == 0)
        {
            SendEvent(Deduplicator::Ui::Events::wxEVT_CHECKED_DUPLICATES_ABSENT);
        }
    }
}

void DuplicatesTreeCtrl::SendEvent(wxEventTypeTag<wxCommandEvent> event) const
{
    wxCommandEvent evt(event, GetId());
    GetEventHandler()->ProcessEvent(evt);
}

void DuplicatesTreeCtrl::OnSelectionChanged(::wxTreeEvent& event)
{
    wxTreeItemId selectedItem = event.GetItem();

    if (IsFileItem(selectedItem))
    {
        event.Skip();
    }
}

void DuplicatesTreeCtrl::DeleteChecked()
{
    wxArrayTreeItemIds checkedItems = GetCheckedTreeItemIds();

    if (checkedItems.IsEmpty())
    {
        return;
    }

    DeleteCheckedTreeItems(checkedItems);
}

wxArrayTreeItemIds DuplicatesTreeCtrl::GetCheckedTreeItemIds() const
{
    wxArrayTreeItemIds checkedItems;

    if (GetCount() == 0)
    {
        return checkedItems;
    }

    wxTreeItemIdValue checksumIdValue;
    wxTreeItemId root = GetRootItem();
    wxTreeItemId checksumItem = GetFirstChild(root, checksumIdValue);

    while (checksumItem.IsOk())
    {
        wxTreeItemIdValue fileIdValue;
        wxTreeItemId fileItem = GetFirstChild(checksumItem, fileIdValue);
        while (fileItem.IsOk())
        {
            if (GetItemImage(fileItem) == m_checkedImg)
            {
                checkedItems.Add(fileItem);
            }

            fileItem = GetNextChild(checksumItem, fileIdValue);
        }
        checksumItem = GetNextChild(root, checksumIdValue);
    }

    return checkedItems;
}

void DuplicatesTreeCtrl::DeleteCheckedTreeItems(wxArrayTreeItemIds& checkedItems)
{
    // Process items in reverse order to avoid iterator invalidation
    for (int i = checkedItems.GetCount() - 1; i >= 0; i--)
    {
        wxTreeItemId item = checkedItems[i];

        if (!IsFileItem(item))
        {
            continue;
        }

        wxTreeItemId parent = GetItemParent(item);

        if (!parent.IsOk() || parent == GetRootItem())
        {
            continue; // Skip if no valid parent
        }

        wxString checksumStr = GetItemText(parent);
        unsigned checksum;

        if (!checksumStr.ToUInt(&checksum))
        {
            continue; // Skip if checksum is not valid
        }

        wxString filePath = GetItemText(item);
        RemoveFileFromChecksumMap(checksum, filePath.ToStdString());

        Delete(item);
    }
}

void DuplicatesTreeCtrl::RemoveFileFromChecksumMap(unsigned checksum, const std::string& filePath)
{
    auto it = checksumFilesMap.find(checksum);
    if (it != checksumFilesMap.end())
    {
        auto& files = it->second;
        auto it_f = find_if(files.begin(), files.end(), [&filePath](const Poco::File& file) {
            return file.path() == filePath;
        });

        Logger::get(LOGGER_NAME)
            .debug("Deleting file %s (checksum: %u).", it_f->absolutePath(), it->first);

        if (files.empty())
        {
            checksumFilesMap.erase(it);
        }
    }
}

} // namespace Deduplicator::Ui