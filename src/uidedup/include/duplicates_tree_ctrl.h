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

    void SetDuplicatesTree(const std::optional<fs::path>& targetFolder);
    bool IsFileItem(const wxTreeItemId& item) const;

    int GetUncheckedImageIndex() const
    {
        return m_uncheckedImg;
    }
    int GetCheckedImageIndex() const
    {
        return m_checkedImg;
    }

  private:
    int m_uncheckedImg;
    int m_checkedImg;
    Deduplicator::ChecksumFilesMap checksumFilesMap;

    wxTreeItemId SetupOrCleanupRootElement(const std::optional<fs::path>& targetFolder);
    void PopulateTree(const wxTreeItemId& rootId);
    bool ScanTargetFolder(const std::optional<fs::path>& targetFolder);
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUP_DUPLICATES_TREE_CTRL_H__