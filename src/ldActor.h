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
#include "TileGrid.h"

namespace ld
{
	class ldWorld;
	
	class ldActor : public fr::Actor
	{
		FRESH_DECLARE_CLASS( ldActor, Actor );
	public:
		
		virtual void update() override;
		
		SYNTHESIZE_GET( vec2, stepDirection );
		bool canStep( const vec2& dir ) const;		
		
		void applyControllerImpulse( const vec2& i ) override;
		
		virtual void onAddedToStage() override;
		
	protected:

		ldWorld& world() const;
		fr::TileGrid& tileGrid() const;
	
		virtual void onLanded( const vec2& hitNormal ) override;
		virtual void onBumpedWall( const vec2& hitNormal ) override;

		void beginStepping( const vec2& dir );
		void stopStepping();
		bool isStepping() const;
		virtual void updateStepping();
		
	private:
	
		VAR( vec2, m_stepDirection );
		VAR( vec2, m_stepStart );
		DVAR( real, m_stepSpeed, 6.0f );
		
		vec2 m_lastStepDirection;
	};
	
}

#endif
