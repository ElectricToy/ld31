//
//  Creature.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Creature_h
#define FreshApp_Creature_h

#include "Essentials.h"
#include "ldActor.h"
#include "Inventory.h"

namespace ld
{
	
	class Creature : public ldActor
	{
		FRESH_DECLARE_CLASS( Creature, ldActor );
	public:
		
		SYNTHESIZE_GET( bool, alive );
		
		virtual bool isCreature() const override { return true; }
		
		virtual bool mayCollide() const override;
		
		virtual void onTouched( ldActor& other ) override;

		virtual bool canBePickedUp() const;
		
		virtual bool canPickup( const ldActor& other ) const;
		virtual void pickup( ldActor& other );
		
		virtual void pickupTouchingActor();
		virtual void dropHeldActor();
		
		virtual void update() override;
		
	protected:
		
		virtual void updateAI() {}
		
	private:

		VAR( ldActor::ptr, m_heldActor );
		VAR( Inventory::ptr, m_inventory );
		DVAR( bool, m_alive, true );
	};
	
}

#endif
