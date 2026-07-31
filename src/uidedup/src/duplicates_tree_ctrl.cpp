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
}

void DuplicatesTreeCtrl::SetDuplicatesTree(const std::optional<fs::path>& targetFolder)
{
    wxTreeItemId rootId = SetupOrCleanupRootElement(targetFolder);

    if (ScanTargetFolder(targetFolder))
    {
        PopulateTree(rootId);
    }
}

bool DuplicatesTreeCtrl::ScanTargetFolder(const std::optional<fs::path>& targetFolder)
{
    if (!targetFolder.has_value())
    {
        return false;
    }

    try
    {
        Deduplicator::DuplicatesFinder df{targetFolder.value().string()};
        checksumFilesMap = df.scan();
    }
    catch (const std::exception& e)
    {
        const auto msg = std::format("Scanning a target folder \"{}\" for duplicates: {}",
                                     targetFolder.value().string(), e.what());
        Logger::get(LOGGER_NAME).error(msg);
        wxMessageBox("Error scanning for duplicates", msg, wxOK | wxICON_ERROR);
        return false;
    }

    return checksumFilesMap.size() > 0;
}

wxTreeItemId DuplicatesTreeCtrl::SetupOrCleanupRootElement(
    const std::optional<fs::path>& targetFolder)
{
    // Delete all children but preserve the root item
    wxTreeItemId rootId = GetRootItem();
    if (rootId.IsOk())
    {
        DeleteChildren(rootId);
    }
    else
    {
        if (targetFolder.has_value())
        {
            rootId = AddRoot(targetFolder.value().c_str());
        }
        else
        {
            rootId = AddRoot("Duplicates");
        }
    }
    return rootId;
}

void DuplicatesTreeCtrl::PopulateTree(const wxTreeItemId& rootId)
{
    for (const auto& [checksum, files] : checksumFilesMap)
    {
        if (files.empty())
        {
            continue;
        }

        if (auto& path = files.front().path(); wxImage::CanRead(path) == false)
        {
            Logger::get(LOGGER_NAME).warning(std::format("{} file format is not supported", path));
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

} // namespace Deduplicator::Ui