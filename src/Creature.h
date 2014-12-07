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
		
		virtual bool isCreature() const override { return true; }
		
		virtual void onTouched( ldActor& other ) override;

		virtual bool canPickup( const ldActor& other ) const;
		virtual void pickup( ldActor& other );
		
		virtual void pickupTouchingActor();
		virtual void dropHeldActor();
		
		virtual vec2 bePickedUpBy( Creature& other ) override;
		
		virtual void update() override;
		
		virtual void onLanded( const vec2& hitNormal ) override;
		virtual void onBumpedWall( const vec2& hitNormal ) override;
		
		SYNTHESIZE_GET( vec2, stepDirection );
		virtual bool canStep( const vec2& dir ) const;
		
		virtual void applyControllerImpulse( const vec2& i ) override;
		
		vec2 facingDirection() const;
		
		virtual void onBeginPlay() override;
		
		void travelTo( const vec2& pos );
		void stopTravel();
		vec2 travelDestination() const;
		
	protected:
		
		SYNTHESIZE_GET( ldActor::ptr, heldActor );
		
		virtual real currentStepSpeed() const;

		void pursueTilePath( const fr::TileGrid::Path& path );
		
		virtual void updateAI() {}
		
		void beginStepping( const vec2& dir );
		void stopStepping();
		bool isStepping() const;
		virtual void updateStepping();

		virtual void grind( const vec2& dir );
		
		virtual void die() override;
		
	private:

		VAR( ldActor::ptr, m_heldActor );
		VAR( Inventory::ptr, m_inventory );
		VAR( vec2, m_stepDirection );
		VAR( vec2, m_stepStart );
		DVAR( real, m_stepSpeed, 6.0f );
		DVAR( TimeType, m_thoughtSpeedHz, 1 );
		DVAR( real, m_grindDamage, 0 );
		DVAR( real, m_normalLightRadius, 0 );
		
		vec2 m_facingDirection = vec2( 1, 0 );
		
		fr::TileGrid::WorldSpacePath m_worldSpacePath;
		
		FRESH_DECLARE_CALLBACK( Creature, onTimeToThink, fr::Event )
	};
	
}

#endif
