/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: DownloadProtocol.cpp
 *			Purpose: Xplicit Stream Protocol
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "EngineStreamingService.h"

/* checking algorithms. */
#include "Crc32.h"

namespace Xplicit::Network
{
	FileStreamWriter::FileStreamWriter(const char* bytes, size_t len)
		: mByteList()
	{
		this->write(bytes, len);
	}

	FileStreamWriter::~FileStreamWriter() {}

	int FileStreamWriter::write(const char* bytes, size_t len)
	{
		if (!bytes ||
			len < 1)
			return 1;

		mByteList.clear();
		mByteList.reserve(len);

		for (size_t i = 0; i < mByteList.size(); ++i)
		{
			mByteList[i] = bytes[i];
		}

		return 0;
	}

	const char* FileStreamWriter::get() noexcept
	{
		if (mByteList.empty())
			return "(null)";

		return mByteList.data();
	}

	size_t FileStreamWriter::size() noexcept { return mByteList.size(); }

	/* Task stream class */

	FileTaskStream::FileTaskStream() 
		: mReady(false), mFileList() 
	{}

	FileTaskStream::~FileTaskStream() = default;

	void FileTaskStream::add(FileStreamWriter* fsw)
	{
		if (fsw)
			mFileList.push_back(fsw);
	}

	bool FileTaskStream::remove(FileStreamWriter* fsw)
	{
		if (fsw)
		{
			auto it = std::find(mFileList.cbegin(), mFileList.cend(), fsw);

			if (it != mFileList.cend())
			{
				mFileList.erase(it);
				return true;
			}
		}

		return false;
	}

	void FileTaskStream::operator()(Socket& socket, const bool isCompressed)
	{
		if (!mReady) 
			return;

		/* dsc is nrz encoded. */
		/* 4 bytes alignement */

#define XPLICIT_PACKET_PAD()\
			data.push_back(0);\
			data.push_back(0);\
			data.push_back(0);\
			data.push_back(0)\



		/* check socket */
		if (socket != XPLICIT_SOCKET_ERROR)
		{
			/* prepare buffers */
			std::vector<std::int32_t> data, data_tmp;

			/* push magic numbers*/
			data.push_back(XPLICIT_STREAM_MAG_0);
			data.push_back(XPLICIT_STREAM_MAG_1);
			data.push_back(XPLICIT_STREAM_MAG_2);

			XPLICIT_PACKET_PAD();

			data.push_back(isCompressed);

			XPLICIT_PACKET_PAD();

			data.push_back(XPLICIT_STREAM_VERSION);

			XPLICIT_PACKET_PAD();

			data.push_back('\r');
			data.push_back('\n');

			XPLICIT_PACKET_PAD();

			Thread worker([&]() -> void
				{
					for (size_t file_index = 0; file_index < mFileList.size(); ++file_index)
					{
						for (size_t data_index = 0; data_index < mFileList[file_index]->size(); ++data_index)
						{
							data_tmp.push_back(mFileList[file_index]->get()[data_index]);
						}


						XPLICIT_PACKET_PAD();

						/* end of file section */
						data.push_back('\r');
						data.push_back('\n');
						data.push_back('\r');
						data.push_back('\n');

						XPLICIT_PACKET_PAD();
					}
			});

			std::int32_t crc = xplicit_crc32(reinterpret_cast<char*>(data_tmp.data()), data_tmp.size());

			data.push_back(crc);

			XPLICIT_PACKET_PAD();

			for (size_t i = 0; i < data_tmp.size(); ++i)
			{
				data.push_back(data_tmp[i]);
			}

			XPLICIT_PACKET_PAD();

			// wait for the worker thread to finish.
			if (worker.joinable())
				worker.join();

			socket.send<std::int32_t*>(data_tmp.data(), data.size());

			/* Clear status */
			mReady = false;
		}
	}

	bool FileTaskStream::ready() noexcept { return mReady; }

	FileTaskStream::operator bool() noexcept { return this->ready(); }

	void FileTaskStream::set(const bool ready) noexcept { mReady = true; }
}