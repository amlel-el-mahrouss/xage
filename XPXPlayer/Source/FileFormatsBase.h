/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <fstream>

namespace XPX::Renderer
{
	class IFileReader
	{
	public:
		explicit IFileReader(const char* path)
			: mIf(fopen(path, "rb"))
		{}

		virtual ~IFileReader() noexcept
		{
			if (mIf)
				fclose(mIf);
		}

	public:
		XPLICIT_COPY_DEFAULT(IFileReader);

	public:
		const char* format() { return "*/none"; }

	protected:
		FILE* mIf;

	};

	enum
	{
		FILE_FORMAT_RIFF,
		FILE_FORMAT_PBM, // XAR CONTAINER FOR PBR MODELS /w TEXTURES.
		FILE_FORMAT_ROXML, // Mesh RoXML.
	};
}