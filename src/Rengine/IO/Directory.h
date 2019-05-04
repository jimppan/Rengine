#ifndef RENGINE_IO_DIRECTORY_H
#define RENGINE_IO_DIRECTORY_H

#include "PCH.h"

namespace Rengine
{
	namespace IO
	{
		namespace FileSystem = std::experimental::filesystem;

		class Path
		{
		public:
			Path();
			Path(const FileSystem::path& path);

			inline bool        HasParentPath() const       { return m_Path.has_parent_path(); }
			inline bool        HasRelativePath() const     { return m_Path.has_relative_path(); }
			inline bool        HasExtension() const        { return m_Path.has_extension(); }
			inline bool        HasFileName() const         { return m_Path.has_filename(); }
			inline bool        HasRootDirectory() const    { return m_Path.has_root_directory(); }
			inline bool        HasRootName() const         { return m_Path.has_root_name(); }
			inline bool        HasRootPath() const         { return m_Path.has_root_path(); }
			inline bool        HasStem() const             { return m_Path.has_stem(); }
			
			inline bool        IsRelative() const          { return m_Path.is_relative(); }
			inline bool        IsAbsolute() const          { return m_Path.is_absolute(); }

			inline Path        ParentPath() const          { return m_Path.parent_path(); }
			inline Path        RootPath() const            { return m_Path.root_path(); }

			inline Path        RootDirectory() const       { return m_Path.root_directory(); }
			inline std::string FullPath() const            { return m_Path.string(); }
			inline std::string Extension() const           { return m_Path.extension().string(); }
			inline std::string FileName() const            { return m_Path.filename().string(); }

			FileSystem::path m_Path;
		};

		class Directory
		{
		public:
			Directory();
			Directory(const std::string& path);
			~Directory();

			bool                           Open(const std::string& path);
			inline std::string             GetPath() const                  { return m_szPath; }

			bool Iterate();
			Path GetIteratorPath();
			
		private:
			std::string m_szPath;
			std::experimental::filesystem::directory_iterator m_Iterator;

			bool m_bIteratedBegin = false;
		};
	}
}

#endif // RENGINE_IO_DIRECTORY_H