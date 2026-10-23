//
// Created by sashko on 13.03.26.
//

#ifndef UIDEDUP_APPLICATION_H
#define UIDEDUP_APPLICATION_H

#include <filesystem>
#include <wx/wx.h>

namespace Deduplicator::Ui
{

namespace fs = std::filesystem;

class Application : public ::wxApp
{
  public:
    bool OnInit() override;
    static wxImage GetCheckedImage();
    static wxImage GetUncheckedImage();
    static wxIcon GetApplicationIcon();

  protected:
    static void ConfigureLogging(const fs::path&);
    static wxString GetResourcesPath();
};

wxDECLARE_APP(Application);

} // namespace Deduplicator::Ui

#endif // UIDEDUP_APPLICATION_H
