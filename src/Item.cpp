//
//  Item.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Item.h"
#include "ldTile.h"
#include "Creature.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Item )
	
	DEFINE_DVAR( Item, bool, m_placed );
	DEFINE_DVAR( Item, real, m_navDistanceScalar );
	DEFINE_DVAR( Item, bool, m_blocksHumans );
	DEFINE_DVAR( Item, bool, m_blocksMonsters );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Item )
	
	bool Item::doesBlock( const Creature& creature ) const
	{
		return
			( creature.isHuman() && m_blocksHumans ) ||
			( creature.isMonster() && m_blocksMonsters );
	}
	
	void Item::onAddedToStage()
	{
		Super::onAddedToStage();
		addToTile();
	}
	
	void Item::addToTile()
	{
		if( auto tile = tileGrid().getTile( position() ).as< ldTile >())
		{
			tile->addItem( this );
		}
	}
	
	void Item::removeFromTile()
	{
		if( auto tile = tileGrid().getTile( position() ).as< ldTile >())
		{
			tile->removeItem( this );
		}
	}
	
	bool Item::canBePickedUpByTouch() const
	{
		return !m_placed && Super::canBePickedUpByTouch();
	}
	
	vec2 Item::bePickedUpBy( Creature& other )
	{
		removeFromTile();
		m_placed = false;
		return Super::bePickedUpBy( other );
	}
	
	void Item::beDroppedBy( Creature& other )
	{
		Super::beDroppedBy( other );
		m_placed = true;		
		addToTile();
	}

	void Item::die()
	{
		if( !isPickedUp() )
		{
			removeFromTile();
		}
		
		Super::die();
		
		markForDeletion();		// TODO!!!
	}
}

