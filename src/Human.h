//
//  Human.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Human_h
#define FreshApp_Human_h

#include "Essentials.h"
#include "Creature.h"

namespace ld
{
	
	class Human : public Creature
	{
		FRESH_DECLARE_CLASS( Human, Creature );
	public:
		
		virtual bool canPickup( const ldActor& other ) const override;
		
		virtual bool isHuman() const override { return true; }
		virtual bool isPlayer() const override;
		
	private:
		
	};
	
}

#endif