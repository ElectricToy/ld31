//
//  Mine.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Mine_h
#define FreshApp_Mine_h

#include "Essentials.h"
#include "Item.h"

namespace ld
{
	
	class Mine : public Item
	{
		FRESH_DECLARE_CLASS( Mine, Item );
	public:
		
		virtual void update() override;

		bool isFuseStarted() const;
		virtual void startFuse();
		virtual void explode();
		
		rect dangerArea() const;
		
	private:
		
		DVAR( real, m_dangerRadius, 64 );
		DVAR( real, m_explodeRadius, 32 );
		DVAR( real, m_maxDamage, 16 * 16 * 8 );
		DVAR( bool, m_harmsHumans, true );
		DVAR( TimeType, m_fuseDuration, 1.35 );

		FRESH_DECLARE_CALLBACK( Mine, onTimeToExplode, fr::Event );
	};
	
}

#endif
