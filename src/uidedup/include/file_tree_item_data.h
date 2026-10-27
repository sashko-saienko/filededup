// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Sashko

#ifndef __UIDEDUP_FILE_TREE_ITEM_DATA_H__
#define __UIDEDUP_FILE_TREE_ITEM_DATA_H__

#include <Poco/File.h>
#include <wx/treectrl.h>

namespace Deduplicator::Ui
{

class FileTreeItemData : public wxTreeItemData
{
  public:
    FileTreeItemData(Poco::File&& file);
    const Poco::File& GetFile() const;
    void DeleteFile();

  private:
    Poco::File m_file;
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUP_FILE_TREE_ITEM_DATA_H__
