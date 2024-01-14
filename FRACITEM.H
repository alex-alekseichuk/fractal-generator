///////////////////////////////////////////////////////////////////////////
//
//        FractalItem Header File
//        Copyright (C) Alexander Alexeychuk 1997
//
///////////////////////////////////////////////////////////////////////////


#ifndef __FRACITEM_H__
#define __FRACITEM_H__

#include  <stdlib.h>

#include "fractal.h"


////////////////////////////////////////////////////////////////////////////
//
//   Abstract Fractal Item Class Definition
//                    
class AbstractFractalItem 
{

  friend class Fractal;

protected :

  AbstractFractalItem *link;  // Next Fractal Item

public :


  AbstractFractalItem() { link = NULL; }     //  Contructors
  AbstractFractalItem(AbstractFractalItem& fi) { link = fi.link; fi.link = this; }
  
  virtual void draw(float, float, float, float, float, float) {}

  

}; // AbstractFractalItem
                    
////////////////////////////////////////////////////////////////////////////
//
//
//                    
                    
class FractalItem : public AbstractFractalItem 
{

  friend class Fractal;

protected :

  float x0, y0, x1, y1, x2, y2;     // Coords of fractal item

  unsigned char color;        // color of fractal item

  Fractal *frac;   // fractal in this fractal item

public :

  void draw(float, float, float, float, float, float);   // virtual drawer 

}; // FractalItem


////////////////////////////////////////////////////////////////////////////
//
//
//
                         
class FractalLine : public AbstractFractalItem 
{

  friend class Fractal;

protected :
      
  float x1 ,     // Coords of fractal line
        y1 ,
        x2 ,
        y2 ; 
  
  unsigned char color;      

public :

  void draw(float, float, float, float, float, float);

}; // FractalLine


////////////////////////////////////////////////////////////////////////////
//
//
//
                         
class FractalCircle : public AbstractFractalItem 
{

  friend class Fractal;

protected :
      
  float x ,     // Coords of fractal line
        y ,
	r  ;

  BOOL fill;
  
  unsigned char color;      

public :

  void draw(float, float, float, float, float, float);

}; // FractalCircle


#endif
