/*
 * =====================================================================
 *
 *			XAGE
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"
#include "Root.h"

#include <cstdio>

namespace XPX
{
	class XPLICIT_API IFileBase
	{
	public:
		IFileBase(FILE* fp) : mIf(fp) {}
		virtual ~IFileBase() = default;

	public:
		std::size_t tell() noexcept { return ftell(mIf); }

		enum
		{
			seek_beg = SEEK_SET,
			sek_cur = SEEK_CUR,
			seek_end = SEEK_END
		};

		void seek(const std::size_t& seek_at) noexcept
		{
			fseek(mIf, seek_at, SEEK_SET);
		}

		operator bool() noexcept { return mIf && ferror(mIf) == 0; }

	public:
		const String xmime() noexcept { return fmt::format("format:{}/{}", fNamespace, fName); }

	public:
		String fNamespace{ "*" };
		String fName{ "txt" };

	protected:
		FILE* mIf;

	};

	class XPLICIT_API IFileReader final : public IFileBase
	{
	public:
		explicit IFileReader(const char* path)
			: IFileBase(fopen(path, "rb"))
		{
			XPLICIT_ASSERT(mIf);
		}

		~IFileReader() noexcept override
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

	};

	class XPLICIT_API IFileWriter final : public IFileBase
	{
	public:
		explicit IFileWriter(const char* path)
			: IFileBase(fopen(path, "wb"))
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

	};
}
