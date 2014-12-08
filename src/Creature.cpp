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

namespace
{
	const real MIN_DIST_TO_TOUCH_NODE = 2.0f;
	const real MIN_DIST_TO_TOUCH_NODE_SQUARED = MIN_DIST_TO_TOUCH_NODE * MIN_DIST_TO_TOUCH_NODE;
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Creature )
	
	DEFINE_VAR( Creature, vec2, m_stepDirection );
	DEFINE_VAR( Creature, vec2, m_stepStart );
	DEFINE_DVAR( Creature, real, m_stepSpeed );
	DEFINE_VAR( Creature, Inventory::ptr, m_inventory );
	DEFINE_VAR( Creature, ldActor::ptr, m_heldActor );
	DEFINE_DVAR( Creature, TimeType, m_thoughtSpeedHz );
	DEFINE_DVAR( Creature, real, m_grindDamage );
	DEFINE_DVAR( Creature, real, m_normalLightRadius );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Creature )

	real Creature::normalLightRadius() const
	{
		return m_normalLightRadius;
	}
	
	real Creature::currentStepSpeed() const
	{
		return m_stepSpeed;
	}
	
	void Creature::onBeginPlay()
	{
		Super::onBeginPlay();
		ASSERT( m_thoughtSpeedHz > 0 );
		
		// Randomize to deter Creatures from thinking on the same frames.
		world().scheduleCallback( FRESH_CALLBACK( onTimeToThink ), 1.0 / m_thoughtSpeedHz + randInRange( 0.0, 1.0 ));
	}
	
	void Creature::onTouched( ldActor& other )
	{
		// TODO Automatic pickup disabled.
//		if( canPickup( other ) && other.canBePickedUpByTouch() )
//		{
//			pickup( other );
//		}
	}

	vec2 Creature::bePickedUpBy( Creature& other )
	{
		ASSERT( !isPickedUp() );
		stopStepping();
		dropHeldActor();
		return Super::bePickedUpBy( other );
	}
	
	bool Creature::canPickup( const ldActor& other ) const
	{
		ASSERT( this != &other );
		
		// TODO held vs inventory.
		
		return alive() && !isPickedUp() && other.canBePickedUp() && !m_heldActor;
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

	ldActor::ptr Creature::pickupableTouchingActor() const
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

		return touchingActor;
	}
	
	bool Creature::canPickupTouchingActor() const
	{
		return pickupableTouchingActor() != nullptr;
	}
	
	void Creature::pickupTouchingActor()
	{
		// Any actors touching me?
		//
		if( ldActor::ptr touchingActor = pickupableTouchingActor() )
		{
			// Yes.
			
			ASSERT( canPickup( *touchingActor ));
			
			pickup( *touchingActor );
		}
	}

	bool Creature::canDropHeldActor() const
	{
		if( m_heldActor )
		{
			const vec2 proposedDestination = snapToGrid( position() );
			
			const auto& tile = world().tileAt( proposedDestination );
			
			if( !tile.isSolid() && ( !m_heldActor->isItem() || tile.mayReceiveItem() ))
			{
				const vec2& proposedDestinationForMe = snapToGrid( position() );
				const auto& myTile = world().tileAt( proposedDestinationForMe );
				
				return !myTile.isSolid();
			}
		}
		return false;
	}
	
	void Creature::dropHeldActor()
	{
		if( canDropHeldActor() )
		{
			const vec2 proposedDestination = snapToGrid( position() );
			world().detach( *m_heldActor, *this );
			m_heldActor->position( proposedDestination );
			m_heldActor->recordPreviousState();
			m_heldActor->beDroppedBy( *this );
			m_heldActor = nullptr;
			
			const vec2& proposedDestinationForMe = snapToGrid( position() );
			position( proposedDestinationForMe );
		}
	}
	
	bool Creature::canUseHeldActor() const
	{
		if( isHuman() && m_heldActor )		// Only humans can "use" actors.
		{
			if( m_heldActor->dropsWhenUsed() )
			{
				const vec2 proposedDestination = snapToGrid( position() );
				
				const auto& tile = world().tileAt( proposedDestination );
				
				if( !tile.isSolid() && ( !m_heldActor->isItem() || tile.mayReceiveItem() ))
				{
					const vec2& proposedDestinationForMe = snapToGrid( position() - ( m_heldActor->pushesHolderWhenUsed() ? facingDirection() * WORLD_PER_TILE : vec2::ZERO ));
					const auto& myTile = world().tileAt( proposedDestinationForMe );
					
					return !myTile.isSolid();
				}
				return false;
			}
			else
			{
				return true;
			}
		}
		return false;
	}
	
	void Creature::useHeldActor()
	{
		if( canUseHeldActor() )
		{
			const bool dropsWhenUsed = m_heldActor->dropsWhenUsed();
			
			if( dropsWhenUsed )
			{
				const vec2 proposedDestination = snapToGrid( position() );
				world().detach( *m_heldActor, *this );
				m_heldActor->position( proposedDestination );
				m_heldActor->recordPreviousState();

				const vec2& proposedDestinationForMe = snapToGrid( position() - ( m_heldActor->pushesHolderWhenUsed() ? facingDirection() * WORLD_PER_TILE : vec2::ZERO ));
				position( proposedDestinationForMe );
			}
			
			m_heldActor->beUsedBy( *this );
			
			if( dropsWhenUsed )
			{
				m_heldActor = nullptr;
			}
		}
	}
	
	void Creature::update()
	{
		if( alive() && !isPickedUp() )
		{
			if( !m_worldSpacePath.empty() )
			{
				applyControllerImpulse( snapToGrid( m_worldSpacePath.front() ) - position() );
				
				// Have we reached the next path node?
				//
				if( distanceSquared( position(), m_worldSpacePath.front() ) < MIN_DIST_TO_TOUCH_NODE_SQUARED )
				{
					m_worldSpacePath.erase( m_worldSpacePath.begin() );
				}
			}

			updateStepping();
		}
		
		if( m_heldActor && m_heldActor->suppressesHolderLight() )
		{
			m_lightRadius = 0;
		}
		else
		{
			m_lightRadius = lerp( m_lightRadius, normalLightRadius(), 0.05f );;
		}
		
		Super::update();
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

			// Found nothing? Well just pick one then.
			//
			if( stepToPursue.isZero() )
			{
				for( int i = 0; i < 2; ++i )
				{
					const auto& potentialStep = steps[ i ];
					if( !potentialStep.isZero() )
					{
						stepToPursue = potentialStep;
						break;
					}
				}
			}
			
			stepToPursue.normalize();
			ASSERT( !stepToPursue.isZero() );
			beginStepping( stepToPursue );
		}
	}
	
	bool Creature::canStep( const vec2& dir ) const
	{
		ASSERT( dir.x == 0 || dir.y == 0 );
		
		const auto proposedStart = snapToGrid( position() );
		const auto proposedDest = proposedStart + dir.normal() * WORLD_PER_TILE;
		const fr::Direction fromDirection( -dir );
		
		const auto& tile = world().tileAt( proposedDest );
		if( !tile.isNavigable( fromDirection ))
		{
			return false;
		}
		else if( auto item = tile.containedItem())
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
		ASSERT( dir.lengthSquared() == 1 );
		
		// Refuse if there's a wall there.
		//
		if( canStep( dir ))
		{
			m_stepStart = snapToGrid( position() );
			m_stepDirection = dir.normal();
			m_facingDirection = m_stepDirection;
		}
		else
		{
			grind( dir );
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
			const auto newPos = position() + m_stepDirection * WORLD_PER_TILE * currentStepSpeed() * stage().secondsPerFrame();
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

	void Creature::grind( const vec2& dir )
	{
		ASSERT( dir.x == 0 || dir.y == 0 );
		ASSERT( dir.lengthSquared() == 1.0f );
		
		m_facingDirection = dir;
		
		// Is there an item in the tile in front of us?
		//
		if( auto item = world().itemInTile( snapToGrid( position() + dir * WORLD_PER_TILE )))
		{
			ASSERT( item->alive() );
			item->receiveDamage( grindDamage() * stage().secondsPerFrame() );
		}
	}

	void Creature::travelTo( const vec2& pos )
	{
		TileGrid::Path path;
		tileGrid().findClosestPath( position(), pos, path, WORLD_PER_TILE * 0.4f );
		
		// The first element, if any, is where I am already.
		if( !path.empty() )
		{
			path.erase( path.begin() );
		}
		
		pursueTilePath( path );
	}
	
	void Creature::stopTravel()
	{
		m_worldSpacePath.clear();
	}
	
	vec2 Creature::travelDestination() const
	{
		if( m_worldSpacePath.empty() )
		{
			return vec2::ZERO;
		}
		else
		{
			return m_worldSpacePath.back();
		}
	}
	
	void Creature::pursueTilePath( const fr::TileGrid::Path& path )
	{
		m_worldSpacePath.clear();
		tileGrid().convertToWorldSpacePath( path.begin(), path.end(), std::back_inserter( m_worldSpacePath ));
		
	}

	void Creature::die()
	{
		stopTravel();
		
		dropHeldActor();
		
		color( DEAD_COLOR );
		
		Super::die();
	}
	
	FRESH_DEFINE_CALLBACK( Creature, onTimeToThink, fr::Event )
	{
		if( doUpdate() && alive() && !isPickedUp() )
		{
			updateAI();
		}
		
		if( doUpdate() && alive() )
		{
			ASSERT( m_thoughtSpeedHz > 0 );
			world().scheduleCallback( FRESH_CALLBACK( onTimeToThink ), 1.0 / m_thoughtSpeedHz );
		}
	}
	
	real Creature::grindDamage() const
	{
		return m_grindDamage;
	}
}

