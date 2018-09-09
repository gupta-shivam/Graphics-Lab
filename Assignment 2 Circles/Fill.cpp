#include <windows.h>
#include "Fill.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

void fill();

/*  function to round of the value*/
double roundVal(double x)
{
  int ix;
  if (x >= 0) 
    ix = (int)(x + 0.5);
  else
    ix = (int)(x - 0.5);
  return (double)(ix);
}
/* function to swap the values*/
void swap(int &x, int &y)
{ 
  int tmp;
  tmp = x;
  x = y;
  y = tmp;
}

/* function to enable and disable the options in the drop down menu depending upon the mode*/
void setupMenus(HWND hwnd)
{
  HMENU hmenu, hsubmenu;
  hmenu=GetMenu(hwnd);
  hsubmenu=GetSubMenu(hmenu, 0);

  switch (gDrawData.drawMode)
  {
    case READY_MODE :
      // enable 'Draw Polygon', disable 'Filling' menu
      EnableMenuItem(hsubmenu,ID_DRAW_POLY,
                     MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hsubmenu,ID_FILL,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAW_MODE :
    case FILL_MODE :
    case FILLED_MODE :
      // disable 'Draw Polygon', 'Filling' menu 
      EnableMenuItem(hsubmenu,ID_DRAW_POLY,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu,ID_FILL,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAWN_MODE :
      // enable 'Filling' menus, disable 'Draw Polygon' menu
      EnableMenuItem(hsubmenu,ID_DRAW_POLY,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu,ID_FILL,
                     MF_BYCOMMAND|MF_ENABLED);
      break;
  }
}
/*This function calls fil function to colour the cirlces */
void performFilling(HWND hwnd)
{
  setDrawMode(FILL_MODE, hwnd);
  SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
  fill();
  reDraw(hwnd);
  setDrawMode(FILLED_MODE, hwnd);
}

/* If filling is clicked then it calls performFilling function*/
void processCommand(int cmd, HWND hwnd)
{
  switch(cmd)
  {
    case ID_FILL:
      performFilling(hwnd);
      break;
    default:
      processCommonCommand(cmd, hwnd);
      break;
  }
}

/* function which checks if a point lies inside,outside or on the circle1 */
bool circle1(int x,int y ){
     int cx = gDrawData.cornerPts[0].x;
     int cy = gDrawData.cornerPts[0].y;
     int r = gDrawData.r1;
     if( (x-cx)*(x-cx) + (y-cy)*(y-cy) - (r)*(r) <=0 )
      return true;
     else
      return false; 
     }
/* function which checks if a point lies inside,outside or on the circle2 */
bool circle2(int x,int y ){
     int cx = gDrawData.cornerPts[1].x;
     int cy = gDrawData.cornerPts[1].y;
     int r = gDrawData.r2;
     if( (x-cx)*(x-cx) + (y-cy)*(y-cy) - (r)*(r) <=0 )
      return true;
     else
      return false; 
     }
/* function which checks if a point lies inside,outside or on the circle3 */     
bool circle3(int x,int y ){
     int cx = gDrawData.cornerPts[2].x;
     int cy = gDrawData.cornerPts[2].y;
     int r = gDrawData.r3;
     if( (x-cx)*(x-cx) + (y-cy)*(y-cy) - (r)*(r) <=0 )
      return true;
     else
      return false; 
     }          

/*main function which fills the circles*/
void fill()
{
	int x=0;
	int y,radius;

	/* cyi are the y coordinates of the center of the circles and cxi are the x coordinates.*/
	int cy1 = gDrawData.cornerPts[0].y;
	int cy2 = gDrawData.cornerPts[1].y;
	int cy3 = gDrawData.cornerPts[2].y;
	int cx1 = gDrawData.cornerPts[0].x;
	int cx2 = gDrawData.cornerPts[1].x;
	int cx3 = gDrawData.cornerPts[2].x;

	/* ymin and ymax are the y values for which scan lines need to be processed*/
	int ymin = min( cy1 - gDrawData.r1 , min(cy2 - gDrawData.r2,cy3 - gDrawData.r3)); 
	int ymax = max( cy1 + gDrawData.r1 , max(cy2 +gDrawData.r2,cy3 +gDrawData.r3));

	/* MoveToEx (gDrawData.hdcMem, 10, ymin, NULL);
	LineTo (gDrawData.hdcMem, 1000+1, ymin);
	gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_XYZ);
	SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
	MoveToEx (gDrawData.hdcMem, 10, ymax, NULL);
	LineTo (gDrawData.hdcMem, 1000+1, ymax);
	*/

    int scan = ymin;
    int r11 = gDrawData.r1 * gDrawData.r1;
    int r22 = gDrawData.r2 * gDrawData.r2;
    int r33 = gDrawData.r3 * gDrawData.r3;
    int x1=-100000,x2=-100000,x3=-100000; 
    int f1 =0,f2=0,f3=0;

    /* scanning the lines here and checking the intersections*/
	while(scan <= ymax)
	{
		if ((r11 - (scan - cy1)*(scan -cy1))>=0)
		{
			x1 = sqrt(r11 - (scan - cy1)*(scan -cy1));
			f1=1;
		}
		if ((r22 - (scan - cy2)*(scan -cy2))>=0)
		{
			x2 = sqrt(r22 - (scan - cy2)*(scan -cy2));
			f2=1;
		}
		if ((r33 - (scan - cy3)*(scan -cy3))>=0)
		{
			x3 = sqrt(r33 - (scan - cy3)*(scan -cy3));
			f3=1;
		}
		
		/*case1: only circle1 has intersections with the scan line*/     
		if ( f1 && !f2 && !f3)
		{
			int x11 =  min( x1 + cx1 , -x1 + cx1);
			int x12 =  max( x1 + cx1 , -x1 + cx1);
			gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_REST);
			SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
			MoveToEx (gDrawData.hdcMem, x11, scan, NULL);
			LineTo (gDrawData.hdcMem, x12+1, scan);
		}

		/*case2: only circle2 has intersections with the scan line*/ 
		else if (f2 && !f1 && !f3)
		{
			int x21 =  min( x2 + cx2 , -x2 + cx2);
			int x22 =  max( x2 + cx2 , -x2 + cx2);
			gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_REST);
			SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
			MoveToEx (gDrawData.hdcMem, x21, scan, NULL);
			LineTo (gDrawData.hdcMem, x22+1, scan);
		}

		/*case3: only circle3 has intersections with the scan line*/ 
		else if ( f3 && !f2 && !f1)
		{
			int x31 =  min( x3 + cx3 , -x3 + cx3);
			int x32 =  max( x3 + cx3 , -x3 + cx3);
			gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_REST);
			SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
			MoveToEx (gDrawData.hdcMem, x31, scan, NULL);
			LineTo (gDrawData.hdcMem, x32+1, scan);
		} 
       
		/*case4:  circle1  and circle2 have intersections with the scan line*/ 
		else if(f1&&f2&&!f3)
		{
			int x11 =  min( x1 + cx1 , -x1 + cx1);
			int x12 =  max( x1 + cx1 , -x1 + cx1);
			int x21 =  min( x2 + cx2 , -x2 + cx2);
			int x22 =  max( x2 + cx2 , -x2 + cx2);
			vector<int > p;
			p.push_back(x11);
			p.push_back(x12);
			p.push_back(x21);
			p.push_back(x22);
			sort(p.begin(),p.end());        //sort the poins in increasing order
			for( int i=0;i< p.size()-1;i++)
			{
				vector<int> n1,n2;
				int mid = (p[i] + p[i+1])/2;      //mid point 
				if(circle1(mid,scan))            //checking where the mid-point lies
					n1.push_back(1);
				if(circle2(mid,scan))
					n1.push_back(2);
				if(n1.size()==2)
				{
					gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_XY);     //sets the colour of the pen
					SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);    //selects the pen
					MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);         //move to the left point
					LineTo (gDrawData.hdcMem, p[i+1]+1, scan);             //draw the line from p[i] to p[i+1]             
				}
				else if(n1.size()==1)
				{
					gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_REST);
					SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
					MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);
					LineTo (gDrawData.hdcMem, p[i+1]+1, scan);  
				}       
            }
        }
        
    	/*case5:  circle3  and circle2 have intersections with the scan line*/ 
		else if(f3&&f2&&!f1)
		{
            int x31 =  min( x3+ cx3 , -x3 + cx3);
			int x32 =  max( x3 + cx3 , -x3 + cx3);
			int x21 =  min( x2 + cx2 , -x2 + cx2);
			int x22 =  max( x2 + cx2 , -x2 + cx2);
			vector<int> p;
			p.push_back(x31);
			p.push_back(x32);
			p.push_back(x21);
			p.push_back(x22);
			sort(p.begin(),p.end());
			for( int i=0;i< p.size()-1;i++)
			{
				vector<int> n1,n2;
				int mid = (p[i]+p[i+1])/2;
				if(circle2(mid,scan))
					n1.push_back(2);
				if(circle3(mid,scan))
					n1.push_back(3);
                  
				if(n1.size()==2)
				{
					gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_YZ);
					SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
					MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);
					LineTo (gDrawData.hdcMem, p[i+1]+1, scan);                        
				}
				else if(n1.size()==1)
				{
					gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_REST);
					SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
					MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);
					LineTo (gDrawData.hdcMem, p[i+1]+1, scan);  
				}     
			}
     
       }  
        /*case6:  circle1  and circle3 have intersections with the scan line*/ 
        else if(f1&&f3&&!f2)
		{
			int x31 =  min( x3+ cx3 , -x3 + cx3);
			int x32 =  max( x3 + cx3 , -x3 + cx3);
			int x11 =  min( x1 + cx1 , -x1 + cx1);
			int x12 =  max( x1 + cx1 , -x1 + cx1);
			vector<int > p;
     		p.push_back(x31);
			p.push_back(x32);
			p.push_back(x11);
			p.push_back(x12);
			sort(p.begin(),p.end());
			for( int i=0;i< p.size()-1;i++)
			{
				vector<int> n1,n2;
				int mid = (p[i] + p[i+1])/2;
				if(circle1(mid,scan))
					n1.push_back(1);
				if(circle3(mid,scan))
					n1.push_back(3);
			
				if(n1.size()==2)
				{
					gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_ZX);
					SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
					MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);
					LineTo (gDrawData.hdcMem, p[i+1]+1, scan);                        
				}
				else if(n1.size()==1)
				{
					gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_REST);
					SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
					MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);
					LineTo (gDrawData.hdcMem, p[i+1]+1, scan);  
				}   
			}
			
		}
		  
		/*case7:  All circles have intersections with the scan line*/ 
        else if( f1 && f2 && f3)
		{
			int x31 =  min( x3+ cx3 , -x3 + cx3);
			int x32 =  max( x3 + cx3 , -x3 + cx3);
			int x11 =  min( x1 + cx1 , -x1 + cx1);
			int x12 =  max( x1 + cx1 , -x1 + cx1);
			int x21 =  min( x2 + cx2 , -x2 + cx2);
			int x22 =  max( x2 + cx2 , -x2 + cx2);
			vector<int > p;
			p.push_back(x31);
			p.push_back(x32);
			p.push_back(x11);
			p.push_back(x12);
			p.push_back(x21);
			p.push_back(x22);
			sort(p.begin(),p.end());
			for( int i=0;i< p.size()-1;i++)
			{
                              
				vector<int> n1,n2;
				int mid = (p[i]+p[i+1])/2;
				if(circle1(mid,scan))
					n1.push_back(1);
				if(circle2(mid,scan))
					n1.push_back(2);   
				if(circle3(mid,scan))
					n1.push_back(3);
                                      
				if(n1.size()==3)
				{
					gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_XYZ);
					SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
					MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);
					LineTo (gDrawData.hdcMem, p[i+1]+1, scan);          
				
				}
				else if(n1.size()==2)
				{
					if(n1[0]==1 && n1[1]==2)
					{
						gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_XY);          
					}
					else if(n1[0]==1 && n1[1]==3) 
					{
						gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_ZX); 
					}
					else if(n1[0]==2 &&n1[1]==3)
					{
						gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_YZ); 
					} 
					SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
					MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);
					LineTo (gDrawData.hdcMem, p[i+1]+1, scan);        
					
					}
					else if(n1.size()==1)
					{
						gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_REST);
						SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
						MoveToEx (gDrawData.hdcMem, p[i], scan, NULL);
						LineTo (gDrawData.hdcMem, p[i+1]+1, scan);	
					}
				}  
            }                                  
		
		scan++;
		f1=0;
        f2=0;
        f3=0;       
    }
}

