//
//  ldTileTemplate.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldTileTemplate.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldTileTemplate )
	DEFINE_VAR( ldTileTemplate, ClassWeights, m_itemClassWeights );
	DEFINE_DVAR( ldTileTemplate, real, m_pctChanceToSpawn );
	DEFINE_DVAR( ldTileTemplate, bool, m_mayReceiveItem );
	DEFINE_DVAR( ldTileTemplate, bool, m_isMonsterSpawner );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldTileTemplate )
	
	ClassInfo::cptr ldTileTemplate::randomTileClass() const
	{
		if( !m_itemClassWeights.empty() && pctChance( m_pctChanceToSpawn ))
		{
			return randomClass( m_itemClassWeights );
		}
		else
		{
			return nullptr;
		}
	}
	
}

