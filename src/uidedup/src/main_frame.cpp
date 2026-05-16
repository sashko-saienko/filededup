//
// Created by sashko on 14.03.26.
//
#include <Poco/Logger.h>
#include <main_frame.h>
#include <wx/listbase.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/wx.h>

namespace Deduplicator::Ui
{
using Poco::Logger;

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "File Deduplicator", wxDefaultPosition, wxSize(800, 600))
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

    CreateStatusBar();

    // Create main splitter window
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
    splitter->SetMinimumPaneSize(200);

    // Left panel with list control
    wxPanel* leftPanel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    wxListCtrl* listCtrl = new wxListCtrl(leftPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                          wxLC_REPORT | wxLC_SINGLE_SEL);

    // Add columns to list control
    listCtrl->InsertColumn(0, "File Name", wxLIST_FORMAT_LEFT, 200);
    listCtrl->InsertColumn(1, "Size", wxLIST_FORMAT_RIGHT, 100);
    listCtrl->InsertColumn(2, "Type", wxLIST_FORMAT_LEFT, 100);

    // Add some sample items
    for (int i = 0; i < 10; i++)
    {
        wxListItem item;
        item.SetId(i);
        item.SetText(wxString::Format("File %d.txt", i));
        listCtrl->InsertItem(item);
        listCtrl->SetItem(i, 1, wxString::Format("%d KB", 100 + i * 10));
        listCtrl->SetItem(i, 2, "Text");
    }

    leftSizer->Add(listCtrl, 1, wxEXPAND | wxALL, 5);
    leftPanel->SetSizer(leftSizer);

    // Right panel for preview
    wxPanel* rightPanel = new wxPanel(splitter, wxID_ANY);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* previewLabel = new wxStaticText(rightPanel, wxID_ANY, "Preview:");
    wxTextCtrl* previewText =
        new wxTextCtrl(rightPanel, wxID_ANY, "Select an item from the list to preview it.",
                       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

    rightSizer->Add(previewLabel, 0, wxALL, 5);
    rightSizer->Add(previewText, 1, wxEXPAND | wxALL, 5);
    rightPanel->SetSizer(rightSizer);

    // Split the window
    splitter->SplitVertically(leftPanel, rightPanel, 300);

    // Main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(splitter, 1, wxEXPAND);
    SetSizer(mainSizer);

    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_LIST_ITEM_SELECTED, &MainFrame::OnItemSelected, this);
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

void MainFrame::OnItemSelected(::wxListEvent& event)
{
    // Update the preview panel based on the selected item from the list
    long selectedIndex = event.GetIndex();
    wxString fileName = event.GetText();

    Logger::get(LOGGER_NAME)
        .information("Item selected: " + std::to_string(selectedIndex) + " - " +
                     fileName.ToStdString());

    // TODO: Update preview panel with actual file content
}

} // namespace Deduplicator::Ui