/*
 *  heuristics.cpp
 *  Reverse_OCR
 *
 *  Created by Seb35 on 2 August 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
 *
 */

#include "../include/heuristics.h"

namespace rocr {

Language_heuristics::Language_heuristics( std::string iso639 ) : iso639(iso639), iso15924(get_iso15924(iso639)) {
	
}

double Language_heuristics::SWB( const particle& particle ) {
	
	const word& value = particle.GetValue();
	
	if( value.WB == B3TRUE ) return 0.5;
	else return 0.25;
}
		
double Language_heuristics::SWI( const particle& particle ) {
	
	const word& value = particle.GetValue();
	
	if( value.WI == B3TRUE ) return 0.5;
	else return 0.3;
}

std::list<std::string> Language_heuristics::get_iso15924( std::string iso639 ) {
	
	std::list<std::string> l;
	
	     if( iso639 == "de" ) l.push_back( "Latn" );
	else if( iso639 == "en" ) l.push_back( "Latn" );
	else if( iso639 == "fr" ) l.push_back( "Latn" );
	else if( iso639 == "la" ) l.push_back( "Latn" );
	else if( iso639 == "ru" ) l.push_back( "Cyrl" );
	
	return l;
}

double Language_fr::SWB( const particle& particle ) {
	
	const word& value = particle.GetValue();
	
	if( value.WK == "«" ) return 0.3;
	else if( value.WK == "»" && value.WB == B3TRUE ) return 0.1;
	else if( value.WB == B3TRUE ) return 0.4;
	else return 0.25;
}

double Language_fr::SWI( const particle& particle ) {
	
	const word& value = particle.GetValue();
	
	if( value.WK == "«" ) return 0.375;
	else if( value.WK == "»" && value.WB == B3TRUE ) return 0.1;
	else if( value.WB == B3TRUE ) return 0.5;
	else return 0.25;
}
}
