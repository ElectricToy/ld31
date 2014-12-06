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
#include "ActorController.h"

namespace ld
{
	class ldWorld;
	class Creature;
	
	class ldActor : public fr::Actor
	{
		FRESH_DECLARE_CLASS( ldActor, Actor );
	public:
		
		virtual void update() override;

		virtual bool isPlayer() const { return false; }
		virtual bool isHuman() const { return false; }
		virtual bool isMonster() const { return false; }
		virtual bool isCreature() const { return false; }
		virtual bool isItem() const { return false; }
	
		SYNTHESIZE_GET( vec2, stepDirection );
		bool canStep( const vec2& dir ) const;		
		
		void applyControllerImpulse( const vec2& i ) override;
		
		virtual void onAddedToStage() override;
		
		virtual void onTouched( ldActor& other );
		
		virtual bool canBePickedUp() const;
		bool isPickedUp() const;
		
		virtual void bePickedUpBy( Creature& other );
		virtual void beDroppedBy( Creature& other );
		
		vec2 facingDirection() const;
		
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
		
		VAR( WeakPtr< Creature >, m_holder );
		
		vec2 m_facingDirection = vec2( 1, 0 );
	};
	
}

#endif
