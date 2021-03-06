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

		SYNTHESIZE_GET( bool, placed );
		SYNTHESIZE_GET( bool, blocksMonsters );
		SYNTHESIZE_GET( bool, blocksHumans );
		
		real navDistanceScalar() const;
		
		virtual bool isItem() const override { return true; }
		
		virtual bool doesBlock( const Creature& creature ) const;
		
		virtual bool canBePickedUpByTouch() const override;
		virtual vec2 bePickedUpBy( Creature& other ) override;
		virtual void beDroppedBy( Creature& other ) override;
		virtual void beUsedBy( Creature& other ) override;
		
		virtual void onAddedToStage() override;

		virtual void receiveDamage( real amount ) override;
		
		virtual void showProvisionFlash();
		
	protected:
		
		void addToTile();
		void removeFromTile();
		
		virtual void die() override;

		void showAsPlaced( bool placed );
		void updateCracks();
		
	private:
		
		DVAR( bool, m_blocksHumans, false );
		DVAR( bool, m_blocksMonsters, false );
		DVAR( bool, m_placed, false );
		DVAR( real, m_navDistanceScalar, 1.0f );
		VAR( fr::DisplayObjectContainer::ptr, m_cracksHost );
		DVAR( bool, m_mayBePlaced, true );
		VAR( ClassInfo::cptr, m_provisionFlashLightSourceClass );
		DVAR( real, m_placedRotation, 30 );
		DVAR( vec2, m_placedScale, vec2( 0.75f ));
		
		fr::MovieClip::ptr m_cracks;
	};
	
}

#endif
