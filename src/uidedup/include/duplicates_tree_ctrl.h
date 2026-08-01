#ifndef __UIDEDUP_DUPLICATES_TREE_CTRL_H__
#define __UIDEDUP_DUPLICATES_TREE_CTRL_H__

#include <deduplib/files_map.h>
#include <filesystem>
#include <wx/treectrl.h>

namespace Deduplicator::Ui
{

namespace fs = std::filesystem;

class DuplicatesTreeCtrl : public wxTreeCtrl
{
  public:
    DuplicatesTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize, long style = wxTR_DEFAULT_STYLE);

    void SetDuplicatesTree(const Deduplicator::ChecksumFilesMap& checksumFilesMap,
                           const std::string& targetFolder);
    bool IsFileItem(const wxTreeItemId& item) const;

  private:
    int m_uncheckedImg;
    int m_checkedImg;

    void OnItemActivated(::wxTreeEvent& event);

    void Populate(const Deduplicator::ChecksumFilesMap& checksumFilesMap,
                  const std::string& targetFolder);
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUP_DUPLICATES_TREE_CTRL_H__