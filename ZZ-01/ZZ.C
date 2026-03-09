#define GRAPHICS_IMPLEMENTATION
#include "graphics.h"

#define CANVAS_IMPLEMENTATION
#include "canvas.h"

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <time.h>


dword pal[] = {
	0x1A1C2CL,
	0x5D275DL,
	0xB13E53L,
	0xEF7D57L,
	0xFFCD75L,
	0xA7F070L,
	0x38B764L,
	0x257179L,
	0x29366FL,
	0x3B5DC9L,
	0x41A676L,
	0x73EFF7L,
	0xF4F4F4L,
	0x94B0C2L,
	0x566C86L,
	0x333C57L
};


clock_t lastTime,currentTime;
double deltaTime;



volatile char keys[128];
void interrupt (*old09)(void);
void interrupt new09(void) {
    unsigned char scancode;

    scancode = inportb(0x60);

    if (scancode & 0x80) {
				keys[scancode & 0x7F] = 0;
    } else {
				keys[scancode] = 1;
    }

    outportb(0x20, 0x20);
}



int main() {

	byte *buf=calloc(SCREEN_SIZE,sizeof(byte));
	Canvas *arrows=Canvas_LoadCVS("arrows.cvs");
	int f=0,key;
	bool quit=false;
	int i,j;
	float x=0,y=0;
	float speed=50;
	int arrowsFrame=0;
	int moving=false;


	srand(time(NULL));

	SetMode(0x13);

	for(i=0;i<16;i++) {
		int r=(int)((pal[i] & 0xFF0000L) >> 16);
		int g=(int)((pal[i] & 0x00FF00L) >> 8);
		int b=(int)((pal[i] & 0x0000FFL));
		r=(int)((double)r/255*63);
		g=(int)((double)g/255*63);
		b=(int)((double)b/255*63);

		SetPalette(i,r,g,b);
	}

	for(i=16;i<256;i++) {
		int r=(int)((pal[0] & 0xFF0000L) >> 16);
		int g=(int)((pal[0] & 0x00FF00L) >> 8);
		int b=(int)((pal[0] & 0x0000FFL));
		r=(int)((double)r/255*63);
		g=(int)((double)g/255*63);
		b=(int)((double)b/255*63);

		SetPalette(i,r,g,b);
	}



	old09 = getvect(0x09);
	setvect(0x09, new09);

	for(i=0;i<128;i++) keys[i]=0;



	lastTime=clock();

	while(!quit) {

		currentTime=clock();
		deltaTime=(double)(currentTime-lastTime)/CLK_TCK;
		lastTime=currentTime;

		if(keys[0x01]) quit=true;

		if(keys[0x11]) {y-=speed*deltaTime; arrowsFrame=0;}
		if(keys[0x1F]) {y+=speed*deltaTime; arrowsFrame=2;}
		if(keys[0x1E]) {x-=speed*deltaTime; arrowsFrame=1;}
		if(keys[0x20]) {x+=speed*deltaTime; arrowsFrame=3;}

		if(x<0) x=0;
		if(x>SCREEN_WIDTH-arrows->w) x=SCREEN_WIDTH-arrows->w;
		if(y<0) y=0;
		if(y>SCREEN_HEIGHT-arrows->h) y=SCREEN_HEIGHT-arrows->h;

		memset(buf,0,SCREEN_SIZE);

		Canvas_Draw(buf,arrows,x,y,arrowsFrame,1);

		vsync();
		memcpy(VGA,buf,SCREEN_SIZE);

		f++;

	}

	setvect(0x09, old09);

	SetMode(0x03);

	return 0;
}