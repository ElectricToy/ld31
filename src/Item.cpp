//
//  Item.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Item.h"
#include "ldTile.h"
#include "ldWorld.h"
#include "Creature.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Item )
	
	DEFINE_DVAR( Item, bool, m_placed );
	DEFINE_DVAR( Item, real, m_navDistanceScalar );
	DEFINE_DVAR( Item, bool, m_blocksHumans );
	DEFINE_DVAR( Item, bool, m_blocksMonsters );
	DEFINE_VAR( Item, fr::DisplayObjectContainer::ptr, m_cracksHost );
	DEFINE_DVAR( Item, bool, m_mayBePlaced );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Item )
	
	real Item::navDistanceScalar() const
	{
		return m_placed ? m_navDistanceScalar : 1.0f;
	}
	
	bool Item::doesBlock( const Creature& creature ) const
	{
		return m_placed &&
			(( creature.isHuman() && blocksHumans() ) ||
			 ( creature.isMonster() && blocksMonsters() ));
	}
	
	void Item::onAddedToStage()
	{
		Super::onAddedToStage();
		
		if( !m_cracks )
		{
			m_cracks = createObject< MovieClip >( *getClass( "Cracks" ));
			if( !m_cracksHost )
			{
				m_cracksHost = this;
			}
			m_cracksHost->addChild( m_cracks );
		}
		
		showAsPlaced( m_placed );
		
		updateCracks();
		
		addToTile();
	}
	
	void Item::addToTile()
	{
		world().tileAt( position() ).addItem( this );
	}
	
	void Item::removeFromTile()
	{
		world().tileAt( position() ).removeItem( this );
	}
	
	bool Item::canBePickedUpByTouch() const
	{
		return !m_placed && Super::canBePickedUpByTouch();
	}
	
	vec2 Item::bePickedUpBy( Creature& other )
	{
		removeFromTile();
		m_placed = false;
		showAsPlaced( m_placed );
		return Super::bePickedUpBy( other );
	}
	
	void Item::beDroppedBy( Creature& other )
	{
		Super::beDroppedBy( other );
		m_placed = false;
		showAsPlaced( m_placed );
		
		addToTile();
	}
	
	void Item::beUsedBy( Creature& other )
	{
		Super::beUsedBy( other );

		m_placed = m_mayBePlaced;
		showAsPlaced( m_placed );
	}

	void Item::die()
	{
		if( !isPickedUp() )
		{
			removeFromTile();
		}
		else
		{
			m_holder->dropHeldActor();
		}
		
		Super::die();
		
		markForDeletion();		// TODO!!!
	}
	
	void Item::receiveDamage( real amount )
	{
		Super::receiveDamage( amount );
		
		updateCracks();
	}
	
	void Item::showAsPlaced( bool placed )
	{
		if( !placed )
		{
			scale( 0.75f );
			rotation( 30 );
		}
		else
		{
			scale( 1 );
			rotation( 0 );
		}
	}
	
	void Item::updateCracks()
	{
		if( m_cracks )
		{
			if( alive() )
			{
				const auto healthiness = clamp( health() / maxHealth(), 0.0f, 1.0f );
				const int frame = 4 - std::round( healthiness * 4 );
				
				m_cracks->gotoAndStop( frame );
			}
			else
			{
				m_cracks->visible( false );
			}
		}
	}
}

