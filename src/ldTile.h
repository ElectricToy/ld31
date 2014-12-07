//
//  ldTile.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_ldTile_h
#define FreshApp_ldTile_h

#include "Essentials.h"
#include "TileGrid.h"
#include "Item.h"

namespace ld
{
	
	class ldTile : public fr::Tile
	{
		FRESH_DECLARE_CLASS( ldTile, Tile );
	public:
		
		void addItem( Item::ptr item );
		void removeItem( Item::ptr item );
		
		virtual real navDistanceScalar() const override
		{
			return Super::navDistanceScalar() * ( m_containedItem ? m_containedItem->navDistanceScalar() : 1.0f );
		}
		
	private:
		
		VAR( Item::wptr, m_containedItem );
		
	};
	
}

#endif
