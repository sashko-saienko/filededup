//
// Created by sashko on 14.03.26.
//
#include <Poco/Logger.h>
#include <deduplib/duplicates_finder.h>
#include <format>
#include <main_frame.h>
#include <reusablelib/time_helpers.h>
#include <string>
#include <wx/artprov.h>
#include <wx/dirdlg.h>
#include <wx/filefn.h>
#include <wx/gdicmn.h>
#include <wx/generic/statbmpg.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

namespace Deduplicator::Ui
{
using Poco::Logger;

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "File Deduplicator", wxDefaultPosition, wxSize(800, 600)),
      m_treeCtrl(nullptr), m_previewImage(nullptr), m_previewPanel(nullptr),
      m_previewSizer(nullptr), m_toolBar(nullptr)
{
    // Initialize all UI components
    Initialize();
}

void MainFrame::Initialize()
{
    SetupMenuBar();
    SetupToolBar();
    SetupMainLayout();
    FindDuplicates();

    CreateStatusBar();
    SetStatusText("Start by selecting a folder where to serch duplicates for.");

    // Bind events
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::OnItemSelected, this);
    Bind(wxEVT_TOOL, &MainFrame::OnSelectFolder, this, ID_SELECTFOLDERTLBR);
    Bind(wxEVT_TOOL, &MainFrame::OnRefresh, this, ID_REFRESHTLBR);
}

void MainFrame::SetupMenuBar()
{
    ::wxMenu* menuFile = new ::wxMenu();
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    ::wxMenu* menuHelp = new ::wxMenu();
    menuHelp->Append(wxID_ABOUT);

    ::wxMenuBar* menuBar = new ::wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
}

void MainFrame::SetupToolBar()
{
    m_toolBar = CreateToolBar(wxTB_HORIZONTAL | wxNO_BORDER);

    // Add folder selection tool
    wxBitmap folderBitmap =
        wxArtProvider::GetBitmap(wxART_FOLDER_OPEN, wxART_TOOLBAR, wxSize(16, 16));
    m_toolBar->AddTool(ID_SELECTFOLDERTLBR, "Select Folder", folderBitmap,
                       "Select target folder for deduplication");

    // Add refresh tool
    wxBitmap refreshBitmap = wxArtProvider::GetBitmap(wxART_REFRESH, wxART_TOOLBAR, wxSize(16, 16));
    m_toolBar->AddTool(ID_REFRESHTLBR, "Refresh", refreshBitmap, "Refresh duplicates list");

    m_toolBar->Realize();
}

void MainFrame::SetupMainLayout()
{
    // Create main splitter window
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
    splitter->SetMinimumPaneSize(200);

    // Left panel with tree control
    wxPanel* leftPanel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    leftPanel->SetSizer(leftSizer);

    m_treeCtrl = new wxTreeCtrl(leftPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxTR_DEFAULT_STYLE | wxTR_SINGLE);

    leftSizer->Add(m_treeCtrl, 1, wxEXPAND | wxALL, 5);

    // Right panel for preview
    m_previewPanel = new wxPanel(splitter, wxID_ANY);
    m_previewSizer = new wxBoxSizer(wxVERTICAL);
    m_previewPanel->SetSizer(m_previewSizer);

    // Split the window
    splitter->SplitVertically(leftPanel, m_previewPanel, 300);

    m_previewImage = new wxGenericStaticBitmap(m_previewPanel, wxID_ANY, wxBitmap());
    m_previewImage->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
    m_previewImage->Hide();

    m_previewSizer->AddStretchSpacer();
    m_previewSizer->Add(m_previewImage, 1, wxEXPAND | wxALL | wxCENTER, 5);
    m_previewSizer->AddStretchSpacer();

    // Main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(splitter, 1, wxEXPAND);
    SetSizer(mainSizer);
}

void MainFrame::FindDuplicates()
{
    EnumerateTargetDirectory();

    wxTreeItemId rootId = SetupOrCleanupRootElement();

    // Add text files
    for (const auto& [checksum, files] : cfm)
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

        wxTreeItemId parent = m_treeCtrl->AppendItem(rootId, wxString::Format("%d", checksum));
        for (const auto& file : files)
        {
            m_treeCtrl->AppendItem(parent, file.path());
        }
    }
    m_treeCtrl->ExpandAll();
}

bool MainFrame::EnumerateTargetDirectory()
{
    using ReusableLib::ObservableStopwatch;

    ObservableStopwatch sw([&](const long seconds) {
        Logger::get(LOGGER_NAME).information("Completed duplicates search in %ld(s)", seconds);
    });

    try
    {
        Deduplicator::DuplicatesFinder df{_targetDirectory};
        cfm = df.scan();
    }
    catch (const std::exception& e)
    {
        const auto msg = std::format("Scanning a target directory \"{}\" for duplicates: {}",
                                     _targetDirectory.string(), e.what());
        Logger::get(LOGGER_NAME).error(msg);
        wxMessageBox("Error scanning for duplicates", msg, wxOK | wxICON_ERROR);
    }

    return cfm.size() > 0;
}

void MainFrame::OnExit(::wxCommandEvent& _)
{
    Close(true);
}

void MainFrame::OnAbout(::wxCommandEvent& _)
{
    wxMessageBox("File Deduplicator\nwxWidgets application for file deduplication",
                 "About File Deduplicator", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSelectFolder(::wxCommandEvent& event)
{
    wxDirDialog dialog(this, "Select target directory for deduplication", _targetDirectory.string(),
                       wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dialog.ShowModal() == wxID_OK)
    {
        _targetDirectory = dialog.GetPath().ToStdString();

        // Clear the tree and repopulate with new directory contents
        FindDuplicates();
    }
}

void MainFrame::OnRefresh(::wxCommandEvent& event)
{
    FindDuplicates();
}

void MainFrame::OnItemSelected(::wxTreeEvent& event)
{
    // Update the preview panel based on the selected item from the tree
    wxTreeItemId selectedItem = event.GetItem();
    wxString itemText = m_treeCtrl->GetItemText(selectedItem);

    // Only allow preview for actual file items (not root or checksum items)
    if (IsFileItem(selectedItem))
    {
        previewImage(itemText);
    }
    else
    {
        SetStatusText("Click on an image file to preview it.");
    }
}

bool MainFrame::IsFileItem(const wxTreeItemId& item) const
{
    // A file item is not the root and has no children (leaf node)
    if (!item.IsOk() || item == m_treeCtrl->GetRootItem())
    {
        return false;
    }

    // Check if this item has children - if it does, it's a checksum group
    return !m_treeCtrl->ItemHasChildren(item);
}

void MainFrame::previewImage(const wxString& imagePath)
{
    // Load and display the image
    if (wxFileExists(imagePath) == false)
    {
        auto errorMessage = wxString::Format("Image file does not exist: %s", imagePath);
        SetStatusText(errorMessage);
        wxMessageBox(errorMessage, "Error finding image to open it for preview", wxOK | wxCENTER,
                     this);
        return;
    }

    wxImage image;
    if (image.LoadFile(imagePath) == false)
    {
        auto errorMessage = wxString::Format("Image file not supported: %s", imagePath);
        SetStatusText(errorMessage);
        wxMessageBox(errorMessage, "Error opening image for preview", wxOK | wxCENTER, this);
        return;
    }

    wxBitmap bitmap(image);
    m_previewImage->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
    m_previewImage->SetBitmap(bitmap);
    m_previewImage->Show();

    SetStatusText(wxString::Format("Previewing: %s", imagePath));
    m_previewPanel->Layout();
    m_previewPanel->Refresh();
}

wxTreeItemId MainFrame::SetupOrCleanupRootElement()
{
    // Delete all children but preserve the root item
    wxTreeItemId rootId = m_treeCtrl->GetRootItem();
    if (rootId.IsOk())
    {
        m_treeCtrl->DeleteChildren(rootId);
    }
    else
    {
        rootId = m_treeCtrl->AddRoot("Checksum duplicates");
    }
    return rootId;
}

} // namespace Deduplicator::Ui