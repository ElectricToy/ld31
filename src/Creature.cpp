//
//  Creature.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Creature.h"
#include "ldWorld.h"
#include "ldTile.h"
#include "AppStage.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Creature )
	
	DEFINE_VAR( Creature, vec2, m_stepDirection );
	DEFINE_VAR( Creature, vec2, m_stepStart );
	DEFINE_DVAR( Creature, real, m_stepSpeed );
	DEFINE_VAR( Creature, Inventory::ptr, m_inventory );
	DEFINE_DVAR( Creature, bool, m_alive );
	DEFINE_VAR( Creature, ldActor::ptr, m_heldActor );
	DEFINE_DVAR( Creature, TimeType, m_thoughtSpeedHz );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Creature )

	void Creature::onBeginPlay()
	{
		Super::onBeginPlay();
		ASSERT( m_thoughtSpeedHz > 0 );
		stage().scheduleCallback( FRESH_CALLBACK( onTimeToThink ), 1.0 / m_thoughtSpeedHz );
	}
	
	bool Creature::mayCollide() const
	{
		return alive() && Super::mayCollide();
	}
	
	void Creature::onTouched( ldActor& other )
	{
		if( canPickup( other ) && other.canBePickedUpByTouch() )
		{
			pickup( other );
		}
	}

	vec2 Creature::bePickedUpBy( Creature& other )
	{
		ASSERT( !isPickedUp() );
		stopStepping();
		return Super::bePickedUpBy( other );
	}
	
	bool Creature::canBePickedUp() const
	{
		return Super::canBePickedUp() && alive();
	}
	
	bool Creature::canPickup( const ldActor& other ) const
	{
		ASSERT( this != &other );
		
		// TODO held vs inventory.
		
		return !m_heldActor && other.canBePickedUp();
	}
	
	void Creature::pickup( ldActor& other )
	{
		ASSERT( canPickup( other ));
		ASSERT( other.canBePickedUp() );
		
		const vec2 desiredCarryOffset = other.bePickedUpBy( *this );
		
		// TODO held vs. inventory.
		
		world().attach( other, *this, desiredCarryOffset );
		m_heldActor = &other;
	}
	
	void Creature::pickupTouchingActor()
	{
		// Any actors touching me?
		//
		ldActor::ptr touchingActor;
		world().touchingActors( collisionBounds(), [&]( ldActor& actor )
							   {
								   if( !touchingActor && this != &actor && canPickup( actor ))
								   {
									   touchingActor = &actor;
								   }
							   } );
		
		if( touchingActor )
		{
			// Yes.
			
			ASSERT( canPickup( *touchingActor ));
			
			pickup( *touchingActor );
		}
	}
	
	void Creature::dropHeldActor()
	{
		if( m_heldActor )
		{
			const vec2 proposedDestination = snapToGrid( position() + facingDirection() * WORLD_PER_TILE );
			
			if( !tileGrid().getTile( proposedDestination ).isSolid())
			{
				world().detach( *m_heldActor, *this );
				m_heldActor->position( proposedDestination );
				m_heldActor->beDroppedBy( *this );
				m_heldActor->recordPreviousState();
				m_heldActor = nullptr;
			}
		}
	}
	
	void Creature::update()
	{
		if( alive() && !isPickedUp() )
		{
			updateStepping();
			Super::update();
		}
	}

	
	void Creature::applyControllerImpulse( const vec2& i )
	{
		if( !isPickedUp() && !isStepping() && !i.isZero() )
		{
			// More than one direction implied here?
			//
			const vec2 steps[ 2 ] =
			{
				vec2( i.x, 0 ),
				vec2( 0, i.y )
			};
			
			vec2 stepToPursue;
			
			for( int i = 0; i < 2; ++i )
			{
				const auto& potentialStep = steps[ i ];
				if( !potentialStep.isZero() )
				{
					if( canStep( potentialStep ))
					{
						// We could do this. But if it's the direction we're already moving,
						// and we have an alternative, then keep the alternative.
						//
						if( stepToPursue.isZero() || potentialStep != m_facingDirection )
						{
							stepToPursue = potentialStep;
						}
					}
				}
			}
			
			if( !stepToPursue.isZero() )
			{
				beginStepping( stepToPursue );
			}
		}
	}
	
	bool Creature::canStep( const vec2& dir ) const
	{
		ASSERT( dir.x == 0 || dir.y == 0 );
		
		const auto proposedStart = snapToGrid( position() );
		const auto proposedDest = proposedStart + dir.normal() * WORLD_PER_TILE;
		const fr::Direction fromDirection( -dir );
		
		auto tile = tileGrid().getTile( proposedDest ).as< ldTile >();
		ASSERT( tile );
		if( !tile->isNavigable( fromDirection ))
		{
			return false;
		}
		else if( auto item = tile->containedItem())
		{
			return !item->doesBlock( *this );
		}
		else
		{
			return true;
		}
	}
	
	void Creature::beginStepping( const vec2& dir )
	{
		ASSERT( !isPickedUp() );
		ASSERT( !isStepping() );
		ASSERT( dir.x == 0 || dir.y == 0 );
		
		// Refuse if there's a wall there.
		//
		if( canStep( dir ))
		{
			m_stepStart = snapToGrid( position() );
			m_stepDirection = dir.normal();
			m_facingDirection = m_stepDirection;
		}
	}
	
	void Creature::stopStepping()
	{
		m_stepDirection.setToZero();
		position( snapToGrid( position() ));
	}
	
	bool Creature::isStepping() const
	{
		return !m_stepDirection.isZero();
	}
	
	void Creature::updateStepping()
	{
		if( isStepping() )
		{
			// Move.
			//
			const auto newPos = position() + m_stepDirection * WORLD_PER_TILE * m_stepSpeed * stage().secondsPerFrame();
			position( newPos );
			
			// Done yet?
			//
			const auto distTraveled = ( newPos - m_stepStart ).dot( m_stepDirection );
			
			if( distTraveled >= WORLD_PER_TILE )
			{
				// Done stepping.
				//
				stopStepping();
			}
		}
	}
	
	void Creature::onLanded( const vec2& hitNormal )
	{
		Super::onLanded( hitNormal );
		stopStepping();
	}
	
	void Creature::onBumpedWall( const vec2& hitNormal )
	{
		stopStepping();
	}
	
	vec2 Creature::facingDirection() const
	{
		return m_facingDirection;
	}
	
	FRESH_DEFINE_CALLBACK( Creature, onTimeToThink, fr::Event )
	{
		if( alive() && !isPickedUp() )
		{
			updateAI();
		}
		
		if( alive() )
		{
			ASSERT( m_thoughtSpeedHz > 0 );
			stage().scheduleCallback( FRESH_CALLBACK( onTimeToThink ), 1.0 / m_thoughtSpeedHz );
		}
	}
}
