///////////////////////////////////////////////////////////////////////////
//
//        Fractal Header File
//        Copyright (C) Alexander Alexeychuk 1997
//
///////////////////////////////////////////////////////////////////////////


#ifndef __FRACTAL_H__
#define __FRACTAL_H__


#ifndef BOOL          // Boolean definitions
#define BOOL  char
#define TRUE  1
#define FALSE 0
#endif


extern unsigned char MAX_ITERATION ;
extern unsigned char ITERATOR ;


#include "fracitem.h"

#ifndef PI
#define PI 3.1415
#endif



class Fractal {

  //  Friends
  friend class AbstarctFractalItem;
  friend class FractalScene;
  
protected :

  AbstractFractalItem *list, *last;      // pointers to first & last fractal items 

  Fractal *link;    // next Frcatal
  char *name;       // Fractal name
  
public :

  Fractal(char *);  // constructor
  ~Fractal(); 
  
  // functions for add some fractal items ------------------------------------------
  // -------------------------------------------------------------------------------
  virtual void additem(float x0, float y0, float x1, float y1, float x2, float y2, 
                       unsigned char color = 15, Fractal *f=NULL);
  virtual void addline(float x1, float y1, float x2, float y2, unsigned char color = 15);  
  virtual void addcircle(float x, float y, float r,  unsigned char color = 15, BOOL fill=TRUE);
  virtual void draw(float x0, float y0, float x1, float y1, float x2, float y2);  // draw fractal function

}; // Fractal



class FractalScene {

  friend class Fractal;
  
protected:
  
  Fractal *list;  // pointers to head and main elements in fractal scene
  
  int create_item(Fractal *f);  // special for open method
  void create_fractal();   //  special for open method
  
public:

  float x0, y0, x1, y1, x2, y2;  // Begin Parameters

  FractalScene();   // Constructor

  void add(Fractal *f) {f->link=list;list=f;}        // add fractal to the fractal list
  void draw(float x0_=0, float y0_=0, float x1_=0, float y1_=0, float x2_=0, float y2_=0);
  
  Fractal *get(char *);                   // get pointer to fractal for name      
                                                                                       
  void clear();
  int open(char *);  // open simple fractal file Return : 0 -  Error


}; // FrcatalScene 


#endif 
        

