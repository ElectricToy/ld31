//
//  Torch.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Torch_h
#define FreshApp_Torch_h

#include "Essentials.h"
#include "Item.h"

namespace ld
{
	
	class Torch : public Item
	{
		FRESH_DECLARE_CLASS( Torch, Item );
	public:
		
		virtual void update() override;
		
		virtual void onAddedToStage() override;

	protected:
		
	private:
		
		DVAR( real, m_maxLightRadius, 180 );
		DVAR( TimeType, m_deathTime, 0 );
		
		real m_lightWakeupLerp = 0.01f;
	};
	
}

#endif
