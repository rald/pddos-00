#define GRAPHICS_IMPLEMENTATION
#include "graphics.h"

#define CANVAS_IMPLEMENTATION
#include "canvas.h"

#define MOUSE_IMPLEMENTATION
#include "mouse.h"

#define BOARD_IMPLEMENTATION
#include "board.h"



#include <stdio.h>
#include <stdlib.h>
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

void DrawText(byte* srf,Canvas *font,int x,int y,int z,char *text) {
	int i;
	int xc=x,yc=y;
	for(i=0;i<strlen(text);i++) {
		Canvas_Draw(srf,font,xc,yc,text[i],z);
		xc+=font->w;
		if(xc>=SCREEN_WIDTH) {
			xc=0;
			yc+=font->h;
			if(yc>=SCREEN_HEIGHT) {
				break;
			}
		}
	}
}

void Palette_Init() {
	int i;
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
}

int main(void) {

	byte *buf=calloc(SCREEN_SIZE,sizeof(byte));

	Canvas *sprite=Canvas_LoadCVS("robo.cvs");
	Canvas *mouse=Canvas_LoadCVS("mouse.cvs");


	Board *board=Board_Load("level-00.rob");




	int f=0,key;
	bool quit=false;
	int i,j,k;
	int flr,str;

	word mouse_on=0;
	word num_buttons=0;
	word mouse_x=0,mouse_y=0;
	word mouse_buttons=0;

	srand(time(NULL));

	Mouse_Init(&mouse_on,&num_buttons);

	if(!mouse_on) {
		printf("Error: cannot initialize mouse\n");
		return 1;
	}


	SetMode(0x13);

	Palette_Init();


	old09 = getvect(0x09);
	setvect(0x09, new09);
	for(i=0;i<128;i++) keys[i]=0;




	lastTime=clock();
	while(!quit) {
		currentTime=clock();
		deltaTime=(double)(currentTime-lastTime)/CLK_TCK;
		lastTime=currentTime;

		Mouse_Status(&mouse_x,&mouse_y,&mouse_buttons);

		if(mouse_x<0) mouse_x=0;
		if(mouse_y<0) mouse_y=0;
		if(mouse_x>639) mouse_x=639;
		if(mouse_y>199) mouse_y=199;

		if(keys[0x01]) quit=true;

		memset(buf,0,SCREEN_SIZE);


		for(j=0;j<board->h;j++) {
			for(i=0;i<board->w;i++) {
				k=j*board->w+i;
				flr=board->tiles[k] & 0x03;
				str=board->tiles[k] & 0x04;
				Canvas_Draw(buf,sprite,i*16,j*16,flr+17,1);
				if(str) Canvas_Draw(buf,sprite,i*16,j*16,25,1);
			}
		}

		Canvas_Draw(buf,sprite,board->x*16,board->y*16,board->d+21,1);

		Canvas_Draw(buf,mouse,mouse_x>>1,mouse_y,0,1);

		vsync();
		memcpy(VGA,buf,SCREEN_SIZE);

		f++;

	}


	Canvas_Free(sprite);
	Canvas_Free(mouse);



	setvect(0x09, old09);



	SetMode(0x03);

	return 0;
}


