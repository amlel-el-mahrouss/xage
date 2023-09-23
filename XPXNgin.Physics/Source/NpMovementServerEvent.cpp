/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 * 
 *			Purpose: Built-in Physics engine for XPX tech.
 *
 * =====================================================================
 */

#include "NpMovementServerEvent.h"

#include <NetworkServerComponent.h>
#include <PxPhysicsAPI.h>

#include <NetworkProtocol.h>
#include <Enums.h>

namespace XPX
{
	namespace Details
	{
		using namespace physx;

		class NpAllocatorTracker final : public PxDefaultAllocator
		{
		public:
			NpAllocatorTracker() = default;
			~NpAllocatorTracker() = default;
			
		public:
			virtual void* allocate(size_t size, 
				const char* typeName,
				const char* filename,
				int line) 
			{
				mTypes.push_back(typeName);
				mLine.push_back(line);
				mFilename.push_back(filename);

				auto ptr = _aligned_malloc(size, 16);;

				mPat.push_back((intptr_t)ptr);

				return ptr;
			}

			virtual void deallocate(void* ptr)
			{
				_aligned_free(ptr);
			}

		public:
			void trace_pointer(void* pointer)
			{
				if (IsValidHeapPtr(pointer))
				{
					std::intptr_t ptr = (intptr_t)pointer;
					std::size_t sz = mPat.size();

					bool reverse = false;

					for (std::size_t i = 0; 
						(reverse ? (i > sz) : (i < sz)); 
						(reverse ? --i : ++i))
					{
						if (mPat[i] > ptr)
						{
							auto sz_clone = sz - 1;
							sz = i;
							i = sz_clone;

							reverse = true;
							
							continue;
						}

						if (mPat[i] < ptr)
						{
							auto sz_clone = sz + 1;
							sz = i;
							i = sz_clone;

							reverse = false;

							continue;
						}

						fmt::print("FILE: {}, LINE: {}, VIRTUAL-MEMORY: {}", mFilename[i], mLine[i], mPat[i]);

						return;
					}
				}
			}

		private:
			std::vector<std::intptr_t> mPat; //! pointer allocation table.
			std::vector<String> mFilename;
			std::vector<String> mTypes;
			std::vector<int> mLine;

		};

		class NpUserErrorCallback : public PxErrorCallback
		{
		public:
			virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file,
				int line)
			{
				switch (code)
				{
				case PxErrorCode::eDEBUG_INFO:
				{
					XPLICIT_INFO(fmt::format("[INFO PhysX] ERR: {}, FILE: {}, LINE: {}", message, file, line));
					break;
				}
				case PxErrorCode::eDEBUG_WARNING:
				{
					XPLICIT_ERROR(fmt::format("[WARN PhysX] ERR: {}, FILE: {}, LINE: {}", message, file, line));
					break;
				}
				default:
				{
					fmt::format("[WARN PhysX] ERR: {}, FILE: {}, LINE: {}", message, file, line);
					break;
				}
				}

			}
		};
	}

	static void xpxSendToClient(ClassComponent* node)
	{
		NetworkPacket repl_packet{};

		repl_packet.channel = XPLICIT_CHANNEL_PHYSICS;
		repl_packet.version = XPLICIT_NETWORK_VERSION;

		repl_packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		repl_packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		repl_packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		repl_packet.pos[XPLICIT_NETWORK_X] = node->pos().X;
		repl_packet.pos[XPLICIT_NETWORK_Y] = node->pos().Y;
		repl_packet.pos[XPLICIT_NETWORK_Z] = node->pos().Z;

		repl_packet.pos_second[XPLICIT_NETWORK_X] = node->scale().X;
		repl_packet.pos_second[XPLICIT_NETWORK_Y] = node->scale().Y;
		repl_packet.pos_second[XPLICIT_NETWORK_Z] = node->scale().Z;

		repl_packet.pos_third[XPLICIT_NETWORK_X] = node->rotation().X;
		repl_packet.pos_third[XPLICIT_NETWORK_Y] = node->rotation().Y;
		repl_packet.pos_third[XPLICIT_NETWORK_Z] = node->rotation().Z;

		String fmt = node->index_as_string("Parent").c_str();

		if (!fmt.empty())
			fmt += ".";

		fmt += node->index_as_string("ClassName").c_str();

		memcpy(repl_packet.additional_data, fmt.c_str(), fmt.size());

		NetworkServerContext::send_all(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"),
			&repl_packet);
	}

	static Details::NpUserErrorCallback gDefaultErrorCallback;
	static Details::NpAllocatorTracker gDefaultAllocatorCallback;

	static physx::PxFoundation* gFoundation = nullptr;
	static physx::PxPhysics* gPhysics = nullptr;
	static physx::PxPvd* gPvd = nullptr;
	static physx::PxCooking* gCooking = nullptr;

	static bool gPhysicsEnabled = false;

	//! pvd default port
#define NP_PHYSX_DEFAULT_PORT (5425)

	NpMovementServerEvent::NpMovementServerEvent() noexcept : mWorldNodes()
	{
		XPLICIT_ASSERT(!gPhysicsEnabled);

		using namespace physx;

		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
			gDefaultErrorCallback);

		if (!gFoundation)
			throw EngineError("PxCreateFoundation failed!");

		// create root object.

		bool recordMemoryAllocations = true;

		gPvd = PxCreatePvd(*gFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(getenv("XPLICIT_SERVER_ADDR"), NP_PHYSX_DEFAULT_PORT, 10);
		gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
			PxTolerancesScale(), recordMemoryAllocations, gPvd);

		if (!gPhysics)
			throw EngineError("PxCreatePhysics failed!");

		gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(gPhysics->getTolerancesScale()));
		
		if (!gCooking)
			throw EngineError("PxCreateCooking failed!");

		if (!PxInitExtensions(*gPhysics, gPvd))
			throw EngineError("PxInitExtensions failed!");
	}

	NpMovementServerEvent::~NpMovementServerEvent() noexcept
	{
		if (gPhysics)
			gPhysics->release();
		
		if (gFoundation)
			gFoundation->release();
	}

	const char* NpMovementServerEvent::name() noexcept { return "NpMovementServerEvent"; }

	void NpMovementServerEvent::operator()()
	{
		for (auto* node : mWorldNodes)
		{
			if (!node)
				continue;

			if (!node->anchor())
			{

			}

			xpxSendToClient(node);
		}
	}

	bool NpMovementServerEvent::insert_node(NpSceneNode node)
	{
		if (node)
		{
			mWorldNodes.push_back(node);
			return true;
		}

		return false;
	}

	bool NpMovementServerEvent::remove_node(NpSceneNode node)
	{
		if (node)
		{
			auto it = std::find(mWorldNodes.cbegin(), mWorldNodes.cend(), node);

			if (it != mWorldNodes.cend())
			{
				mWorldNodes.erase(it);
				return true;
			}
		}

		return false;
	}
}