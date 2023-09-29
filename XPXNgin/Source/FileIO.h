/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"
#include "Root.h"

#include <fstream>

namespace XPX
{
	class IFileReader
	{
	public:
		explicit IFileReader(const char* path)
			: mIf(fopen(path, "rb"))
		{
			XPLICIT_ASSERT(mIf);
		}

		virtual ~IFileReader() noexcept
		{
			if (mIf)
				fclose(mIf);
		}

		const String operator>>(const std::size_t& sz) noexcept
		{
			String buf;
			buf.reserve(sz);

			fread(buf.data(), sz, 2, mIf);
		
			return buf;
		}

	public:
		XPLICIT_COPY_DEFAULT(IFileReader);

	public:
		const char* xmime() { return "format:*/none"; }

	protected:
		FILE* mIf;

	};

	class IFileWriter final
	{
	public:
		explicit IFileWriter(const char* path)
			: mIf(fopen(path, "rb"))
		{
			XPLICIT_ASSERT(mIf);
		}

		virtual ~IFileWriter() noexcept
		{
			if (mIf)
				fclose(mIf);
		}

		bool operator<<(const String& str) noexcept
		{
			return fwrite(str.data(), str.size() - 1, 2, mIf) == str.size() - 1;
		}

	public:
		XPLICIT_COPY_DEFAULT(IFileWriter);

	public:
		const char* xmime() { return "format:*/none"; }

	protected:
		FILE* mIf;

	};
}
