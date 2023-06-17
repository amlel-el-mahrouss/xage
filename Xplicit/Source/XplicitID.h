/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: NetworkComponent.h
 *			Purpose: XDP Protocol Client
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit::ID
{
	class XPLICIT_API XplicitID final
	{
	public:
		explicit XplicitID() noexcept
			: mId()
		{
			String creationTime = std::to_string(xplicit_get_epoch());
			String creationId = uuids::to_string(UUIDFactory::version<4>());

			mId =  creationTime + ":" + creationId;
		}

		~XplicitID() = default;

	public:
		XPLICIT_COPY_DEFAULT(XplicitID);

	public:
		bool verify() noexcept
		{
			auto epochEnd = mId.find(":");

			if (epochEnd == String::npos)
				return false;

			String substr = mId.substr(0, epochEnd);

			try
			{
				auto tim = std::atoi(substr.c_str());

				auto now = std::chrono::system_clock::now().time_since_epoch();

				// Use the correct time duration below. Milliseconds could be wrong, see 1)
				auto diff = now - std::chrono::milliseconds(now.count());
				bool is_old = diff > std::chrono::seconds{ 120 };

				if (is_old)
					throw std::runtime_error("check fail");
			}
			catch (...)
			{
				return false;
			}

			String subStrUuid = mId.substr(epochEnd + 1, mId.size());

			if (uuids::uuid::is_valid_uuid(subStrUuid))
				return true;

			return false;
		}

	private:
		String mId;

	};
}