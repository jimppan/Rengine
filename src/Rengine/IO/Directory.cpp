#include "PCH.h"
#include "Directory.h"

namespace Rengine
{
	namespace IO
	{
		/* PATH */
		Path::Path(const FileSystem::path& path) :
			m_Path(path)
		{

		}

		Path::Path():
			m_Path(FileSystem::path())
		{
		}

		/* DIRECTORY */
		Directory::Directory()
		{
		}

		Directory::Directory(const std::string& path)
		{
			Open(path);
		}

		Directory::~Directory()
		{
		}

		bool Directory::Open(const std::string& path)
		{
			if (!FileSystem::is_directory(path))
				return false;

			m_bIteratedBegin = false;
			m_szPath = path;
			m_Iterator = FileSystem::directory_iterator(path);
			return true;
		}

		bool Directory::Iterate()
		{
			if (m_Iterator == FileSystem::end(m_Iterator))
				return false;

			if (!m_bIteratedBegin && m_Iterator == FileSystem::begin(m_Iterator))
			{
				m_bIteratedBegin = true;
				return true;
			}

			m_Iterator++;
			return m_Iterator != FileSystem::end(m_Iterator);
		}

		Path Directory::GetIteratorPath()
		{
			if (!m_bIteratedBegin || m_Iterator == FileSystem::end(m_Iterator))
			{
				m_bIteratedBegin = true;
				return Path();
			}

			return Path((*m_Iterator).path());
		}
	}
}

