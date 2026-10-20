//
// Created by sashko on 29.07.26.
//
#include <Poco/File.h>
#include <Poco/Logger.h>
#include <algorithm>
#include <application.h>
#include <custom_events.h>
#include <deduplib/duplicates_finder.h>
#include <duplicates_tree_ctrl.h>
#include <format>
#include <thread>
#include <vector>
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

void DuplicatesTreeCtrl::SetDuplicatesTree(std::stop_token st, const std::string& targetFolder)
{
    try
    {
        Deduplicator::DuplicatesFinder df{targetFolder};
        checksumFilesMap = df.scan(st);
    }
    catch (const std::exception& e)
    {
        const auto msg = std::format("Scanning a target folder \"{}\" for duplicates: {}",
                                     targetFolder, e.what());
        Logger::get(LOGGER_NAME).error(msg);
        wxMessageBox("Error scanning for duplicates", msg, wxOK | wxICON_ERROR);
        return;
    }

    if (!st.stop_requested() && checksumFilesMap.size() > 0)
    {
        Populate(targetFolder);
    }

    if (st.stop_requested())
    {
        DeleteAllItems();
        checksumFilesMap.clear();
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

bool DuplicatesTreeCtrl::canSelectWithinChecksumGroup(const wxTreeItemId& item) const
{
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

void DuplicatesTreeCtrl::ActivateItem(const wxTreeItemId& item)
{
    if (!IsFileItem(item))
    {
        return;
    }

    int currentImage = GetItemImage(item);
    bool hadChecked = HasCheckedItems();

    if (currentImage != m_checkedImg && canSelectWithinChecksumGroup(item))
    {
        CheckItem(item);
        if (!hadChecked)
        {
            SendEvent(Deduplicator::Ui::Events::wxEVT_CHECKED_DUPLICATES_AVAILABLE);
        }
    }
    else if (currentImage == m_checkedImg)
    {
        UncheckItem(item);
        if (hadChecked && !HasCheckedItems())
        {
            SendEvent(Deduplicator::Ui::Events::wxEVT_CHECKED_DUPLICATES_ABSENT);
        }
    }
}

void DuplicatesTreeCtrl::CheckItem(const wxTreeItemId& item)
{
    SetItemImage(item, m_checkedImg);
    checked++;
}

void DuplicatesTreeCtrl::UncheckItem(const wxTreeItemId& item)
{
    SetItemImage(item, m_uncheckedImg);
    checked--;
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
    DeleteCheckedTreeItems(checkedItems);
}

void DuplicatesTreeCtrl::ClearChecked()
{
    wxArrayTreeItemIds checkedItems = GetCheckedTreeItemIds();
    std::for_each(checkedItems.begin(), checkedItems.end(),
                  [this](const wxTreeItemId& item) { ActivateItem(item); });
}

void DuplicatesTreeCtrl::CheckOldest()
{
    EnumerateChecksumFilesSortedGroups([](const Poco::File* l, const Poco::File* r) -> bool {
        return l->created() > r->created();
    });
}

void DuplicatesTreeCtrl::CheckNewest()
{
    EnumerateChecksumFilesSortedGroups([](const Poco::File* l, const Poco::File* r) -> bool {
        return l->getLastModified() < r->getLastModified();
    });
}

void DuplicatesTreeCtrl::EnumerateChecksumFilesSortedGroups(FilesComparisonCallback cmpCallback)
{
    wxTreeItemId root = GetRootItem();

    auto fileGroupEnumerator = [this, &cmpCallback](const wxTreeItemId& checksumItem) -> void {
        auto checksum = GetChecksumFromItem(checksumItem);

        if (!checksum.has_value())
        {
            return;
        }

        auto checksumValue = checksum.value();
        auto files = checksumFilesMap.at(checksumValue);

        std::vector<const Poco::File*> filePtrs{};
        std::transform(files.begin(), files.end(), std::back_inserter(filePtrs),
                       [](const Poco::File& file) -> const Poco::File* { return &file; });

        std::sort(filePtrs.begin(), filePtrs.end(), cmpCallback);

        EnumerateItems(checksumItem, [this, &filePtrs](const wxTreeItemId& fileItem) -> void {
            auto fileItemPath = GetItemText(fileItem).ToStdString();
            auto it = std::find_if(
                filePtrs.begin(), filePtrs.end(),
                [&fileItemPath](const Poco::File* f) { return fileItemPath == f->path(); });
            if (it == filePtrs.begin())
            {
                UncheckItem(fileItem);
            }
            else if (it != filePtrs.end())
            {
                CheckItem(fileItem);
            }
        });
    };

    EnumerateItems(root, fileGroupEnumerator);
}

wxArrayTreeItemIds DuplicatesTreeCtrl::GetCheckedTreeItemIds()
{
    wxArrayTreeItemIds checkedItems;

    if (!HasCheckedItems())
    {
        return checkedItems;
    }

    wxTreeItemId root = GetRootItem();

    auto checkedItemsAccumulator = [this, &checkedItems](wxTreeItemId checksum) -> void {
        EnumerateItems(checksum, [this, &checkedItems](wxTreeItemId file) -> void {
            if (GetItemImage(file) == m_checkedImg)
            {
                checkedItems.Add(file);
            }
        });
    };

    EnumerateItems(root, checkedItemsAccumulator);

    return checkedItems;
}

void DuplicatesTreeCtrl::EnumerateItems(wxTreeItemId parent, TreeItemIdCallback treeItemCallback)
{
    if (GetCount() == 0)
    {
        return;
    }

    wxTreeItemIdValue cookie;
    wxTreeItemId childItemId = GetFirstChild(parent, cookie);

    while (childItemId.IsOk())
    {
        treeItemCallback(childItemId);
        childItemId = GetNextChild(parent, cookie);
    }
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

        auto checksum = GetChecksumFromItem(parent);

        if (!checksum.has_value())
        {
            continue; // Skip if checksum is not valid
        }

        wxString filePath = GetItemText(item);
        RemoveFileFromChecksumMap(checksum.value(), filePath.ToStdString());

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

        Logger::get(LOGGER_NAME).debug("Deleting file %s (checksum: %u).", it_f->path(), it->first);

        if (files.empty())
        {
            checksumFilesMap.erase(it);
        }
    }
}

std::optional<unsigned> DuplicatesTreeCtrl::GetChecksumFromItem(const wxTreeItemId& item) const
{
    wxString checksumStr = GetItemText(item);

    if (unsigned value{}; checksumStr.ToUInt(&value))
    {
        return std::optional<unsigned>{value};
    }

    return std::optional<unsigned>{};
}

} // namespace Deduplicator::Ui