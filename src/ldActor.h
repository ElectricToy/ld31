//
//  ldActor.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_ldActor_h
#define FreshApp_ldActor_h

#include "Essentials.h"
#include "Actor.h"

namespace ld
{
	
	class ldActor : public fr::Actor
	{
		FRESH_DECLARE_CLASS( ldActor, Actor );
	public:
		
		virtual void update() override;
		
		virtual void applyControllerImpulse( const vec2& i ) override;
		
		// Disabled.
		virtual void jump( real jumpPower ) override {}
		
	protected:

		virtual void onLanded( const vec2& hitNormal ) override;
		virtual void onBumpedWall( const vec2& hitNormal ) override;

		void beginStepping( const vec2& dir );
		void stopStepping();
		bool isStepping() const;
		virtual void updateStepping();
		
	private:
	
		VAR( vec2, m_stepDirection );
		VAR( vec2, m_stepStart );
		DVAR( real, m_stepSpeed, 4.0f );
	
		vec2 m_pendingStepDirection;
	
	};
	
}

#endif
