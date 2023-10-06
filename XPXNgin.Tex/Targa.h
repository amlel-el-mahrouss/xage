/*
 * =====================================================================
 *
 *			XAGE.Tex
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			Purpose: XIFF container header.
 *
 * =====================================================================
 */

#pragma once

namespace XPX::Renderer
{
	struct TargaHeader
	{
		unsigned char spec[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char id_length;
	};

	struct TargaResultHeader
	{
		TargaHeader f_sHeader{ };
		BOOL f_bLoaded{ false };
		BYTE* f_pImage{ nullptr };

		operator bool() { return f_bLoaded; }
	};

	inline TargaResultHeader LoadTarga32(const char* filename)
	{
		int error = 0;
		int bpp = 0;
		int imageSize = 0;
		int index = 0;
		int i = 0;
		int j = 0;
		int k = 0;

		FILE* filePtr = nullptr;
		unsigned int count = 0U;
		TargaHeader targaFileHeader;
		unsigned char* targaImage = nullptr;

		//! Open the targa file for reading in binary.
		//! We use fopen_s, so that MSVC doesn't yell at us.
		error = fopen_s(&filePtr, filename, "rb");

		if (error != 0)
			return {};

		// Read in the targa header.
		count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
		
		if (count != 1)
		{
			fclose(filePtr);
			return {};
		}

		bpp = targaFileHeader.bpp;

		// Check that it is 32 bit and not 24 bit.
		if (bpp != 32)
		{
			fclose(filePtr);
			return {};
		}

		// Calculate the size of the 32 bit image data.
		imageSize = targaFileHeader.width * targaFileHeader.height * 4;

		// Allocate memory for the targa image data.
		targaImage = new BYTE[imageSize];

		// Read in the targa image data.
		count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
		
		if (count != imageSize)
		{
			delete[] targaImage;
			fclose(filePtr);

			return {};
		}

		// Close the file.
		error = fclose(filePtr);

		if (error != 0)
		{
			delete[] targaImage;

			return {};
		}

		// Allocate memory for the targa destination data.
		BYTE* targaData = new unsigned char[imageSize];

		// Initialize the index into the targa destination data array.
		index = 0;

		// Initialize the index into the targa image data.
		k = imageSize - (targaFileHeader.width * 4);

		// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order.
		for (j = 0; j < targaFileHeader.height; j++)
		{
			for (i = 0; i < targaFileHeader.width; i++)
			{
				targaData[index + 0] = targaImage[k + 2];  // Red.
				targaData[index + 1] = targaImage[k + 1];  // Green.
				targaData[index + 2] = targaImage[k + 0];  // Blue
				targaData[index + 3] = targaImage[k + 3];  // Alpha

				// Increment the indexes into the targa data.
				k += 4;
				index += 4;
			}

			// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
			k -= (targaFileHeader.width * 8);
		}

		TargaResultHeader image{};
		image.f_pImage = targaImage;
		image.f_sHeader = targaFileHeader;
		image.f_bLoaded = true;

		return image;
	}
}
