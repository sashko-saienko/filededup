# FileDedup - File Deduplication Tool

**C++23** cross-platform duplicate image file finder and remover with **GUI** and **CLI** interfaces.

## Overview

FileDedup identifies duplicate files by computing Adler-32 checksums and provides two interfaces:
- **uidedup**: wxWidgets-based GUI application
- **condedup**: Console/CLI application

Both share a common core library (`deduplib`) for duplicate detection.

### CMake Features
- **Static analysis**: CppCheck integration
- **Code coverage**: gcov instrumentation (Debug builds)
- **Code formatting**: clang-format integration
- **Link-time optimization**: LTO enabled
- **Include-what-you-use**: IWYU support

## Usage

### Console (condedup)

```bash
# Find duplicates (dry-run)
condedup --target-dir /path/to/directory

# Find and delete duplicates, keep oldest
condedup --target-dir /path/to/directory --delete

# Find and delete duplicates, keep newest
condedup --target-dir /path/to/directory --delete --keep-policy newest

# Show help
condedup --help
```

### GUI (uidedup)

1. Launch the application
2. Click "Select Folder" or press Ctrl+O
3. Choose a directory to scan
4. View duplicates in the tree
5. Use toolbar buttons or menu items:
   - **Refresh** (F5): Rescan the directory
   - **Delete Selected** (Del): Remove selected duplicate files
   - **Clear Selected**: Deselect all items
   - **Select Oldest/Newest**: Auto-select oldest or newest files in each duplicate group

## Platform Support
- **Windows**: Full support (wxWidgets, Poco)
- **macOS**: Full support (wxWidgets, Poco)
- **Linux**: Full support (wxWidgets, Poco)

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

## Metadata
- **Version**: 0.0.1
- **Namespace**: Deduplicator (Console), Deduplicator::Ui (GUI), Deduplicator::Console (Console)
- **Logger**: Poco logging with per-module logger names
