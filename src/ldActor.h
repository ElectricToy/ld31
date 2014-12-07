//
//  ldActor.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_ldActor_h
#define FreshApp_ldActor_h

#include "Essentials.h"
#include "Actor.h"
#include "TileGrid.h"
#include "ActorController.h"

namespace ld
{
	class ldWorld;
	class Creature;
	
	class ldActor : public fr::Actor
	{
		FRESH_DECLARE_CLASS( ldActor, Actor );
	public:
		
		virtual void update() override;

		virtual bool isPlayer() const { return false; }
		virtual bool isHuman() const { return false; }
		virtual bool isMonster() const { return false; }
		virtual bool isCreature() const { return false; }
		virtual bool isItem() const { return false; }
		
		virtual bool mayCollide() const;
	
		virtual void onAddedToStage() override;
		
		virtual void onTouched( ldActor& other );
		
		virtual bool canBePickedUp() const;
		virtual bool canBePickedUpByTouch() const;
		bool isPickedUp() const;
		
		virtual vec2 bePickedUpBy( Creature& other );
		virtual void beDroppedBy( Creature& other );
		
	protected:

		ldWorld& world() const;
		fr::TileGrid& tileGrid() const;
	
	private:

		DVAR( vec2, m_carryOffset, vec2( 5, -5 ));
		DVAR( vec2, m_carryScale, vec2( 0.75f ));
		DVAR( angle, m_carryRotation, 30 );
		DVAR( vec2, m_precarryScale, vec2( 1 ));
		
		VAR( WeakPtr< Creature >, m_holder );
		
	};
	
}

#endif