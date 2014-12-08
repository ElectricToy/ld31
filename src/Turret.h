//
//  Turret.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Turret_h
#define FreshApp_Turret_h

#include "Essentials.h"
#include "Item.h"

namespace ld
{
	
	class Turret : public Item
	{
		FRESH_DECLARE_CLASS( Turret, Item );
	public:
		
		virtual void update() override;
		
		virtual void shoot();
		
		virtual vec2 bePickedUpBy( Creature& other ) override;
		virtual void beDroppedBy( Creature& other ) override;
		virtual void beUsedBy( Creature& other ) override;

	private:
		
		DVAR( size_t, m_ammo, 8 );
		DVAR( vec2, m_facingDirection, vec2( 1, 0 ));
	};
	
}

#endif
