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
		if( !isPickedUp() )
		{
			// Shoot automatically.
			//
			// TODO
		}
		else
		{
			m_facingDirection = m_holder->facingDirection();
			rotation( m_facingDirection.angle() );
		}
		
		Super::update();
	}
	
	void Turret::shoot()
	{
		auto missile = createObject< Missile >( *getClass( "TurretArrow" ));
		ASSERT( missile );
		missile->position( position() );
		missile->facingDirection( m_facingDirection );
		
		world().addChild( missile );
		
		--m_ammo;
		
		if( m_ammo == 0 )
		{
			die();
		}
	}
	
}

