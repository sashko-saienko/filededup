//
// Created by sashko on 14.03.26.
//
#include <Poco/Logger.h>
#include <deduplib/duplicates_finder.h>
#include <format>
#include <main_frame.h>
#include <reusablelib/time_helpers.h>
#include <string>
#include <wx/filefn.h>
#include <wx/gdicmn.h>
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
      m_treeCtrl(nullptr), m_previewImage(nullptr), m_previewPanel(nullptr), m_previewSizer(nullptr)
{
    // Initialize all UI components
    Initialize();
}

void MainFrame::Initialize()
{
    SetupMenuBar();
    SetupMainLayout();
    PopulateDuplicatesTree();

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

    m_previewLabel =
        new wxStaticText(m_previewPanel, wxID_ANY, "Select an item from the list to preview it.",
                         wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    m_previewImage = new wxStaticBitmap(m_previewPanel, wxID_ANY, wxBitmap());
    m_previewImage->Hide();

    m_previewSizer->AddStretchSpacer();
    m_previewSizer->Add(m_previewLabel, 1, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    m_previewSizer->Add(m_previewImage, 1, wxEXPAND | wxALL | wxCENTER, 5);
    m_previewSizer->AddStretchSpacer();

    // Main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(splitter, 1, wxEXPAND);
    SetSizer(mainSizer);
}

void MainFrame::PopulateDuplicatesTree()
{
    using ReusableLib::ObservableStopwatch;

    ObservableStopwatch sw([&](const long seconds) {
        Logger::get(LOGGER_NAME).information("Completed duplicates seach in %ld(s)", seconds);
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

    wxTreeItemId rootId = m_treeCtrl->GetRootItem();

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

    previewImage(fileName);
}

void MainFrame::previewImage(const wxString& imagePath)
{
    // Hide text control and show image control
    m_previewLabel->Show();
    m_previewImage->Hide();

    // Load and display the image
    if (wxFileExists(imagePath) == false)
    {
        auto errorMessage = wxString::Format("Image file does not exist: %s", imagePath);
        wxMessageBox(errorMessage, "Error finding image to open it for preview", wxOK | wxCENTER,
                     this);
        return;
    }

    wxImage image;
    if (image.LoadFile(imagePath) == false)
    {
        auto errorMessage = wxString::Format("Image file not supported: %s", imagePath);
        wxMessageBox(errorMessage, "Error opening image for preview", wxOK | wxCENTER, this);
        return;
    }

    wxBitmap bitmap(image);
    m_previewImage->SetBitmap(bitmap);

    m_previewLabel->Hide();
    m_previewImage->Show();

    m_previewPanel->Layout();
    m_previewPanel->Refresh();
}

} // namespace Deduplicator::Ui