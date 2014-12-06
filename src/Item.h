//
//  Item.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Item_h
#define FreshApp_Item_h

#include "Essentials.h"
#include "ldActor.h"

namespace ld
{
	
	class Item : public ldActor
	{
		FRESH_DECLARE_CLASS( Item, ldActor );
	public:
		
		virtual bool isItem() const override { return true; }

		virtual bool canPickup( const ldActor& other ) const { return false; };

	private:
		
	};
	
}

#endif
