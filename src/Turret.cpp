//
//  Turret.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Turret.h"
#include "Creature.h"
#include "Missile.h"
#include "ldWorld.h"
using namespace fr;

namespace
{
	using namespace ld;
	const real MIN_TURRET_SHOT_RANGE = WORLD_PER_TILE;
	const real MIN_TURRET_SHOT_RANGE_SQUARED = MIN_TURRET_SHOT_RANGE * MIN_TURRET_SHOT_RANGE;
	const int TURRET_RANGE_TILES = 6;
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Turret )
	DEFINE_VAR( Turret, size_t, m_ammo );
	DEFINE_VAR( Turret, vec2, m_facingDirection );
	DEFINE_VAR( Turret, TimeType, m_nextEarliestShootTime );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Turret )

	vec2 Turret::bePickedUpBy( Creature& other )
	{
		Super::bePickedUpBy( other );
		
		facingDirection( other.facingDirection() );
		
		return vec2::ZERO;
	}
	
	void Turret::beDroppedBy( Creature& other )
	{
		Super::beDroppedBy( other );
		
		facingDirection( other.facingDirection() );
	}
	
	void Turret::beUsedBy( Creature& other )
	{
		if( alive() )
		{
			shoot();
			
			Super::beUsedBy( other );
		}
	}
	
	void Turret::update()
	{
		Super::update();

		if( !isPickedUp() )
		{
			if( world().isGameActive() && !m_facingDirection.isZero() )
			{
				// Shoot automatically.
				//
				const real range = TURRET_RANGE_TILES;
				real closestDistSquared = Infinity;
				ldActor::ptr target;
				
				world().touchingActors( rect{ position() - WORLD_PER_TILE * range, position() + WORLD_PER_TILE * range }, [&]( ldActor& actor )
									   {
										   // Relevant for my missile collision?
										   //
										   if( Missile::wouldCollideWith( actor ))
										   {
											   // In line with my shot?
											   //
											   const auto delta = actor.position() - position();
											   if( m_facingDirection.dot( delta.normal() ) > 0.99f )
											   {
												   const auto distSquared = delta.lengthSquared();
												   
												   if( distSquared < closestDistSquared )
												   {
													   closestDistSquared = distSquared;
													   target = &actor;
												   }
											   }
										   }
									   } );
				
				if( target && target->isMonster() && distanceSquared( position(), target->position() ) > MIN_TURRET_SHOT_RANGE_SQUARED )
				{
					const Vector2i step = vector_cast< int >( m_facingDirection );
					ASSERT( !step.isZero() );
					
					Vector2i tilePos = tileGrid().worldToTileSpace( position() );
					const Vector2i endTile = tileGrid().worldToTileSpace( target->position() );
					
					bool clear = true;
					for( ; tilePos != endTile; tilePos += step )
					{
						const auto& tile = tileGrid().getTile( tilePos );
						if( tile.isSolid() )
						{
							clear = false;
							break;
						}
					}
					
					if( clear )
					{
						shoot();
					}
				}
			}
		}
		else
		{
			ASSERT( holder() );
			facingDirection( holder()->facingDirection() );
		}
	}
	
	void Turret::shoot()
	{
		if( !m_facingDirection.isZero() && world().time() > m_nextEarliestShootTime )
		{
			auto missile = createObject< Missile >( *getClass( "TurretArrow" ));
			ASSERT( missile );
			missile->position( position() );
			missile->facingDirection( m_facingDirection );
			
			world().addChild( missile );
			
			--m_ammo;
			
			const TimeType SHOT_DELAY = 1;
			
			m_nextEarliestShootTime = world().time() + SHOT_DELAY;

			if( m_ammo == 0 )
			{
				die();
			}
		}
	}

	void Turret::facingDirection( const vec2& facingDirection_ )
	{
		m_facingDirection = facingDirection_;
		rotation( m_facingDirection.angle() );
	}
}

