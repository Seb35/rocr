/*
 *  utils.h
 *  Reverse_OCR
 *
 *  Created by sbeyou on 1 août 2011.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */

#ifndef __UTILS_H__
#define __UTILS_H__

typedef enum { FALSE=0, TRUE=1, UNDEF=-1 } bool3;

bool bool32bool( bool3 b );

bool3 bool2bool3( bool b );

#endif /* __UTILS_H__ */
