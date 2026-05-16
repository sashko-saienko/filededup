//
// Created by sashko on 14.03.26.
//
#include <Poco/Logger.h>
#include <main_frame.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

namespace Deduplicator::Ui
{
using Poco::Logger;

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "File Deduplicator", wxDefaultPosition, wxSize(800, 600)),
      m_treeCtrl(nullptr), m_previewText(nullptr)
{
    // Initialize all UI components
    Initialize();
}

void MainFrame::Initialize()
{
    SetupMenuBar();
    SetupMainLayout();
    PopulateSampleData();

    CreateStatusBar();

    // Bind events
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::OnItemSelected, this);
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

    // Add root item
    wxTreeItemId rootId = m_treeCtrl->AddRoot("Duplicates");

    leftSizer->Add(m_treeCtrl, 1, wxEXPAND | wxALL, 5);

    // Right panel for preview
    wxPanel* rightPanel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    // Split the window
    splitter->SplitVertically(leftPanel, rightPanel, 300);

    wxStaticText* previewLabel = new wxStaticText(rightPanel, wxID_ANY, "Preview:");
    m_previewText =
        new wxTextCtrl(rightPanel, wxID_ANY, "Select an item from the list to preview it.",
                       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

    rightSizer->Add(previewLabel, 0, wxALL, 5);
    rightSizer->Add(m_previewText, 1, wxEXPAND | wxALL, 5);
    rightPanel->SetSizer(rightSizer);

    // Main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(splitter, 1, wxEXPAND);
    SetSizer(mainSizer);
}

void MainFrame::PopulateSampleData()
{
    // Add some sample items
    wxTreeItemId rootId = m_treeCtrl->GetRootItem();
    for (int i = 0; i < 10; i++)
    {
        wxString itemText = wxString::Format("File %d.txt", i);
        m_treeCtrl->AppendItem(rootId, itemText);
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

void MainFrame::OnItemSelected(::wxTreeEvent& event)
{
    // Update the preview panel based on the selected item from the tree
    wxTreeItemId selectedItem = event.GetItem();
    wxString fileName = m_treeCtrl->GetItemText(selectedItem);

    Logger::get(LOGGER_NAME).information("Item selected: " + fileName.ToStdString());

    // m_previewText->SetValue(fileName);

    // TODO: Update preview panel with actual file content
}

} // namespace Deduplicator::Ui