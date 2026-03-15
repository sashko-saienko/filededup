//
// Created by sashko on 14.03.26.
//
#include "uideduplib/main_frame.h"
#include <Poco/Logger.h>
#include <wx/wx.h>
#include <iostream>

using Poco::Logger;

MainFrame::MainFrame()
  : wxFrame(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize)
{
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_HELLO, "&Hello...\tCtrl+H", "Hello world example!");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Hello, world!");

    Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_HELLO);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void
MainFrame::OnHello(wxCommandEvent& _)
{
    Logger::get(LOGGER_NAME).information("Hello world!");
    std::cerr << "Hello world!" << std::endl;
}

void
MainFrame::OnExit(wxCommandEvent& _)
{
    Close(true);
}

void
MainFrame::OnAbout(wxCommandEvent& _)
{
    wxMessageBox(
      "This is Hello World! example in wxWidgets", "About uidedup", wxOK | wxICON_INFORMATION);
}