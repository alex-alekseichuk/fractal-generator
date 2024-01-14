

#include  <graphics.h>
#include  <math.h> 
 
#include  "fractal.h"
#include  "fracitem.h" 
 
 
void FractalItem::draw(float x0_, float y0_, float x1_, float y1_, float x2_, float y2_)
{
  char b1,b2;
  ITERATOR++;

   
 
  if(ITERATOR==MAX_ITERATION )
    {setcolor(color); moveto((int)x0_,(int)y0_); lineto((int)x0_+x1_,(int)y0_+y1_);}
  else { if((fabs(x1_)<1.6 && fabs(y1_)<1.6) || (fabs(x2_)<1.6 && fabs(y2_)<1.6))
			       putpixel((int)x0_,(int)y0_, color);
	 else frac->draw( x0_ + x0*x1_ + y0*x2_, y0_ + x0*y1_ + y0*y2_,
			  x1*x1_ + y1*x2_, x1*y1_ + y1*y2_,
			  x2*x1_ + y2*x2_, x2*y1_ + y2*y2_ ) ;
       }
  ITERATOR--;
}


void FractalLine::draw(float x0_, float y0_, float x1_, float y1_, float x2_, float y2_)
{
    setcolor(color);
    moveto(x0_ + x1*x1_ - y1*x2_, y0_ + x1*y1_ + y1*y2_ );
    lineto(x0_ + x2*x1_ - y2*x2_, y0_ + x2*y1_ + y2*y2_ );
}


void FractalCircle::draw(float x0_, float y0_, float x1_, float y1_, float x2_, float y2_)
{
    setcolor(color);
    if(fill) {
      float rr;
      setfillstyle(SOLID_FILL,color);
      rr = r*x1_ - r*x2_ ;
      fillellipse(x0_ + x*x1_ - y*x2_, y0_ + x*y1_ + y*y2_, rr, rr);
    } else {
      circle(x0_ + x*x1_ - y*x2_, y0_ + x*y1_ + y*y2_,r*x1_ - r*x2_);
    }
}

