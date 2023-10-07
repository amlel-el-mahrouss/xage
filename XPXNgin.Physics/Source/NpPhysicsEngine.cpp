/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 * 
 *			Purpose: PhysX backend for the Nplicit ragdoll engine.
 *
 * =====================================================================
 */

#include "NpPhysicsEngine.h"

#include <NetworkServerComponent.h>
#include <PxPhysicsAPI.h>

#include <NetworkProtocol.h>
#include <Enums.h>

#define NP_RIGID_TYPE PxRigidDynamic
#define NP_CREATE_RIGID createRigidDynamic

#define NpDefaultGravity() PxVec3(0.0f, -9.81f, 0.0f)

#define NP_FPS (60.f)
#define NP_DELTATIME (1.0 / NP_FPS)

#define NpGetHowManyWorkers() (8)

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

					for (std::size_t i = 0; i < sz; ++i)
					{
						if (mPat[i] == ptr)
						{
							try
							{
								fmt::print("MEMORY: {}", mPat[i]);
								return;
							}
							catch (...)
							{
								fmt::print("FAILED TO RETRIEVE MEMORY INFORMATION ABOUT THIS POINTER!");
							}
						}
					}
				}
			}

		private:
			std::vector<std::intptr_t> mPat; //! pointer allocation table.

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

	static NetworkServerComponent* gNetwork = nullptr;

	static void xpxSendToClients(XPXAttribute* node)
	{
		NetworkPacket repl_packet{};

		repl_packet.channel = XPLICIT_CHANNEL_3D;
		repl_packet.version = XPLICIT_NETWORK_VERSION;

		repl_packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		repl_packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		repl_packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		repl_packet.pos[0][XPLICIT_NETWORK_X] = node->pos().X;
		repl_packet.pos[0][XPLICIT_NETWORK_Y] = node->pos().Y;
		repl_packet.pos[0][XPLICIT_NETWORK_Z] = node->pos().Z;

		repl_packet.pos[1][XPLICIT_NETWORK_X] = node->scale().X;
		repl_packet.pos[1][XPLICIT_NETWORK_Y] = node->scale().Y;
		repl_packet.pos[1][XPLICIT_NETWORK_Z] = node->scale().Z;

		repl_packet.pos[2][XPLICIT_NETWORK_X] = node->rotation().X;
		repl_packet.pos[2][XPLICIT_NETWORK_Y] = node->rotation().Y;
		repl_packet.pos[2][XPLICIT_NETWORK_Z] = node->rotation().Z;

		repl_packet.pos[3][XPLICIT_NETWORK_X] = node->color().R;
		repl_packet.pos[3][XPLICIT_NETWORK_Y] = node->color().G;
		repl_packet.pos[3][XPLICIT_NETWORK_Z] = node->color().B;
		repl_packet.pos[3][XPLICIT_NETWORK_DELTA] = node->color().A;

		repl_packet.id = node->f_SceneId;

		NetworkServerContext::send_all(gNetwork, &repl_packet);
	}

	static Details::NpUserErrorCallback gDefaultErrorCallback;
	static Details::NpAllocatorTracker	gDefaultAllocatorCallback;

	static physx::PxScene*		gScene				= nullptr;
	static physx::PxPvd*		gPvd				= nullptr;
	static physx::PxFoundation* gFoundation			= nullptr;
	static physx::PxPhysics*	gPhysics			= nullptr;
	static physx::PxCooking*	gCooking			= nullptr;

	static NpPhysicsEvent*		gEventPtr			= nullptr;

	static bool gPhysicsEnabled = false;

	NpPhysicsEvent::NpPhysicsEvent() noexcept 
		: mWorldNodes()
	{
		XPLICIT_ASSERT(!gPhysicsEnabled);

		gPhysicsEnabled = true;
		gEventPtr = this;

		using namespace physx;

		gNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");
		XPLICIT_ASSERT(gNetwork);

		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
			gDefaultErrorCallback);

		if (!gFoundation)
			throw EngineError("PxCreateFoundation failed!");

		gPvd = PxCreatePvd(*gFoundation);

		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		// create root object.

		bool recordMemoryAllocations = true;

		physx::PxTolerancesScale       toleranceScale;
		toleranceScale.length = 100;        // typical length of an object
		toleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
			toleranceScale, recordMemoryAllocations, gPvd);

		if (!gPhysics)
			throw EngineError("PxCreatePhysics failed!");

		gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(gPhysics->getTolerancesScale()));
		
		if (!gCooking)
			throw EngineError("PxCreateCooking failed!");

		if (!PxInitExtensions(*gPhysics, nullptr))
			throw EngineError("PxInitExtensions failed!");

		PxSceneDesc desc(gPhysics->getTolerancesScale());
		desc.gravity = NpDefaultGravity();

		std::size_t numCores = NpGetHowManyWorkers();

		auto cpuDispatcher = PxDefaultCpuDispatcherCreate(numCores);

		if (!cpuDispatcher)
			throw EngineError("PxDefaultCpuDispatcherCreate failed!");

		desc.filterShader = &PxDefaultSimulationFilterShader;
		desc.cpuDispatcher = cpuDispatcher;

		gScene = gPhysics->createScene(desc);

		if (!gScene)
			throw EngineError("gPhysics-|createScene failed! This is probably caused to a bad scene descriptor.");

		physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();

		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}


	}

	NpPhysicsEvent::~NpPhysicsEvent() noexcept
	{
		if (gPhysics)
			gPhysics->release();
		
		if (gFoundation)
			gFoundation->release();

		if (gScene)
			gScene->release();
	}

	const char* NpPhysicsEvent::name() noexcept { return "NpPhysicsEvent"; }

	void NpPhysicsEvent::operator()()
	{
		using namespace physx;

		if (gScene)
		{
			for (auto* node : mWorldNodes)
			{
				if (!node ||
					!node->f_PhysicsDelegate)
					continue;

				NP_RIGID_TYPE* actor = static_cast<NP_RIGID_TYPE*>(node->f_PhysicsDelegate);

				if (!IsValidHeapPtr(actor))
				{
					node->f_PhysicsDelegate = nullptr;
					continue;
				}

				actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, node->anchor());
				actor->addForce(PxVec3(node->pos().X, node->pos().Y, node->pos().Z), PxForceMode::eIMPULSE);
			}

			gScene->simulate(NP_DELTATIME);
			gScene->fetchResults(true);

			for (auto* node : mWorldNodes)
			{
				if (!node ||
					!node->f_PhysicsDelegate)
					continue;

				NP_RIGID_TYPE* actor = static_cast<NP_RIGID_TYPE*>(node->f_PhysicsDelegate);

				PxShape* shape = nullptr;
				actor->getShapes(&shape, 1);

				auto world_pose = actor->getGlobalPose();

				node->pos().X = world_pose.transform(world_pose.p).x;
				node->pos().Y = world_pose.transform(world_pose.p).y;
				node->pos().Z = world_pose.transform(world_pose.p).z;

#ifdef XPLICIT_DEBUG
				std::cout << node->pos().X << std::endl;
				std::cout << node->pos().Y << std::endl;
				std::cout << node->pos().Z << std::endl;
#endif

				if (gNetwork)
					xpxSendToClients(node);
			}
		}
	}

	bool NpPhysicsEvent::insert_node(NpNodePtr node)
	{
		if (node)
		{
			using namespace physx;

			auto dynamic_rigid = gPhysics->NP_CREATE_RIGID(PxTransform(node->pos().X, node->pos().Y, node->pos().Z));
			node->f_PhysicsDelegate = dynamic_rigid;

			if (dynamic_rigid)
			{
				PxReal friction = node->scale().X * node->scale().Y * node->scale().Z;
				auto mat = gPhysics->createMaterial(friction, friction, 1);

				PxBoxGeometry geom(node->scale().X, node->scale().Y, node->scale().Z);

				auto shape = gPhysics->createShape(geom, *mat, true);

				XPLICIT_ASSERT(shape);
				XPLICIT_ASSERT(dynamic_rigid->attachShape(*shape));

				mWorldNodes.push_back(node);
				gScene->addActor(*dynamic_rigid);

				mat->release();
				shape->release();

				return true;
			}

			node->f_PhysicsDelegate = nullptr;

			return false;
		}

		return false;
	}

	bool NpPhysicsEvent::remove_node(NpNodePtr node)
	{
		if (node)
		{
			auto it = std::find(mWorldNodes.cbegin(), mWorldNodes.cend(), node);

			if (it != mWorldNodes.cend())
			{
				using namespace physx;

				NP_RIGID_TYPE* actor = static_cast<NP_RIGID_TYPE*>(node->f_PhysicsDelegate);
				node->f_PhysicsDelegate = nullptr;

				if (actor)
				{
					Thread job([](NP_RIGID_TYPE* actor) {
						while (!actor->isReleasable())
							;
						
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

	bool XPXPhysicsAddGround(NpNodePtr node)
	{
		XPLICIT_ASSERT(node);

		if (!node)
			return false;

		using namespace physx;

		PxReal friction = node->scale().X * node->scale().Y * node->scale().Z;

		auto mat = gPhysics->createMaterial(friction, friction, 1);

		auto plane = PxCreatePlane(*gPhysics, PxPlane(PxVec3(node->scale().X, node->scale().Y, node->scale().Z).getNormalized(), 0.6), *mat);

		mat->release();

		if (plane)
		{
			gScene->addActor(*plane);
			plane->release();

			XPLICIT_INFO("XPXPhysicsAddGround: Create a new ground with success.");

			return true;
		}

		XPLICIT_CRITICAL("XPXPhysicsAddGround: Failed to create a new ground.");
		return false;
	}
}