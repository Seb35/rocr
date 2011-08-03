/*
 *  heuristics.cpp
 *  Reverse_OCR
 *
 *  Created by sbeyou on 2 août 2011.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */

#include "../include/heuristics.h"

namespace rocr {

Language_heuristics::Language_heuristics(std::string iso639) : iso639(iso639), iso15924(get_iso15924(iso639)) {
	
}

double Language_heuristics::SWB( const particle& particle ) {
	
	const word& value = particle.GetValue();
	
	if( value.WB == TRUE ) return 0.5;
	else return 0.25;
}
		
double Language_heuristics::SWI( const particle& particle ) {
	
	const word& value = particle.GetValue();
	
	if( value.WI == TRUE ) return 0.5;
	else return 0.3;
}

std::list<std::string> get_iso15924( std::string iso639 ) {
	
	std::list<std::string> l;
	
	switch( iso639 ) {
		
		case "de":
		case "en":
		case "fr":
			l.push_back( "Latn" );
			break;
		
		case "ru":
			l.push_back( "Cyrl" );
			break;
	}
	
	return l;
}

double Language_fr::SWB( const particle& particle ) {
	
	const word& value = particle.GetValue();
	
	if( value.WK == "«" ) return 0.3;
	else if( value.WK == "»" && value.WB == TRUE ) return 0.1;
	else if( value.WB == TRUE ) return 0.4;
	else return 0.25;
}

double Language_fr::SWI( const particle& particle ) {
	
	const word& value = particle.GetValue();
	
	if( value.WK == "«" ) return 0.375;
	else if( value.WK == "»" && value.WB == TRUE ) return 0.1;
	else if( value.WB == TRUE ) return 0.5;
	else return 0.25;
}
}
