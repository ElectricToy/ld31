//
//  Monster.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Monster_h
#define FreshApp_Monster_h

#include "Essentials.h"
#include "Creature.h"

namespace ld
{
	
	class Monster : public Creature
	{
		FRESH_DECLARE_CLASS( Monster, Creature );
	public:

		virtual bool isMonster() const override { return true; }
		
		virtual bool canPickup( const ldActor& other ) const override;

		virtual void update() override;
		
	protected:
		
		virtual void updateAI() override;

	private:
		
		fr::TileGrid::WorldSpacePath m_worldSpacePath;
	};
	
}

#endif
