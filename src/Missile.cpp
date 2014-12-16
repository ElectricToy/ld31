//
//  Missile.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Missile.h"
#include "Item.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Missile )
	DEFINE_DVAR( Missile, vec2, m_facingDirection );
	DEFINE_DVAR( Missile, real, m_damage );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Missile )

	bool Missile::wouldCollideWith( const ldActor& other )
	{
		// Missiles collide with living monsters.
		//
		if( other.isMonster() && other.alive())
		{
			return true;
		}
		else if( auto item = other.as< Item >() )
		{
			// And with placed items that block monsters
			// (such as doors and blocks).
			//
			if( item->placed() && item->blocksMonsters() )
			{
				return true;
			}
		}
		return false;
	}
	
	void Missile::facingDirection( const vec2& facingDirection_ )
	{
		m_facingDirection = facingDirection_;
		rotation( m_facingDirection.angle() );
	}
	
	void Missile::onLanded( const vec2& hitNormal )
	{
		onBumpedWall( hitNormal );
	}
	
	void Missile::onBumpedWall( const vec2& hitNormal )
	{
		die();
	}
	
	void Missile::onTouched( ldActor& other )
	{
		if( alive() )
		{
			if( wouldCollideWith( other ))
			{
				other.receiveDamage( m_damage );
				die();
			}
		}
	}
	
	void Missile::update()
	{
		Super::update();
		
		position( position() + m_facingDirection * 4 );
		
		if( nUpdates() > 40 )
		{
			die();
		}
	}

	void Missile::die()
	{
		Super::die();
		
		markForDeletion();
	}
}

