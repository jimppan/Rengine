#ifndef RENGINE_IO_H
#define RENGINE_IO_H

#include "PCH.h"
#include "Rengine/IO/Directory.h"

namespace Rengine
{
	namespace IO
	{
		namespace FileSystem = std::experimental::filesystem;

		static bool ReadRaw(const std::string& path, std::string* out, size_t start = 0, size_t end = 0)
		{
			std::ios_base::openmode mode = std::ios::binary;
			if (end == 0)
				mode |= std::ios::ate;

			std::ifstream inFile(path, mode);
			if (!inFile.is_open())
				return false;

			size_t length;
			if (end == 0)
				length = (size_t)inFile.tellg() - start;
			else
				length = end - start;
			inFile.seekg(start);

			out->resize(length);
			inFile.read(&(*out)[0], length);

			inFile.close();
			return true;
		}

		static bool ReadText(const std::string& path, std::string* out)
		{
			std::ifstream inFile(path);
			if (!inFile.is_open())
				return false;

			inFile.seekg(0, std::ios::end);
			out->reserve((size_t)inFile.tellg());
			inFile.seekg(0, std::ios::beg);
			out->assign((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

			inFile.close();
			return true;
		}

		static inline Path            GetApplicationPath(const std::string& path) { return FileSystem::current_path(); }

		static inline bool            IsDirectory(const std::string& path)        { return FileSystem::is_directory(path); }
		static inline bool            IsRegularFile(const std::string& path)      { return FileSystem::is_regular_file(path); }
		static inline bool            IsBlockFile(const std::string& path)        { return FileSystem::is_block_file(path); }
		static inline bool            IsTextFile(const std::string& path)         { return FileSystem::is_character_file(path); }
		static inline bool            IsEmpty(const std::string& path)            { return FileSystem::is_empty(path); }

		static inline bool            CreateDir(const std::string& path)          { return FileSystem::create_directory(path); }
		static inline bool            CreateDirs(const std::string& path)         { return FileSystem::create_directories(path); }

		static inline uint64_t        FileSize(const std::string& path)           { return FileSystem::file_size(path); }
		static inline bool            Remove(const std::string& path)             { return FileSystem::remove(path); }
		
		static inline bool            IsEquivalent(const std::string& path1, const std::string& path2) { return FileSystem::equivalent(path1, path2); }
		static inline void            Rename(const std::string& path1, const std::string& path2)       { FileSystem::rename(path1, path2); }
		
	}
}

#endif // RENGINE_IO_H