//
//  ldTile.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldTile.h"
#include "ldTileTemplate.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldTile )
	DEFINE_VAR( ldTile, Item::wptr, m_containedItem );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldTile )

	bool ldTile::mayReceiveItem() const
	{
		auto ldTemplate = static_cast< const ldTileTemplate* >( tileTemplate().get() );
		return !m_containedItem && !isSolid() && ldTemplate->mayReceiveItem();
	}
	
	void ldTile::addItem( Item::ptr item )
	{
		ASSERT( item );
		ASSERT( !m_containedItem );
		
		m_containedItem = item;
	}
	
	void ldTile::removeItem( Item::ptr item )
	{
		ASSERT( m_containedItem == item );
		m_containedItem = nullptr;
	}

	Item::ptr ldTile::createRandomItem() const
	{
		auto ldTemplate = static_cast< const ldTileTemplate* >( tileTemplate().get() );
		
		if( auto tileClass = ldTemplate->randomTileClass())
		{
			return createObject< Item >( *tileClass );
		}
		else
		{
			return nullptr;
		}
	}
}

