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

namespace ld
{	
	FRESH_DEFINE_CLASS( Turret )
	DEFINE_DVAR( Turret, size_t, m_ammo );
	DEFINE_DVAR( Turret, vec2, m_facingDirection );
	DEFINE_DVAR( Turret, TimeType, m_nextEarliestShootTime );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Turret )

	vec2 Turret::bePickedUpBy( Creature& other )
	{
		Super::bePickedUpBy( other );

		m_facingDirection = other.facingDirection();
		rotation( m_facingDirection.angle() );
		
		return vec2::ZERO;
	}
	
	void Turret::beDroppedBy( Creature& other )
	{
		Super::beDroppedBy( other );
		
		m_facingDirection = other.facingDirection();
		rotation( m_facingDirection.angle() );
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
			// Shoot automatically.
			//
			const real range = 6;
			real closestDistSquared = Infinity;
			ldActor::ptr target;
			
			world().touchingActors( rect{ position() - WORLD_PER_TILE * range, position() + WORLD_PER_TILE * range }, [&]( ldActor& actor )
								   {
									   // Relevant for my missile collision?
									   //
									   bool relevant = actor.isMonster() && actor.alive();
									   
									   if( !relevant )
									   {
										   if( auto item = actor.as< Item >() )
										   {
											   if( item->m_blocksMonsters && item->placed() )
											   {
												   relevant = true;
											   }
										   }
									   }
									   
									   if( relevant )
									   {
										   // In line with my shot?
										   //
										   const auto delta = actor.position() - position();
										   if( m_facingDirection.dot( delta ) > 0.99f )
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
			
			if( target && target->isMonster() )
			{
				shoot();
			}
		}
		else
		{
			m_facingDirection = m_holder->facingDirection();
			rotation( m_facingDirection.angle() );
		}
	}
	
	void Turret::shoot()
	{
		if( world().time() > m_nextEarliestShootTime )
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
	
}

