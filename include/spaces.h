/*
 *  spaces.h
 *  Reverse_OCR
 *
 *  Created by sbeyou on 30 juil. 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
 *
 */

#ifndef __SPACES_H__
#define __SPACES_H__

#include "utils.h"
#include "smctc.hh"
#include <CImg.h>
#include <string>
#include <deque>
#include <list>

namespace rocr {
	
	struct line;
	struct text;
	struct stats_word;
	struct Language_heuristics;
	
	// Should I define CAESURA_ILLPOSED? this is related to the semantic of the word (given the language), not related to the "writing" it appears
	typedef enum { CAESURA_UNDEF, CAESURA_NO, CAESURA_ONE, CAESURA_HALF, CAESURA_TWO } caesura_t;
	
	struct word {
		
		//////////
		// Data //
		//////////
		
		// Assimilable parameters, ordered from the more stable to the more volatile
		std::string WF;  ///< Font
		unsigned int WS; ///< Size in pt
		bool3 WB;
		bool3 WI;
		std::string WK;
		caesura_t WC;
		std::list<unsigned int> WD;
		double WY;
		double WX;
		
		// Semi-assimilable parameters
		bool3 WN;
		stats_word* stats;
		
		// Linked parameters, do not touch externally
		std::string WE;
		unsigned int WH;
		unsigned int WW;
		unsigned int WL;
		
		
		/////////////
		// Setters //
		/////////////
		
		void set_WF( std::string& wf ) { WF = wf; }
		void set_WS( unsigned int ws ) { WS = ws; }
		void set_WB( bool3 wb ) { WB = wb; }
		void set_WB( bool wb ) { WB = bool2bool3(wb); }
		void set_WI( bool3 wi ) { WI = wi; }
		void set_WI( bool wi ) { WI = bool2bool3(wi); }
		void set_WE( std::string& we ) { WE = we; }
		void set_WK( const std::string& wk ) {
			
			WK = wk;
			WL = WK.length();
			WW = WL*WS;
		}
		void set_WC( caesura_t wc ) { WC = wc; }
		void set_WC( bool wc, std::string we = "" ) { set_WC( bool2bool3( wc ), we ); }
		void set_WC( bool3 wc, std::string we = "" ) {
			
			WE = wc == B3TRUE && WC != CAESURA_ONE && WC != CAESURA_HALF ? we.length() > 0 ? we : WK : ( WC == CAESURA_ONE || WC == CAESURA_HALF ? WE : "" );
			WC = wc != B3UNDEF ? wc == B3TRUE ? WC == CAESURA_UNDEF || WC == CAESURA_NO || WC == CAESURA_TWO ? CAESURA_ONE : CAESURA_HALF : WC == CAESURA_ONE || WC == CAESURA_HALF ? CAESURA_TWO : CAESURA_NO : CAESURA_UNDEF;
		}
		void set_WD( unsigned int wd ) { WD.push_back( wd ); }
		void set_WN( bool3 wn ) { WN = wn; }
		void set_WN( bool wn ) { WN = bool2bool3(wn); }
		void set_WY( double wy ) { WY = wy; }
		void set_WX( double wx ) { WX = wx; }
		
		void set_WE( const std::string& we = "" ) { WE = WC == CAESURA_UNDEF || WC == CAESURA_NO ? "" : we; }
		void set_WW( unsigned int ww ) { WW = ww; }
		void set_WH( unsigned int wh ) { WH = wh; }
		void set_WL( unsigned int wl ) { WL = wl; }
		
		
		/////////////
		// Private //
		/////////////
		
		// Until authorized, never touch to this variable
		line* parent_line;
	};
	
	typedef smc::particle<word> particle;
	
	struct stats_word {
		
		///////////
		// Means //
		///////////
		
		// Assimilable parameters, ordered from the more stable to the more volatile
		double MWF;
		double MWS;
		double MWB;
		double MWI;
		double MWK;
		double MWC;
		double MWD;
		double MWY;
		double MWX;
		
		// Linked parameters, do not touch externally
		double MWH;
		double MWW;
		double MWL;
		
		// Assimilable parameters, ordered from the more stable to the more volatile
		double SWF;
		double SWS;
		double SWB;
		double SWI;
		double SWK;
		double SWC;
		double SWD;
		double SWY;
		double SWX;
		
		// Linked parameters, do not touch externally
		double SWH;
		double SWW;
		double SWL;
	};
	
	struct line {
		
		std::deque<word> words;
		unsigned int OX;
		unsigned int OY;
		unsigned int OW;
		unsigned int OH;
		text* parent_text;
		
		line();
		
		void addword( word& w );
		
		void update( const word& w );
	};
	
	struct text {
		
		std::deque<line> lines;
		unsigned int OX;
		unsigned int OY;
		unsigned int OW;
		unsigned int OH;
		
		text();
		
		void addline( line& l );
		
		void update( const line& l );
	};
}

#endif /* __SPACES_H__ */
