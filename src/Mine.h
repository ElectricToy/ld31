//
//  Mine.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Mine_h
#define FreshApp_Mine_h

#include "Essentials.h"
#include "Item.h"

namespace ld
{
	
	class Mine : public Item
	{
		FRESH_DECLARE_CLASS( Mine, Item );
	public:
		
		virtual void update() override;
		
	private:
		
	};
	
}

#endif