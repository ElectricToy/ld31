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

		virtual void onTouched( ldActor& other ) override;
		virtual void update() override;
		
	protected:

		virtual void die() override;
		
		virtual void updateAI() override;

	private:
				
		DVAR( real, m_awarenessRadius, WORLD_PER_TILE * 24 );
		DVAR( real, m_beginPursuingRadius, WORLD_PER_TILE * 24 );
		DVAR( real, m_giveUpPursuingRadius, WORLD_PER_TILE * 32 );
		VAR( ldActor::wptr, m_pursueee );
		VAR( fr::Vector2i, m_exitDestination );
		VAR( ClassWeights, m_dropItemWeights );
		DVAR( Range< size_t >, m_numDropItemsRange, Range< size_t >( 2, 4 ));
	};
	
}

#endif
