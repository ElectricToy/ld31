//
//  Door.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Door_h
#define FreshApp_Door_h

#include "Essentials.h"
#include "Item.h"

namespace ld
{
	
	class Door : public Item
	{
		FRESH_DECLARE_CLASS( Door, Item );
	public:
		
		virtual void update() override;
		virtual void onTouched( ldActor& other ) override;
		
	private:
		
	};
	
}

#endif
