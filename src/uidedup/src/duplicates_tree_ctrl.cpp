//
// Created by sashko on 29.07.26.
//
#include <Poco/Logger.h>
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
    : wxTreeCtrl(parent, id, pos, size, style), m_uncheckedImg(-1), m_checkedImg(-1)
{
    wxImageList* imageList = new wxImageList(16, 16);

    wxFileName execFileName{wxStandardPaths::Get().GetExecutablePath()};
    wxString resourcesPath = wxString::Format("%s/resources/", execFileName.GetPath());

    wxImage uncheckedImg;
    uncheckedImg.LoadFile(resourcesPath + "unchecked.png", wxBITMAP_TYPE_PNG);
    m_uncheckedImg = imageList->Add(uncheckedImg);

    wxImage checkedImg;
    checkedImg.LoadFile(resourcesPath + "checked.png", wxBITMAP_TYPE_PNG);
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
            continue;
        }

        wxTreeItemId parent = AppendItem(rootId, wxString::Format("%d", checksum));
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

void DuplicatesTreeCtrl::OnItemActivated(::wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();

    if (IsFileItem(item))
    {
        int currentImage = GetItemImage(item);
        if (currentImage == m_checkedImg)
        {
            SetItemImage(item, m_uncheckedImg);
        }
        else
        {
            SetItemImage(item, m_checkedImg);
        }
    }
}

void DuplicatesTreeCtrl::OnSelectionChanged(::wxTreeEvent& event)
{
    wxTreeItemId selectedItem = event.GetItem();

    if (IsFileItem(selectedItem))
    {
        event.Skip();
    }
}

} // namespace Deduplicator::Ui