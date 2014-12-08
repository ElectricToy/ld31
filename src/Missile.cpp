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
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Missile )

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
		if( other.isMonster() )
		{
			other.receiveDamage( 1.0f );
			die();
		}
		else if( auto item = other.as< Item >() )
		{
			if( item->m_blocksMonsters && item->placed() )
			{
				other.receiveDamage( 1.0f );
				die();
			}
		}
	}
	
	void Missile::update()
	{
		Super::update();
		
		rotation( m_facingDirection.angle() );
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

