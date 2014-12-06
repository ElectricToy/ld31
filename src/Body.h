//
//  Body.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/5/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Body_h
#define FreshApp_Body_h

#include "Essentials.h"

namespace ld
{
	
	class Body : public fr::Sprite
	{
		FRESH_DECLARE_CLASS( Body, Sprite );
	public:

		SYNTHESIZE_GET( real, mass );
		SYNTHESIZE_GET( real, airDrag );
		SYNTHESIZE_GET( real, stiffness );
		SYNTHESIZE_GET( real, windDrag );
		
		void teleport( const vec2& p )
		{
			const auto v = velocity();
			m_position = p;
			m_lastPosition = m_position - v;
		}
		
		bool underground() const	 { return m_position.y > GROUND_Y; }
		real undergroundness() const { return fr::proportion( m_position.y, GROUND_Y, DEEP_GROUND_Y ); }
		
		inline vec2 velocity() const { return m_position - m_lastPosition; }
		inline angle angularVelocity() const { return m_rotation - m_lastRotation; }

		virtual void update() override;
		
		virtual void postLoad() override;
		
		void applyGravity( const vec2& g );
		void applyWind( const vec2& f );
		void applyImpulse( const vec2& i );

		void applyTorque( const angle t );
		
	private:

		DVAR( real, m_mass, 1 );
		DVAR( real, m_airDrag, 0.1 );
		DVAR( real, m_buriedDrag, 0.5 );
		DVAR( real, m_stiffness, 100 );
		DVAR( real, m_windDrag, 1 );
		
		VAR( vec2, m_lastPosition );
		VAR( vec2, m_acceleration );
		
		VAR( angle, m_lastRotation );
		VAR( angle, m_torque );
		
	};
	
}

#endif
