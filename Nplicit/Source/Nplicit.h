/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Nplicit.h
 *			Purpose: Nplicit standard includes
 *
 * =====================================================================
 */

#pragma once

#include "Config.h"
#include "NMath.h" /* NPlicit math library */

#include <Instance.h>
#include <Avx.h>

namespace Xplicit::Physics
{
	template <typename TypeFloat = float>
	class NPLICIT_API Vector final
	{
	public:
		TypeFloat X, Y, Z;

	public:
		Vector() = default;
		~Vector() = default;

		explicit Vector(TypeFloat x = 0, TypeFloat y = 0, TypeFloat z = 0) noexcept
		{
			this->add(x, y, z);
		}

		Vector& operator=(const Vector&) = delete;
		Vector(const Vector&) = delete;

		Vector& add(TypeFloat x = 0.f, TypeFloat y = 0.f, TypeFloat z = 0.f) noexcept
		{
#ifdef NPLICIT_USE_AVX
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_add_pd(x1, x2);
#else
			this->X += x;
			this->Y += y;
			this->Z += z;
#endif

			return *this;
		}

		Vector& sub(TypeFloat x = 0.f, TypeFloat y = 0.f, TypeFloat z = 0.f) noexcept
		{
#ifdef NPLICIT_USE_AVX
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_sub_pd(x1, x2);
#else
			this->X -= x;
			this->Y -= y;
			this->Z -= z;
#endif

			return *this;
		}

		Vector& mul(TypeFloat x = 1.f, TypeFloat y = 1.f, TypeFloat z = 1.f) noexcept
		{
#ifdef NPLICIT_USE_AVX
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_mul_pd(x1, x2);
#else
			this->X *= x;
			this->Y *= y;
			this->Z *= z;
#endif

			return *this;
		}

	public:
		Vector& operator*=(const Vector& vec)
		{
			this->mul(vec.X, vec.Y, vec.Z);
			return *this;
		}

		Vector& operator+=(const Vector& vec)
		{
			this->add(vec.X, vec.Y, vec.Z);
			return *this;
		}

		Vector& operator-=(const Vector& vec)
		{
			this->sub(vec.X, vec.Y, vec.Z);
			return *this;
		}

	};

	using VectorFloat = Vector<float>;
	using VectorDouble = Vector<double>;

	template <typename TypeFloat = float>
	class NPLICIT_API PhysicsComponent
	{
	public:
		Vector<TypeFloat> Position;
		Vector<TypeFloat> Velocity;
		Vector<TypeFloat> Force;

		TypeFloat Mass;

	};

	template <typename TypeFloat = float>
	class NPLICIT_API PhysicsSystem : public Instance
	{
	public:
		PhysicsSystem() : Instance(), m_rigid_bodies(), m_delta(0), m_gravity(0, -9.81f, 0) {}
		virtual ~PhysicsSystem() = default;

		PhysicsSystem& operator=(const PhysicsSystem&) = delete;
		PhysicsSystem(const PhysicsSystem&) = delete;

		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_PHYSICS; }
		virtual const char* name() noexcept override { return ("PhysicsSystem"); }

		virtual void update() override
		{
			for (PhysicsComponent<TypeFloat>* rigid : m_rigid_bodies)
			{
				rigid->Force.mul(m_gravity.X * rigid->Mass, m_gravity.Y * rigid->Mass, m_gravity.Z * rigid->Mass);

				rigid->Velocity.add(rigid->Force.X / rigid->Mass * m_delta,
					rigid->Force.Y / rigid->Mass * m_delta,
					rigid->Force.Z / rigid->Mass * m_delta);

				rigid->Position.add(rigid->Velocity.X * m_delta, rigid->Velocity.Y * m_delta, rigid->Velocity.Z * m_delta);

				rigid->Force = Vector<TypeFloat>(0, 0, 0);

				++m_delta;
			}
		}

	public:
		void set(const Vector<float>& gravity) noexcept { m_gravity.add(gravity.X, gravity.Y, gravity.Z); }
		Vector<float>& get() noexcept { return m_gravity; }

		void add(PhysicsComponent<TypeFloat>* rigid)
		{
			if (!rigid)
				return;

			m_rigid_bodies.push_back(rigid);
		}

		void remove(PhysicsComponent<TypeFloat>* rigid)
		{
			if (!rigid)
				return;

			auto it = std::find(m_rigid_bodies.cbegin(), m_rigid_bodies.cend(), rigid);

			if (it != m_rigid_bodies.cend())
				m_rigid_bodies.erase(it);
		}

	private:
		std::vector<PhysicsComponent<TypeFloat>*> m_rigid_bodies;
		Vector<float> m_gravity;
		int64_t m_delta;

	};
}