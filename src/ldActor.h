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
#include "FreshActor.h"
#include "FreshTileGrid.h"
#include "FreshActorController.h"
#include "Lighting.h"

namespace ld
{
	class ldWorld;
	class Creature;
	
	class ldActor : public fr::FreshActor
	{
		FRESH_DECLARE_CLASS( ldActor, FreshActor );
	public:
		
		SYNTHESIZE_GET( bool, alive );
		SYNTHESIZE_GET( real, health );
		SYNTHESIZE_GET( real, maxHealth );
		SYNTHESIZE_GET( bool, suppressesHolderLight );
		SYNTHESIZE_GET( bool, dropsWhenUsed );
		SYNTHESIZE_GET( bool, pushesHolderWhenUsed );
		SYNTHESIZE_GET( std::string, friendlyName );
		virtual int desiredDepth() const;
		
		
		virtual real lightWobble() const;
		
		
		virtual void update() override;

		virtual bool isPlayer() const { return false; }
		virtual bool isHuman() const { return false; }
		virtual bool isMonster() const { return false; }
		virtual bool isCreature() const { return false; }
		virtual bool isItem() const { return false; }
		
		virtual bool mayCollide() const;
	
		virtual void onAddedToStage() override;
		
		virtual void onTouched( ldActor& other );
		
		virtual bool canBePickedUp() const;
		virtual bool canBePickedUpByTouch() const;
		bool isPickedUp() const;
		
		
		virtual vec2 bePickedUpBy( Creature& other );
		virtual void beDroppedBy( Creature& other );
		virtual void beUsedBy( Creature& other );
		
		virtual void receiveDamage( real amount );
		
		virtual void die();
		
	protected:
		
		SYNTHESIZE_GET( WeakPtr< Creature >, holder );
		SYNTHESIZE_GET( TimeType, diedTime );

		SYNTHESIZE_GET( fr::LightSource::ptr, lightSource );

		DVAR( real, m_lightRadius, 0 );
		DVAR( Color, m_lightColor, Color::White );

		ldWorld& world() const;
		fr::FreshTileGrid& tileGrid() const;
		
	private:

		DVAR( vec2, m_carryOffset, vec2( 5, -5 ));
		DVAR( vec2, m_carryScale, vec2( 0.75f ));
		DVAR( angle, m_carryRotation, 30 );
		DVAR( vec2, m_precarryScale, vec2( 1 ));

		DVAR( real, m_health, 1 );
		DVAR( real, m_maxHealth, 1 );
		
		DVAR( bool, m_alive, true );
		
		DVAR( bool, m_suppressesHolderLight, false );
		DVAR( bool, m_dropsWhenUsed, true );
		DVAR( bool, m_pushesHolderWhenUsed, false );

		DVAR( int, m_desiredDepth, 0 );

		VAR( ClassInfo::cptr, m_dieEmitterClass );
		VAR( std::string, m_friendlyName );

		DVAR( TimeType, m_diedTime, -1 );
		VAR( WeakPtr< Creature >, m_holder );
		
		// Glow light.
		//
		VAR( ClassInfo::cptr, m_lightClass );
		DVAR( real, m_lightWobble, 16 );
		DVAR( real, m_lightWobbleLerp, 0.1f );

		fr::LightSource::ptr m_lightSource;
	};
	
}

#endif
