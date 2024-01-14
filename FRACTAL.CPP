///////////////////////////////////////////////////////////////////////////
//
//        Fractal Source File
//        Copyright (C) Alexander Alexeychuk 1997
//
///////////////////////////////////////////////////////////////////////////




#include "fractal.h"
#include "fracitem.h"


#include  <stdlib.h>
#include  <fstream.h>
#include  <math.h>
#include  <conio.h> 
#include  <graphics.h>
#include  <ctype.h>
#include  <string.h>

                                       
unsigned char MAX_ITERATION = 30;
unsigned char ITERATOR = 0;
                                       

//----------------------------------------------------------------------------------------------
//
//      Fractal Class
//
/////////////////////////////////////////////////////////////////////////////////////////////



Fractal::Fractal(char *name_)     // constructor
{                                           
  link=NULL; list=last=NULL; name = new char[strlen(name_)+1]; strcpy(name,name_);
}  


Fractal::~Fractal() 
{ 
  AbstractFractalItem *afi;
  delete [sizeof(name)] name; 
  while(list) { afi=list->link; delete list; list=afi; }
}
                                       
void Fractal::additem(float x0, float y0, float x1, float y1, float x2,float y2,
                      unsigned char color, Fractal *f)
{
  FractalItem *it = new FractalItem;

  it->frac=(f?f:this);

  it->x0   = x0; it->y0   = y0;  
  it->x1   = x1; it->y1   = y1;
  if(x2==0 && y2==0) {it->x2 = -y1; it->y2 = x1; } else {it->x2 = x2; it->y2 = y2; }
  
  it->color = color;

  if(list) { last->link=it; last=it; }
  else { list=last=it; }

} // Fractal::additem


void Fractal::addline(float x1, float y1, float x2, float y2, unsigned char color)
{
  FractalLine *it = new FractalLine;

  it->x1 = x1;
  it->y1 = y1;
  it->x2 = x2;
  it->y2 = y2;
  
  it->color = color;

  if(list) { last->link=it; last=it; }
  else { list=last=it; }

} // Fractal::addline

 
void Fractal::addcircle(float x, float y, float r,  unsigned char color , BOOL fill)
{
  FractalCircle *it = new FractalCircle;

  it->x = x;
  it->y = y;
  it->r = r;
  it->fill = fill;
  it->color = color;

  if(list) { last->link=it; last=it; }
  else { list=last=it; }
} // Fractal::addcircle
  
 
void Fractal::draw(float x0, float y0, float x1, float y1, float x2, float y2)
{
  AbstractFractalItem *it;

  if(kbhit() || !list) return;

  for(it=list;it!=NULL;it=it->link) it->draw(x0, y0, x1, y1, x2, y2);
} // Fractal::draw
    
//----------------------------------------------------------------------------------------------
//
//      Fractal Scene Class
//
/////////////////////////////////////////////////////////////////////////////////////////////

    
FractalScene::FractalScene() 
{ 
  list=NULL;  x0=0; y0=300; x1=600; y1=0; x2=0; y2=0; 
}     
    
 
void FractalScene::draw(float x0_, float y0_, float x1_, float y1_, float x2_, float y2_)
{
  Fractal *f=get("MAIN");
  if(!f) { if(list) f=list; else return; }

  f->draw(x0, y0, x1, y1, x2, y2);
}
 
    
Fractal *FractalScene::get(char *name_)
{
  Fractal *f;
  for(f=list;f;f=f->link) if(0==strcmp(name_, f->name)) return f;
  return NULL;
}    
    

void FractalScene::clear()
{
  Fractal *f;
  while(list) { f=list->link; delete list; list=f; }
}
  
  
/////////////////////////////////////////////////////////////////////////////////////////////
//
//    Open Simple FRactal Text File
//
//-------------------------------------------------------------------------------------------

        
static enum TYPE_TOK { _END, _FRAC, _ITEM, _LINE, _END_FILE, _CIRCLE,
                       _LET, _EXPR, _LITERAL, _NONE } curr_tok = _NONE;
           
static ifstream file;

static char str_value[40] = "";
static float number_value = 0;




static TYPE_TOK get_token()
{
  char ch;
  
  do {
    if(!file.get(ch)) return _END;
    if(ch=='/')  do { if(!file) return _END; file.get(ch);} while('\n'!=ch);
  } while(ch==' ' || ch=='\n');

  switch(ch) {
              
    case '=' :
      return curr_tok=_LET;          
              
    case '0': case '1': case '2': case '3': case '4': case '-':
    case '5': case '6': case '7': case '8': case '9': case '.': 
      file.putback(ch);
      file >> number_value; 
      return curr_tok=_EXPR;
      
    default :
      if(isalpha(ch)) {
        char *p=str_value;
        if(ch>=97&&ch<=122)ch-=32;
        *p++ = ch;
        while(file.get(ch) && (ch=='_'||isalnum(ch))) {if(ch>=97&&ch<=122)ch-=32; *p++ = ch;}
        file.putback(ch);
        *p=0;
        //cout << " STR : " << str_value <<endl; getch();
        if(0==strcmp(str_value,"FRACTAL") || 0==strcmp(str_value,"F")) return curr_tok=_FRAC;
        if(0==strcmp(str_value,"ITEM") || 0==strcmp(str_value,"I")) return curr_tok=_ITEM;
        if(0==strcmp(str_value,"LINE") || 0==strcmp(str_value,"L")) return curr_tok=_LINE;
        if(0==strcmp(str_value,"CIRCLE") || 0==strcmp(str_value,"CI")) return curr_tok=_CIRCLE;
        if(0==strcmp(str_value,"END") || 0==strcmp(str_value,"E")) return curr_tok=_END;
        
        return curr_tok=_LITERAL;

      } else return _NONE;  
  } // switch(ch)
} // get_token


float get_value()
{ 
  if(get_token() == _LET) get_token(); 
  if(curr_tok == _EXPR) return number_value; else return 0;
}
      
char *get_literal()
{ 
  if(get_token() == _LET) get_token(); 
  if(curr_tok == _LITERAL) return str_value; else return NULL;
}      
                         
int FractalScene::create_item(Fractal *f)
{
  float x0_=0,y0_=0, x1_=0,y1_=0, x2_=0,y2_=0;
  unsigned char color_=15;
  Fractal *fractal_ = NULL;                                              
                                                 
  while(get_token()!=_END) {
    if(curr_tok==_LITERAL) {
      if(0==strcmp(str_value, "COLOR") || 0==strcmp(str_value, "C"))
         { color_=get_value(); continue;}
      if(0==strcmp(str_value, "X0")){ x0_=get_value(); continue;}  
      if(0==strcmp(str_value, "Y0")){ y0_=-get_value(); continue;}      
      if(0==strcmp(str_value, "X1")){ x1_=get_value(); continue;}  
      if(0==strcmp(str_value, "Y1")){ y1_=-get_value(); continue;}
      if(0==strcmp(str_value, "X2")){ x2_=get_value(); continue;}  
      if(0==strcmp(str_value, "Y2")){ y2_=-get_value(); continue;}
    }
    if(curr_tok==_FRAC)
       if(get_literal()) fractal_=get(str_value); 

  } // while
  f->additem(x0_, y0_, x1_, y1_, x2_, y2_, color_, fractal_);
  return 0;
} // create_item
                         

int create_line(Fractal *f)
{
  float x1=0.0, y1=0.0, x2=1.0, y2=0.0;
  unsigned char color=15;
                        
  while(get_token()!=_END) 
    if(curr_tok==_LITERAL) {
      if(0==strcmp(str_value, "COLOR") || 0==strcmp(str_value, "C"))
                                            { color=get_value(); continue;}
      if(0==strcmp(str_value, "X1")){ x1=get_value(); continue;}  
      if(0==strcmp(str_value, "Y1")){ y1=-get_value(); continue;}
      if(0==strcmp(str_value, "X2")){ x2=get_value(); continue;}  
      if(0==strcmp(str_value, "Y2")){ y2=-get_value(); continue;}
    }  
  
  f->addline(x1, y1, x2, y2, color);
  return 0;
} // create_line

int create_circle(Fractal *f)
{
  float x=0, y=0, r=1;
  BOOL fill=TRUE;
  unsigned char color=15;
                        
  while(get_token()!=_END) 
    if(curr_tok==_LITERAL) {
      if(0==strcmp(str_value, "COLOR") || 0==strcmp(str_value, "C"))
                                { color=get_value(); continue;}
      if(0==strcmp(str_value, "X")){ x=get_value(); continue;}  
      if(0==strcmp(str_value, "Y")){ y=-get_value(); continue;}
      if(0==strcmp(str_value, "R")){ r=get_value(); continue;}  
      if(0==strcmp(str_value, "FILL")  || 0==strcmp(str_value, "FIL"))
                                 { fill=get_value()?TRUE:FALSE; continue;}
    }  
  
  f->addcircle(x, y, r, color, fill);
  return 0;
} // create_circle                       

void FractalScene::create_fractal()
{
  Fractal *f;                              
      
  get_token(); 
  f=get(str_value);
  if(!f) { f = new Fractal(str_value); add(f); }

  while(get_token()!=_END) 
    switch(curr_tok) {
      case _ITEM :
        create_item(f); break;
      case _LINE :
        create_line(f); break;              
      case _CIRCLE :
        create_circle(f); break;   
        
      case _LITERAL :         
        if(0==strcmp(str_value, "X0")){ x0=get_value(); break;;}
        if(0==strcmp(str_value, "Y0")){ y0=get_value(); break;;}
        if(0==strcmp(str_value, "X1")){ x1=get_value(); break;;}
        if(0==strcmp(str_value, "Y1")){ y1=get_value(); break;;}        
        if(0==strcmp(str_value, "X2")){ x2=get_value(); break;;}
        if(0==strcmp(str_value, "Y2")){ y2=get_value(); break;;}              
        break;                 
              
    } // switch 
} // create_fractal                                                
                 

int FractalScene::open(char *filename)
{
  file.open(filename);
  char ch;

  if(!file) return 1; // error : can't open file
  
  for(get_token();file;get_token()) if(curr_tok==_FRAC) create_fractal();
  
  if(x2==0 && y2==0) {x2=-y1; y2=x1;}
  
  return 0;
} // Fractal::open
