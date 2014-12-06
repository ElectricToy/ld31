//
//  Inventory.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Inventory_h
#define FreshApp_Inventory_h

#include "Essentials.h"
#include "ldActor.h"

namespace ld
{
	
	class Inventory : public fr::Object
	{
		FRESH_DECLARE_CLASS( Inventory, Object );
	public:
		
		bool has( ldActor::ptr item ) const;

		void take( ldActor::ptr item );
		void drop( ldActor::ptr item, const vec2& pos );
		
		template< typename FunctionT >
		void eachItem( FunctionT&& fn )
		{
			for( auto item : m_items )
			{
				fn( item );
			}
		}

		template< typename FunctionT >
		void eachItem( FunctionT&& fn ) const
		{
			for( auto item : m_items )
			{
				fn( item );
			}
		}

	private:
		
		VAR( std::vector< ldActor::ptr >, m_items );
		
	};
	
}

#endif
