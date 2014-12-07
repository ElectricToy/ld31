//
//  Item.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Item_h
#define FreshApp_Item_h

#include "Essentials.h"
#include "ldActor.h"

namespace ld
{
	
	class Item : public ldActor
	{
		FRESH_DECLARE_CLASS( Item, ldActor );
	public:
		
		real navDistanceScalar() const;
		
		virtual bool isItem() const override { return true; }
		
		virtual bool doesBlock( const Creature& creature ) const;

		virtual bool canBePickedUpByTouch() const override;
		virtual vec2 bePickedUpBy( Creature& other ) override;
		virtual void beDroppedBy( Creature& other );
		
		virtual void onAddedToStage() override;

		virtual void receiveDamage( real amount ) override;
		
	protected:
		
		void addToTile();
		void removeFromTile();
		
		virtual void die() override;

		void showAsPlaced( bool placed );
		void updateCracks();
		
	private:
		
		DVAR( bool, m_placed, false );
		DVAR( real, m_navDistanceScalar, 1.0f );
		DVAR( bool, m_blocksHumans, false );
		DVAR( bool, m_blocksMonsters, false );
		VAR( fr::DisplayObjectContainer::ptr, m_cracksHost );
		
		fr::MovieClip::ptr m_cracks;
	};
	
}

#endif
