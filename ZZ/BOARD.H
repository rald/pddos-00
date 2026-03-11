#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>



typedef struct {
	int w,h;
	int x,y;
	int d;
	byte *tiles;
} Board;

Board *Board_Load(char *filename);
void Board_Free(Board *board);




#ifdef BOARD_IMPLEMENTATION

Board *Board_Load(char *filename) {
	Board *board=malloc(sizeof(*board));
	FILE *fp=fopen(filename,"r");

	int ch;
	int i;

	fscanf(fp,
		"%d,%d,%d,%d,%d",
		&board->w, &board->h,
		&board->x, &board->y, &board->d
	);

	board->tiles=calloc(board->w*board->h,sizeof(*board->tiles));

	i=0;
	while((ch=fgetc(fp))!=EOF) {
		if(ch>='0' && ch<='7') {
			board->tiles[i++]=ch-'0';
		}
	}

	fclose(fp);

	return board;

}

void Board_Free(Board *board) {
	free(board->tiles);
	board->tiles=NULL;
	free(board);
}

#endif /* BOARD_IMPLEMENTATION */

#endif /* BOARD_H */


