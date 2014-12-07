//
//  Creature.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Creature_h
#define FreshApp_Creature_h

#include "Essentials.h"
#include "ldActor.h"
#include "Inventory.h"

namespace ld
{
	
	class Creature : public ldActor
	{
		FRESH_DECLARE_CLASS( Creature, ldActor );
	public:
		
		SYNTHESIZE_GET( bool, alive );
		
		virtual bool isCreature() const override { return true; }
		
		virtual bool mayCollide() const override;
		
		virtual void onTouched( ldActor& other ) override;

		virtual bool canBePickedUp() const;
		
		virtual bool canPickup( const ldActor& other ) const;
		virtual void pickup( ldActor& other );
		
		virtual void pickupTouchingActor();
		virtual void dropHeldActor();
		
		virtual vec2 bePickedUpBy( Creature& other ) override;
		
		virtual void update() override;
		
		virtual void onLanded( const vec2& hitNormal ) override;
		virtual void onBumpedWall( const vec2& hitNormal ) override;
		
		SYNTHESIZE_GET( vec2, stepDirection );
		bool canStep( const vec2& dir ) const;
		
		void applyControllerImpulse( const vec2& i ) override;
		
		vec2 facingDirection() const;
		
		virtual void onBeginPlay() override;
		
	protected:
		
		virtual void updateAI() {}
		
		void beginStepping( const vec2& dir );
		void stopStepping();
		bool isStepping() const;
		virtual void updateStepping();
		
		
	private:

		VAR( ldActor::ptr, m_heldActor );
		VAR( Inventory::ptr, m_inventory );
		DVAR( bool, m_alive, true );
		VAR( vec2, m_stepDirection );
		VAR( vec2, m_stepStart );
		DVAR( real, m_stepSpeed, 6.0f );
		DVAR( TimeType, m_thoughtSpeedHz, 1 );
		
		vec2 m_facingDirection = vec2( 1, 0 );
		
		FRESH_DECLARE_CALLBACK( Creature, onTimeToThink, fr::Event )
	};
	
}

#endif