/*
 *  heuristics.h
 *  Reverse_OCR
 *
 *  Created by sbeyou on 2 août 2011.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */

#ifndef __HEURISTICS_H__
#define __HEURISTICS_H__

#include "spaces.h"
#include <list>
#include <string>

namespace rocr {
	
	struct Language_heuristics {
		
		std::string iso639;
		std::list<std::string> iso15924;
		
		Language_heuristics();
		
		void initialize( std::string iso639 );
		
		static std::list<std::string> get_iso15924( std::string iso639 );
		
		virtual double SWB( const particle& particle );
		
		virtual double SWI( const particle& particle );
	};
	
	struct Language_fr : public Language_heuristics {
		
		virtual double SWB( const particle& particle );
		
		virtual double SWI( const particle& particle );
	};
}

#endif /* __HEURISTICS_H__ */
