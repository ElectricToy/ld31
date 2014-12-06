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
		
		inline vec2 velocity() const { return m_position - m_lastPosition; }

		virtual void update() override;
		
		virtual void postLoad() override;
		
		virtual void applyGravity( const vec2& g );
		virtual void applyWind( const vec2& f );
		virtual void applyImpulse( const vec2& i );

	private:

		DVAR( real, m_mass, 1 );
		DVAR( real, m_airDrag, 0 );
		DVAR( real, m_stiffness, 300 );
		DVAR( real, m_windDrag, 1 );
		
		VAR( vec2, m_lastPosition );
		VAR( vec2, m_acceleration );
		
	};
	
}

#endif
