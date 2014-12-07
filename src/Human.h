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

		virtual bool canStep( const vec2& dir ) const override;
		
		virtual bool canPickupTouchingActor() const override;
		virtual bool canDropHeldActor() const override;
		virtual bool canUseHeldActor() const;
		
		virtual void dropHeldActor() override;
		virtual void placeHeldActor();
		
	protected:
		
		virtual void updateAI() override;
		virtual real currentStepSpeed() const override;
	
	private:
		
	};
	
}

#endif
