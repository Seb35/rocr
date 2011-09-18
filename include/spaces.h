/*
 *  spaces.h
 *  Reverse_OCR
 *
 *  Created by Seb35 on 30 July 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
 *
 */

#ifndef __SPACES_H__
#define __SPACES_H__

#include "utils.h"
#include "smctc.hh"

#include <CImg.h>
#include <unicode/unistr.h>

#include <string>
#include <deque>
#include <list>
#include <map>


/**
In the whole program I use these conventions for the variables:

XY: standard variable; X=group/class; Y=specific attribute
MXY: in the statistics, mean of the variable XY
SXY: in the statistics, standard deviation of the variable XY

----

The caesura is modelized as follows:
UNDEF: undefined state
NO: there is no caesura in the word
ONE: first part of the word when there is a caesura
HALF: when a line contains only the middle of a word with a caesura
TWO: second part of the word when there is a caesura

*/

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
		std::string WF;             ///< Font, string (name)
		unsigned int WS;            ///< Size in pt
		bool3 WB;                   ///< Bold
		bool3 WI;                   ///< Italics
		UnicodeString WK;           ///< Word, Unicode-encoded, only the visible part of the word if there is a caesura
		caesura_t WC;               ///< Caesura state
		std::list<unsigned int> WD; ///< Caesura index
		double WY;                  ///< Word position horizontally
		double WX;                  ///< Word position vertically
		
		// Semi-assimilable parameters
		bool3 WN;                   ///< The word is the first of the line
		stats_word* stats;          ///< Pointer to the current statistics (of the last word)
		
		// Linked parameters, do not touch externally
		UnicodeString WE;             ///< Entire word, Unicode-encoded
		unsigned int WH;            ///< Height, in pixels
		unsigned int WW;            ///< Width, in pixels
		unsigned int WL;            ///< Number of letters
		
		
		///////////////
		// Structors //
		///////////////
		
		word() : WF(), WS(0), WB(B3UNDEF), WI(B3UNDEF), WK(), WC(CAESURA_UNDEF), WD(), WY(0), WX(0), WN(B3UNDEF), stats(0), WE(), WH(0), WW(0), WL(0) {}
		
		/////////////
		// Setters //
		/////////////
		
		void set_WF( std::string& wf ) { WF = wf; }
		void set_WS( unsigned int ws ) { WS = ws; }
		void set_WB( bool3 wb ) { WB = wb; }
		void set_WB( bool wb ) { WB = bool2bool3(wb); }
		void set_WI( bool3 wi ) { WI = wi; }
		void set_WI( bool wi ) { WI = bool2bool3(wi); }
		//void set_WE( const UnicodeString& we ) { WE = we; }
		void set_WK( const UnicodeString& wk ) {
			
			WK = wk;
			WL = WK.length();
			WW = WL*WS;
		}
		void set_WC( caesura_t wc ) { WC = wc; }
		void set_WC( bool wc, UnicodeString we = "" ) { set_WC( bool2bool3( wc ), we ); }
		void set_WC( bool3 wc, UnicodeString we = "" ) {
			
			if( wc != B3UNDEF && ( ( we.length() == 0 && WE.length() == 0 ) || WK.length() == 0 ) ) throw std::exception();
			
			//WE = wc == B3TRUE && WC != CAESURA_ONE && WC != CAESURA_HALF ? we.length() > 0 ? we : WE : ( WC == CAESURA_ONE || WC == CAESURA_HALF ? WE : "" );
			WC = wc != B3UNDEF ? wc == B3TRUE ? WC == CAESURA_UNDEF || WC == CAESURA_NO || WC == CAESURA_TWO ? CAESURA_ONE : CAESURA_HALF : WC == CAESURA_ONE || WC == CAESURA_HALF ? CAESURA_TWO : CAESURA_NO : CAESURA_UNDEF;
			//WE = WC != CAESURA_UNDEF || WC != CAESURA_NO ? WE.length() > 0 ? WE : we : "";
			
			if( WC != CAESURA_UNDEF && WC != CAESURA_NO ) {
				
				if( WC == CAESURA_ONE ) {
					
					if( WD.size() > 0 || we.length() == 0 || WE.length() != 0 ) throw std::exception();
					WE = we;
					
					int32_t d;
					for( d=0; d<WE.length(); d++ ) if( WE[d] != WK[d] ) break;
					if( d == WE.length() ) throw std::exception();
					else WD.push_back( d );
				}
				else if( WC == CAESURA_HALF ) {
					
					if( WD.size() == 0 || WE.length() == 0 ) throw std::exception();
					
					int32_t d;
					for( d=WD.back(); d<WE.length(); d++ ) if( WE[d] != WK[d-WD.back()] ) break;
					if( d == WE.length() ) throw std::exception();
					else WD.push_back( d );
				}
				else if( WC == CAESURA_TWO ) {
					
					if( WD.size() == 0 || WE.length() == 0 ) throw std::exception();
				}
			}
			else {
				
				WE = "";
				WD.clear();
			}
		}
		void set_WD( unsigned int wd ) { WD.push_back( wd ); }
		void set_WN( bool3 wn ) { WN = wn; }
		void set_WN( bool wn ) { WN = bool2bool3(wn); }
		void set_WY( double wy ) { WY = wy; }
		void set_WX( double wx ) { WX = wx; }
		
		//void set_WE( const UnicodeString& we = "" ) { WE = WC == CAESURA_UNDEF || WC == CAESURA_NO ? "" : we; }
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
		
		////////////////////////
		// Standard deviation //
		////////////////////////
		
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
		
		size_t size() { return words.size(); }
	};
	
	struct text {
		
		std::deque<line> lines;
		unsigned int OX;
		unsigned int OY;
		unsigned int OW;
		unsigned int OH;
		
		unsigned int ncolumn;
		unsigned int ntextblock;
		
		text();
		
		void addline( line& l );
		
		void update( const line& l );
		
		size_t size() { return lines.size(); }
	};
	
	template<typename T>
	struct page : public std::map<T,text*> {};
	//typedef std::map<T,text*> page;
	
//	struct page {
//		
//		std::map<T,text*> page;
//		text* operator[]( T o ) { return this->page[o]; }
//	};
}

#endif /* __SPACES_H__ */
