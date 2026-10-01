//
// Created by sashko on 14.03.26.
//
#include <Poco/Logger.h>
#include <deduplib/duplicates_finder.h>
#include <duplicates_tree_ctrl.h>
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
#include <wx/wx.h>

namespace Deduplicator::Ui
{
using Poco::Logger;

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "File Deduplicator", wxDefaultPosition, wxSize(800, 600)),
      m_treeCtrl(nullptr), m_previewImage(nullptr), m_previewPanel(nullptr),
      m_previewSizer(nullptr), m_toolBar(nullptr)
{
    SetupMenuBar();
    SetupToolBar();
    SetupMainLayout();

    CreateStatusBar();
    SetStatusText("Start by selecting a folder where to serch duplicates for.");

    // Bind events
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnSelectFolder, this, ID_SELECTFOLDER);
    Bind(wxEVT_MENU, &MainFrame::OnRefresh, this, ID_REFRESH);
    Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::OnItemSelected, this, ID_DUPLICATES_TREE);
    Bind(wxEVT_TOOL, &MainFrame::OnSelectFolder, this, ID_SELECTFOLDER);
    Bind(wxEVT_TOOL, &MainFrame::OnRefresh, this, ID_REFRESH);

    UpdateRefreshButtonState();
}

void MainFrame::SetupMenuBar()
{
    ::wxMenu* menuFile = new ::wxMenu();
    menuFile->Append(ID_SELECTFOLDER, "&Select Folder\tCtrl-O",
                     "Select target folder for deduplication");
    menuFile->Append(ID_REFRESH, "&Refresh\tF5", "Refresh duplicates list");
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
    m_toolBar->AddTool(ID_SELECTFOLDER, "Select Folder", folderBitmap,
                       "Select target folder for deduplication");

    // Add refresh tool
    wxBitmap refreshBitmap = wxArtProvider::GetBitmap(wxART_REFRESH, wxART_TOOLBAR, wxSize(16, 16));
    m_toolBar->AddTool(ID_REFRESH, "Refresh", refreshBitmap, "Refresh duplicates list");

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

    m_treeCtrl = new DuplicatesTreeCtrl(leftPanel, ID_DUPLICATES_TREE, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_SINGLE | wxTR_FULL_ROW_HIGHLIGHT);

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

bool MainFrame::ScanTargetFolder()
{
    if (!_targetFolder.has_value())
    {
        return false;
    }

    try
    {
        Deduplicator::DuplicatesFinder df{_targetFolder.value()};
        checksumFilesMap = df.scan();
    }
    catch (const std::exception& e)
    {
        const auto msg = std::format("Scanning a target folder \"{}\" for duplicates: {}",
                                     _targetFolder.value().string(), e.what());
        Logger::get(LOGGER_NAME).error(msg);
        wxMessageBox("Error scanning for duplicates", msg, wxOK | wxICON_ERROR);
        return false;
    }

    return checksumFilesMap.size() > 0;
}

void MainFrame::SearchDuplicates()
{
    if (_targetFolder.has_value() != true)
    {
        wxMessageBox("Target folder is not set.", "Error searching for duplicates",
                     wxOK | wxICON_ERROR);
        return;
    }

    if (ScanTargetFolder())
    {
        m_treeCtrl->SetDuplicatesTree(checksumFilesMap, _targetFolder.value().string());
        m_treeCtrl->ExpandAll();
    }
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
    wxDirDialog dialog(this, "Select target folder for deduplication", ::wxGetUserHome(),
                       wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dialog.ShowModal() == wxID_OK)
    {
        _targetFolder = dialog.GetPath().ToStdString();
        UpdateRefreshButtonState();
        SearchDuplicates();
    }
}

void MainFrame::OnRefresh(::wxCommandEvent& event)
{
    SearchDuplicates();
}

void MainFrame::OnItemSelected(::wxTreeEvent& event)
{
    // Update the preview panel based on the selected item from the tree
    wxTreeItemId selectedItem = event.GetItem();
    wxString itemText = m_treeCtrl->GetItemText(selectedItem);
    previewImage(itemText);
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

void MainFrame::UpdateRefreshButtonState()
{
    if (m_toolBar)
    {
        bool enableRefresh = !_targetFolder.has_value();
        m_toolBar->EnableTool(ID_REFRESH, enableRefresh);
    }
}

} // namespace Deduplicator::Ui