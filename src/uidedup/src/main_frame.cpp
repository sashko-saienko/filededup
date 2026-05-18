//
// Created by sashko on 14.03.26.
//
#include <Poco/Logger.h>
#include <main_frame.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

namespace Deduplicator::Ui
{
using Poco::Logger;

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "File Deduplicator", wxDefaultPosition, wxSize(800, 600)),
      m_treeCtrl(nullptr), m_previewText(nullptr), m_previewImage(nullptr), m_previewPanel(nullptr),
      m_previewSizer(nullptr)
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
    wxTreeItemId rootId = m_treeCtrl->AddRoot("Checksum duplicates");

    leftSizer->Add(m_treeCtrl, 1, wxEXPAND | wxALL, 5);

    // Right panel for preview
    m_previewPanel = new wxPanel(splitter, wxID_ANY);
    m_previewSizer = new wxBoxSizer(wxVERTICAL);
    m_previewPanel->SetSizer(m_previewSizer);

    // Split the window
    splitter->SplitVertically(leftPanel, m_previewPanel, 300);

    // Create both controls but only show text initially
    m_previewText =
        new wxTextCtrl(m_previewPanel, wxID_ANY, "Select an item from the list to preview it.",
                       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    m_previewImage = new wxStaticBitmap(m_previewPanel, wxID_ANY, wxBitmap());

    // Add text control to sizer initially
    m_previewSizer->Add(m_previewText, 1, wxEXPAND | wxALL, 5);

    // Main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(splitter, 1, wxEXPAND);
    SetSizer(mainSizer);
}

void MainFrame::PopulateSampleData()
{
    // Add some sample items
    wxTreeItemId rootId = m_treeCtrl->GetRootItem();

    // Add text files
    for (int i = 0; i < 5; i++)
    {
        wxString itemText = wxString::Format("File %d.txt", i);
        m_treeCtrl->AppendItem(rootId, itemText);
    }

    // Add image files
    m_treeCtrl->AppendItem(rootId, "image1.png");
    m_treeCtrl->AppendItem(rootId, "photo.jpg");
    m_treeCtrl->AppendItem(rootId, "screenshot.jpeg");

    // Add some other file types
    m_treeCtrl->AppendItem(rootId, "document.pdf");
    m_treeCtrl->AppendItem(rootId, "data.csv");
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

    // Determine file type and show appropriate preview
    if (fileName.EndsWith(".txt") || fileName.EndsWith(".cpp") || fileName.EndsWith(".h"))
    {
        ShowTextPreview("This would show the content of " + fileName);
    }
    else if (fileName.EndsWith(".png") || fileName.EndsWith(".jpg") || fileName.EndsWith(".jpeg") ||
             fileName.EndsWith(".bmp"))
    {
        ShowImagePreview(fileName);
    }
    else
    {
        ShowTextPreview("Unsupported file type: " + fileName);
    }
}

void MainFrame::ShowTextPreview(const wxString& content)
{
    // Clear the preview sizer
    m_previewSizer->Clear();

    // Show text control and hide image control
    m_previewText->Show();
    m_previewImage->Hide();

    // Set the text content
    m_previewText->SetValue(content);

    // Add text control to sizer and layout
    m_previewSizer->Add(m_previewText, 1, wxEXPAND | wxALL, 5);
    m_previewPanel->Layout();
    m_previewPanel->Refresh();
}

void MainFrame::ShowImagePreview(const wxString& imagePath)
{
    // Clear the preview sizer
    m_previewSizer->Clear();

    // Hide text control and show image control
    m_previewText->Hide();
    m_previewImage->Hide();

    // Load and display the image
    if (wxImage image; wxFileExists(imagePath) && image.LoadFile(imagePath))
    {
        wxBitmap bitmap(image);
        m_previewImage->SetBitmap(bitmap);
    }
    else
    {
        auto errorMessage =
            wxString::Format("Image file not found or not supported: %s", imagePath);
        wxMessageBox(errorMessage, "Error opening image for preview", wxOK | wxCENTRE, this);
    }

    m_previewImage->Show();

    // Add image control to sizer and layout
    m_previewSizer->Add(m_previewImage, 1, wxEXPAND | wxALL, 5);

    m_previewPanel->Layout();
    m_previewPanel->Refresh();
}

void MainFrame::ClearPreview()
{
    m_previewSizer->Clear();
    m_previewText->SetValue("");
    m_previewImage->SetBitmap(wxBitmap());
    m_previewSizer->Add(m_previewText, 1, wxEXPAND | wxALL, 5);
    m_previewPanel->Layout();
    m_previewPanel->Refresh();
}

} // namespace Deduplicator::Ui