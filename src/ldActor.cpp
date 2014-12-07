//
//  ldActor.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldActor.h"
#include "ldTile.h"
#include "ldWorld.h"
#include "Stage.h"
#include "TileGrid.h"
#include "Creature.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldActor )
	DEFINE_DVAR( ldActor, vec2, m_carryOffset );
	DEFINE_DVAR( ldActor, vec2, m_carryScale );
	DEFINE_DVAR( ldActor, angle, m_carryRotation );
	DEFINE_DVAR( ldActor, vec2, m_precarryScale );
	DEFINE_DVAR( ldActor, real, m_health );
	DEFINE_DVAR( ldActor, bool, m_alive );
	DEFINE_VAR( ldActor, WeakPtr< Creature >, m_holder );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldActor )

	bool ldActor::mayCollide() const
	{
		return !isMarkedForDeletion() && !isPickedUp() && alive();
	}
	
	void ldActor::onAddedToStage()
	{
		Super::onAddedToStage();
		
		position( snapToGrid( position() ));
		recordPreviousState();
	}
	
	void ldActor::onTouched( ldActor& other )
	{}
	
	bool ldActor::isPickedUp() const
	{
		return m_holder != nullptr;
	}
	
	bool ldActor::canBePickedUp() const
	{
		return !isPickedUp() && alive();
	}
	
	bool ldActor::canBePickedUpByTouch() const
	{
		return canBePickedUp();
	}
	
	vec2 ldActor::bePickedUpBy( Creature& other )
	{
		ASSERT( !isPickedUp() );

		m_holder = &other;
		
		rotation( m_carryRotation );
		m_precarryScale = scale();
		scale( m_carryScale );
		
		return m_carryOffset;
	}
	
	void ldActor::beDroppedBy( Creature& other )
	{
		rotation( 0 );
		scale( m_precarryScale );
		m_holder = nullptr;
	}
	
	ldWorld& ldActor::world() const
	{
		const auto ancestor = firstAncestorOfType< ldWorld >( *this );
		ASSERT( ancestor );
		return *ancestor;
	}
	
	TileGrid& ldActor::tileGrid() const
	{
		return world().tileGrid();
	}
	
	void ldActor::update()
	{
		Super::update();
	}

	void ldActor::receiveDamage( real amount )
	{
		ASSERT( amount >= 0 );
		
		if( alive() )
		{
			m_health -= amount;
			
			if( m_health <= 0 )
			{
				die();
			}
		}
	}
	
	void ldActor::die()
	{
		m_alive = false;
	}
}

