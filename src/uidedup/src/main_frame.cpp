//
// Created by sashko on 14.03.26.
//
#include <Poco/Logger.h>
#include <custom_events.h>
#include <duplicates_tree_ctrl.h>
#include <main_frame.h>
#include <reusablelib/time_helpers.h>
#include <thread>
#include <wx/artprov.h>
#include <wx/dirdlg.h>
#include <wx/filefn.h>
#include <wx/gdicmn.h>
#include <wx/generic/statbmpg.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/progdlg.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/wx.h>

namespace Deduplicator::Ui
{
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

    SetupEventBinds();

    SetRefreshOptionState();
    SetActionsOverSelectedOptionsState(false);
}

void MainFrame::SetupMenuBar()
{
    ::wxMenu* menuFile = new ::wxMenu();
    menuFile->Append(ID_SELECTFOLDER, "&Select Folder\tCtrl-O",
                     "Select target folder for deduplication");
    menuFile->Append(ID_REFRESH, "&Refresh\tF5", "Refresh duplicates list");
    menuFile->Append(ID_DELETE_SELECTED, "&Delete Selected\tDel",
                     "Delete selected duplicate files");
    menuFile->Append(ID_CLEAR_SELECTED, "&Clear Selected", "Clear selected items");
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

    wxBitmap folderBitmap =
        wxArtProvider::GetBitmap(wxART_FOLDER_OPEN, wxART_TOOLBAR, wxSize(16, 16));
    m_toolBar->AddTool(ID_SELECTFOLDER, "Select Folder", folderBitmap,
                       "Select target folder for deduplication");

    wxBitmap refreshBitmap = wxArtProvider::GetBitmap(wxART_REFRESH, wxART_TOOLBAR, wxSize(16, 16));
    m_toolBar->AddTool(ID_REFRESH, "Refresh", refreshBitmap, "Refresh duplicates list");

    wxBitmap deleteBitmap = wxArtProvider::GetBitmap(wxART_DELETE, wxART_TOOLBAR, wxSize(16, 16));
    m_toolBar->AddTool(ID_DELETE_SELECTED, "Delete Selected", deleteBitmap,
                       "Delete selected items");

    wxBitmap clearBitmap =
        wxArtProvider::GetBitmap(wxART_CROSS_MARK, wxART_TOOLBAR, wxSize(16, 16));
    m_toolBar->AddTool(ID_CLEAR_SELECTED, "Clear Selected", clearBitmap, "Clear selected items");

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

    m_treeCtrl =
        new DuplicatesTreeCtrl(leftPanel, ID_DUPLICATES_TREE, wxDefaultPosition, wxDefaultSize,
                               wxTR_DEFAULT_STYLE | wxTR_SINGLE | wxTR_FULL_ROW_HIGHLIGHT);

    leftSizer->Add(m_treeCtrl, 1, wxEXPAND | wxALL, 5);

    // Right panel for preview
    m_previewPanel = new wxPanel(splitter, wxID_ANY);
    m_previewSizer = new wxBoxSizer(wxVERTICAL);
    m_previewPanel->SetSizer(m_previewSizer);

    // Split the window
    splitter->SplitVertically(leftPanel, m_previewPanel, 300);

    m_previewImage = new ImageFilePreview(m_previewPanel, wxID_ANY, wxBitmap());
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

void MainFrame::SetupEventBinds()
{
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnSelectFolder, this, ID_SELECTFOLDER);
    Bind(wxEVT_MENU, &MainFrame::OnRefresh, this, ID_REFRESH);
    Bind(wxEVT_MENU, &MainFrame::OnDeleteChecked, this, ID_DELETE_SELECTED);
    Bind(wxEVT_MENU, &MainFrame::OnClearSelected, this, ID_CLEAR_SELECTED);
    Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::OnItemSelected, this, ID_DUPLICATES_TREE);
    Bind(wxEVT_TOOL, &MainFrame::OnSelectFolder, this, ID_SELECTFOLDER);
    Bind(wxEVT_TOOL, &MainFrame::OnRefresh, this, ID_REFRESH);
    Bind(wxEVT_TOOL, &MainFrame::OnDeleteChecked, this, ID_DELETE_SELECTED);
    Bind(wxEVT_TOOL, &MainFrame::OnClearSelected, this, ID_CLEAR_SELECTED);
    Bind(Events::wxEVT_CHECKED_DUPLICATES_AVAILABLE, &MainFrame::ToggleActionsOverSelectedItems,
         this);
    Bind(Events::wxEVT_CHECKED_DUPLICATES_ABSENT, &MainFrame::ToggleActionsOverSelectedItems, this);
}

void MainFrame::SetDuplicatesTree()
{
    if (_targetFolder.has_value() != true)
    {
        wxMessageBox("Target folder is not set.", "Error searching for duplicates",
                     wxOK | wxICON_ERROR);
        return;
    }

    std::stop_source stopSource;

    std::jthread worker([this, folder = _targetFolder.value().string(),
                         stopToken = stopSource.get_token()]() mutable {
        m_treeCtrl->SetDuplicatesTree(stopToken, folder);
        if (!stopToken.stop_requested())
        {
            m_treeCtrl->ExpandAll();
        }
    });

    wxProgressDialog progressDialog("Searching for duplicates", "Scanning files...", 100, this,
                                    wxPD_CAN_ABORT | wxPD_APP_MODAL | wxPD_SMOOTH);
    progressDialog.Show();

    while (!worker.joinable())
    {
        if (!stopSource.stop_requested() && progressDialog.WasCancelled())
        {
            stopSource.request_stop();
            progressDialog.Update(100, "Operation cancelled");
            wxMilliSleep(500);
        }
        wxMilliSleep(100);
        progressDialog.Pulse();
    }
    progressDialog.Destroy();
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
        SetDuplicatesTree();
        SetRefreshOptionState();
    }
}

void MainFrame::OnRefresh(::wxCommandEvent& event)
{
    SetDuplicatesTree();
}

void MainFrame::OnDeleteChecked(::wxCommandEvent& event)
{
    m_treeCtrl->DeleteChecked();
}

void MainFrame::OnClearSelected(::wxCommandEvent& event)
{
}

void MainFrame::OnItemSelected(::wxTreeEvent& event)
{
    wxTreeItemId selectedItem = event.GetItem();
    wxString itemText = m_treeCtrl->GetItemText(selectedItem);
    if (m_previewImage->LoadImage(itemText) == false)
    {
        wxMessageBox(wxString::Format("File could not be previewed: %s", itemText), "Preview Error",
                     wxOK | wxICON_INFORMATION);
        return;
    }
    SetStatusText(wxString::Format("Previewing: %s", itemText));
    m_previewPanel->Layout();
    m_previewPanel->Refresh();
}

void MainFrame::SetRefreshOptionState()
{
    m_toolBar->EnableTool(ID_REFRESH, _targetFolder.has_value());
    if (GetMenuBar())
    {
        GetMenuBar()->Enable(ID_REFRESH, _targetFolder.has_value());
    }
}

void MainFrame::ToggleActionsOverSelectedItems(wxCommandEvent& event)
{
    SetActionsOverSelectedOptionsState(m_treeCtrl->HasSelectedItems());
}

void MainFrame::SetActionsOverSelectedOptionsState(bool isEnabled)
{
    m_toolBar->EnableTool(ID_DELETE_SELECTED, isEnabled);
    m_toolBar->EnableTool(ID_CLEAR_SELECTED, isEnabled);
    if (GetMenuBar())
    {
        GetMenuBar()->Enable(ID_DELETE_SELECTED, isEnabled);
        GetMenuBar()->Enable(ID_CLEAR_SELECTED, isEnabled);
    }
}

} // namespace Deduplicator::Ui