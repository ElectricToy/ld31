//
//  Item.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Item.h"
#include "ldTile.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Item )
	
	DEFINE_DVAR( Item, bool, m_placed );
	DEFINE_DVAR( Item, real, m_navDistanceScalar );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Item )
	
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
	
}

