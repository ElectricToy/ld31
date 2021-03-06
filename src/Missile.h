//
//  Missile.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Missile_h
#define FreshApp_Missile_h

#include "Essentials.h"
#include "ldActor.h"

namespace ld
{
	
	class Missile : public ldActor
	{
		FRESH_DECLARE_CLASS( Missile, ldActor );
	public:
		
		SYNTHESIZE_GET( vec2, facingDirection )
		void facingDirection( const vec2& facingDirection_ );
		
		virtual void onTouched( ldActor& other ) override;
		
		virtual void update() override;
		virtual void onLanded( const vec2& hitNormal ) override;
		virtual void onBumpedWall( const vec2& hitNormal ) override;

		virtual void die() override;
		
		static bool wouldCollideWith( const ldActor& other );
		
	private:
		
		DVAR( vec2, m_facingDirection, vec2( 1, 0 ));
		DVAR( real, m_damage, 1.0f );
		
	};
	
}

#endif
