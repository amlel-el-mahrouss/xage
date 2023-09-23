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

 //! pvd default port
#define NP_PHYSX_DEFAULT_PORT (5425)
#define XPX_DEFAULT_GRAVITY PxVec3(0, -9.81f, 0)

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
				if (typeName)
					mTypes.push_back(typeName);

				mLine.push_back(line);

				if (filename)
					mFilename.push_back(filename);

				auto ptr = _aligned_malloc(size, 16);

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

						try
						{
							fmt::print("FILE: {}, LINE: {}, VIRTUAL-MEMORY: {}", mFilename[i], mLine[i], mPat[i]);
						}
						catch (...)
						{

						}

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
					fmt::print("[PhysX] ERR: {}, FILE: {}, LINE: {}\n", message, file, line);
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


		repl_packet.pos_fourth[XPLICIT_NETWORK_X] = node->color().R;
		repl_packet.pos_fourth[XPLICIT_NETWORK_Y] = node->color().G;
		repl_packet.pos_fourth[XPLICIT_NETWORK_Z] = node->color().B;
		repl_packet.pos_fourth[XPLICIT_NETWORK_Z + 1] = node->color().A;

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

	static physx::PxScene* gScene = nullptr;

	static physx::PxFoundation* gFoundation = nullptr;
	static physx::PxPhysics* gPhysics = nullptr;
	static physx::PxPvd* gPvd = nullptr;
	static physx::PxCooking* gCooking = nullptr;

	static bool gPhysicsEnabled = false;

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

		PxSceneDesc desc(gPhysics->getTolerancesScale());
		desc.gravity = XPX_DEFAULT_GRAVITY;

		SYSTEM_LOGICAL_PROCESSOR_INFORMATION info;
		RtlZeroMemory(&info, sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));

		DWORD len = 0U;
		GetLogicalProcessorInformation(&info, &len);

		std::size_t numCores = (len / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));

		auto cpuDispatcher = PxDefaultCpuDispatcherCreate(numCores);

		if (!cpuDispatcher)
			throw EngineError("PxDefaultCpuDispatcherCreate failed!");

		desc.filterShader = &PxDefaultSimulationFilterShader;
		desc.cpuDispatcher = cpuDispatcher;

		gScene = gPhysics->createScene(desc);

		if (!gScene)
			throw EngineError("createScene failed! Refer to XPX support for help.");
	}

	NpMovementServerEvent::~NpMovementServerEvent() noexcept
	{
		if (gPhysics)
			gPhysics->release();
		
		if (gFoundation)
			gFoundation->release();
	}

	const char* NpMovementServerEvent::name() noexcept { return "NpMovementServerEvent"; }

	static physx::PxReal gDeltaTime = 1;

	void NpMovementServerEvent::operator()()
	{
		using namespace physx;

		if (gScene)
		{
			gDefaultAllocatorCallback.trace_pointer(gScene);

			gScene->simulate(gDeltaTime);
			++gDeltaTime;

			gScene->fetchResults(true);

			for (auto* node : mWorldNodes)
			{
				if (!node)
					continue;

				PxRigidStatic* actor = static_cast<PxRigidStatic*>(node->PhysicsDelegate);

				actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, node->anchor());

				if (actor)
				{
					PxVec3 input(node->pos().X, node->pos().Y, node->pos().Z);

					auto pos = actor->getGlobalPose().transform(input);

					node->pos().X = pos.x;
					node->pos().Y = pos.y;
					node->pos().Z = pos.z;

					xpxSendToClient(node);
				}
			}
		}
	}

	bool NpMovementServerEvent::insert_node(NpSceneNode node)
	{
		if (node)
		{
			using namespace physx;

			auto static_rigid = gPhysics->createRigidStatic(PxTransform(node->pos().X, node->pos().Y, node->pos().Z));;
			XPLICIT_ASSERT(static_rigid);

			static_rigid->setName(node->name());

			static_rigid->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, node->anchor());

			node->PhysicsDelegate = static_rigid;

			// ???
			if (auto actor_ptr = dynamic_cast<PxActor*>(static_rigid);
				actor_ptr &&
				actor_ptr == static_rigid)
			{
				mWorldNodes.push_back(node);
				gScene->addActor(*actor_ptr);
				return true;
			}

			static_rigid->release();
			node->PhysicsDelegate = nullptr;
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
				using namespace physx;

				PxActor* actor = static_cast<PxActor*>(node->PhysicsDelegate);

				if (actor)
				{
					Thread job([](PxActor* actor) {
						if (actor->isReleasable())
							actor->release();
						}, actor);

					job.detach();
				}

				mWorldNodes.erase(it);
				return true;
			}
		}

		return false;
	}
}