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
		
		virtual void update() override;
		
		virtual void postLoad() override;
		
		virtual void applyImpulse( const vec2& i );
		
	private:

		DVAR( real, m_mass, 0 );
		DVAR( real, m_damping, 0 );
		
		VAR( vec2, m_lastPosition );
		VAR( vec2, m_acceleration );
		
	};
	
}

#endif
