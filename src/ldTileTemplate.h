//
//  ldTileTemplate.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_ldTileTemplate_h
#define FreshApp_ldTileTemplate_h

#include "Essentials.h"
#include "TileGrid.h"

namespace ld
{
	class ldTileTemplate : public fr::TileTemplate
	{
		FRESH_DECLARE_CLASS( ldTileTemplate, TileTemplate );
	public:
		
		SYNTHESIZE_GET( bool, mayReceiveItem );
		SYNTHESIZE_GET( bool, isMonsterSpawner );
		
		ClassInfo::cptr randomTileClass() const;		// May return null.
		
	private:
		
		VAR( ClassWeights, m_itemClassWeights );
		DVAR( real, m_pctChanceToSpawn, 5 );
		DVAR( bool, m_mayReceiveItem, true );
		DVAR( bool, m_isMonsterSpawner, false );
		
		
	};
	
}

#endif
