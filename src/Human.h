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

		SYNTHESIZE_GET( int, controlPriority );
		
		virtual int desiredDepth() const override;

		
		virtual real lightWobble() const override;
		virtual real normalLightRadius() const override;
		
		virtual bool canPickup( const ldActor& other ) const override;
		
		virtual bool isHuman() const override { return true; }
		virtual bool isPlayer() const override;

		virtual bool canStep( const vec2& dir ) const override;		
		
		virtual void die() override;
		
		virtual vec2 bePickedUpBy( Creature& other ) override;
		virtual void beDroppedBy( Creature& other ) override;

	protected:

		virtual real grindDamage() const override;

		virtual void updateAI() override;
		virtual real currentStepSpeed() const override;
	
	private:
		
		DVAR( int, m_controlPriority, 0 );
		DVAR( bool, m_male, false );
		
	};
	
}

#endif
