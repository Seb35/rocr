/*
 *  heuristics.h
 *  Reverse_OCR
 *
 *  Created by Sébastien Beyou on 2 August 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
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
		
		Language_heuristics( std::string iso639 );
		
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
