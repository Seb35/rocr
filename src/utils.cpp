/*
 *  utils.cpp
 *  Reverse_OCR
 *
 *  Created by sbeyou on 1 août 2011.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */

#include "../include/utils.h"
#include <exception>

bool bool32bool( bool3 b ) {
	
	if( b == FALSE ) return false;
	if( b == TRUE ) return true;
	throw std::exception();
	return false;
}

bool3 bool2bool3( bool b ) {
	
	if( b == false ) return FALSE;
	if( b == true ) return TRUE;
	return UNDEF;
}
