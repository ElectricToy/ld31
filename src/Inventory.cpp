//
//  Inventory.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Inventory.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Inventory )

	DEFINE_VAR( Inventory, std::vector< ldActor::ptr >, m_items );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Inventory )
	
	bool Inventory::has( ldActor::ptr item ) const
	{
		return std::find( m_items.begin(), m_items.end(), item ) != m_items.end();
	}
	
	void Inventory::take( ldActor::ptr item )
	{
		ASSERT( !has( item ));
		m_items.push_back( item );
	}
	
	void Inventory::drop( ldActor::ptr item, const vec2& pos )
	{
		const auto iter = std::find( m_items.begin(), m_items.end(), item );
		ASSERT( iter != m_items.end() );
		
		m_items.erase( iter );
		
		item->position( snapToGrid( pos ));
		
		ASSERT( !has( item ));
	}
}

