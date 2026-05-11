//
// Created by sashko on 13.03.26.
//

#ifndef UIDEDUP_APPLICATION_H
#define UIDEDUP_APPLICATION_H

#include <filesystem>
#include <wx/wx.h>

namespace Deduplicator::UiLib
{

namespace fs = std::filesystem;

class Application : public ::wxApp
{
  public:
    bool OnInit() override;

  protected:
    static void ConfigureLogging(const fs::path&);
};

} // namespace Deduplicator::UiLib

#endif // UIDEDUP_APPLICATION_H
