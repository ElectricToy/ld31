//
//  Creature.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Creature.h"
#include "ldWorld.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Creature )
	
	DEFINE_VAR( Creature, Inventory::ptr, m_inventory );
	DEFINE_DVAR( Creature, bool, m_alive );
	DEFINE_VAR( Creature, ldActor::ptr, m_heldActor );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Creature )
	
	void Creature::onTouched( ldActor& other )
	{
		if( canPickup( other ))
		{
			pickup( other );
		}
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
		
		other.bePickedUpBy( *this );
		
		// TODO held vs. inventory.
		
		world().attach( other, *this );
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
				m_heldActor->beDroppedBy( *this );
				m_heldActor->position( proposedDestination );
				m_heldActor = nullptr;
			}
		}
	}
}

