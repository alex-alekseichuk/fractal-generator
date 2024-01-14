

#include <iostream.h>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>


#include   "fractal.h"
#include   "fracitem.h"





FractalScene MyFrac;





// Global Parameters

enum WHATER {_NONE, _MAX_ITERATION} ;

char ChangeParameter(WHATER what, float x)
{
  switch(what) {
    case _NONE : return(0);

    case _MAX_ITERATION : MAX_ITERATION=(unsigned char)x;break;

  } // switch
  return(1);
}



void main(int argc, char *argv[]) {

  int i;

  /* request auto detection */
  int gdriver = DETECT, gmode, errorcode;


  if(argc<2) { cout << "Invalid Parameter \n" << endl; exit(1); }

   MyFrac.open(argv[1]);


    /* initialize graphics mode */
    initgraph(&gdriver, &gmode, "");

    /* read result of initialization */
    errorcode = graphresult();

    if (errorcode != grOk)  /* an error occurred */
    {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1);             /* return with error code */
    }


  // Our Parameters

  for(i=2;i<argc;i++) {
    char *s=argv[i], *ss;
    BOOL dod=FALSE;
    float _float;
    WHATER what = _NONE;

    for(ss=s;*ss!='\0';ss++) {
      switch(*ss){
	case '-' :
	case '/' :
	  if(dod) ChangeParameter(what, _float);
	  what = _NONE;
	  break;



	case 'M' :
	case 'm' :
	  what=_MAX_ITERATION; dod = TRUE; break;

	case '=' :
	  _float=atof(ss+1);dod=FALSE; ChangeParameter(what, _float); break;

      } // switch

    } // for(ss)


  } // for(i)

  MyFrac.draw();


  while(kbhit()) getch();
  getch();

  closegraph();
}