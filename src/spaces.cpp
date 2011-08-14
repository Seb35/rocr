/*
 *  spaces.cpp
 *  Reverse_OCR
 *
 *  Created by Sébastien Beyou on 30 July 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
 *
 */

#include "../include/spaces.h"

rocr::line::line() : words(), OX(0), OY(0), OW(0), OH(0), parent_text(0) {}

void rocr::line::addword( word& w ) {
	
	words.push_back( w );
	w.parent_line = this;
	update( w );
	parent_text->update( *this );
}

void rocr::line::update( const word& w ) {
	
	if( w.WX < OX ) OX = w.WX;
	if( w.WY < OY+OH ) throw std::exception();
	
	if( OY+OH < w.WY+w.WH ) OH = w.WY + w.WH - OY;
	if( OX+OW < w.WX+w.WW ) OW = w.WX + w.WW - OX;
}

rocr::text::text() : lines(), OX(0), OY(0), OW(0), OH(0) {}

void rocr::text::addline( line& l ) {
	
	lines.push_back( l );
	l.parent_text = this;
	update( l );
}

void rocr::text::update( const line& l ) {
	
	if( l.OX < OX ) OX = l.OX;
	if( l.OY < OY+OH ) throw std::exception();
	
	if( OY+OH < l.OY+l.OH ) OH = l.OY + l.OH - OY;
	if( OX+OW < l.OX+l.OW ) OW = l.OX + l.OW - OX;
}
