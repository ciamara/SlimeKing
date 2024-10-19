#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640 //pixele
#define SCREEN_HEIGHT	480 //pixele
#define GROUND_LEVEL    450 //pixele
#define STARTING_POSITION_X  50 //pixele
#define SPIKE_DROP_HEIGHT 100 //pixele
#define LEFT_BORDER 7 //pixele
#define RIGHT_BORDER 633 //pixele
#define SLIME_HEIGHT 50 //pixele
#define PLATFORM_HEIGHT 13 //pixele
#define FLOOR0 432 //pixele
#define FLOOR1 370 //pixele
#define FLOOR2 310 //pixele
#define FLOOR3 250 //pixele
#define FLOOR4 190 //pixele
#define FLOOR5 130 //pixele
#define FLOOR6 70 //pixele
#define FLOOR7 55 //pixele

struct game {
	bool running;
	bool menu;
	bool notice;
	int round;
	int lives;
	bool canLeft;
	bool canRight;
	bool startingPos;
	bool onWaterfall;
	bool onCrown;
	bool onPlatform;
	bool won;
	bool roundStart;
	int points;
	bool coin1;
	bool coin2;
	bool coin3;
	int coinPoints;
};

struct visuals
{
	char text[128];
	int czarny;
	int darkBlue;
	int blue;
	int lightBlue;
	int white;
	int darkGreen;
	int green;
	int lightGreen;
};

void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset) 
{
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) 
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};

void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
};

void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void DrawFrame(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor)
{
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
}


int slimeOnPlatformRound3(int pos, int* height, double* slimeSpeedVertical)
{
	if (*height >= GROUND_LEVEL) { *height = GROUND_LEVEL; *slimeSpeedVertical = 0; return 1; }
	else if (((*height >= FLOOR1) && (*height <= FLOOR1 + 1)) && (((pos>=LEFT_BORDER)&&(pos<=222))||((pos >= 367) && (pos <= 600))) ) { *height = FLOOR1; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR2) && (*height <= FLOOR2 + 1)) && ( ((pos>=LEFT_BORDER)&&(pos<=102))||((pos >= 132) && (pos <= 475))||((pos>=540)&&(pos<=RIGHT_BORDER)) ) ) { *height = FLOOR2; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR3) && (*height <= FLOOR3 + 1)) && ((pos >= LEFT_BORDER) && (pos <= 430))) { *height = FLOOR3; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR4) && (*height <= FLOOR4 + 1)) && ((pos >= LEFT_BORDER) && (pos <= 210))) { *height = FLOOR4; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR5) && (*height <= FLOOR5 + 1)) && ((pos >= 115) && (pos <= RIGHT_BORDER))) { *height = FLOOR5; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR6) && (*height <= FLOOR6 + 1)) && ((pos >= 135) && (pos <= 556))) { *height = FLOOR6; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR7 - 5) && (*height <= FLOOR7 - 4)) && ((pos >= 60) && (pos <= 148))) { *height = FLOOR7; *slimeSpeedVertical = 0; return true; }
	else return false;
}

int slimeOnPlatformRound2(int pos, int* height, double* slimeSpeedVertical)
{
	if (*height >= GROUND_LEVEL) { *height = GROUND_LEVEL; *slimeSpeedVertical = 0; return 1; }
	else if (((*height >= FLOOR1) && (*height <= FLOOR1 + 1)) && ((pos >= LEFT_BORDER) && (pos <= 448))) { *height = FLOOR1; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR2) && (*height <= FLOOR2 + 1)) && ((pos >= 59) && (pos <= 581))) { *height = FLOOR2; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR3) && (*height <= FLOOR3 + 1)) && ((pos >= LEFT_BORDER) && (pos <= 164)))  { *height = FLOOR3; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR3) && (*height <= FLOOR3 + 1)) && ((pos >= 309) && (pos <= RIGHT_BORDER))) { *height = FLOOR3; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR4) && (*height <= FLOOR4 + 1)) && ((pos >= 59) && (pos <= RIGHT_BORDER))) { *height = FLOOR4; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR5) && (*height <= FLOOR5 + 1)) && ((pos >= LEFT_BORDER) && (pos <= 551))) { *height = FLOOR5; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR6) && (*height <= FLOOR6 + 1)) && ((pos >= 96) && (pos <= 444))) { *height = FLOOR6; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR7 - 5) && (*height <= FLOOR7 - 4)) && ((pos >= 455) && (pos <= 504))) { *height = FLOOR7; *slimeSpeedVertical = 0; return true; }
	else return false;
}

int slimeOnPlatformRound1(int pos, int *height, double *slimeSpeedVertical)
{
	if (*height >= GROUND_LEVEL) { *height = GROUND_LEVEL; *slimeSpeedVertical = 0; return 1; }
	else if (((*height >= FLOOR1) && (*height <= FLOOR1 + 1)) && ((pos >= LEFT_BORDER) && (pos <= 581))) { *height = FLOOR1; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR2) && (*height <= FLOOR2 + 1)) && ((pos >= 55) && (pos <= RIGHT_BORDER))) { *height = FLOOR2; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR3) && (*height <= FLOOR3 + 1)) && ((pos >= LEFT_BORDER) && (pos <= 581))) { *height = FLOOR3; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR4) && (*height <= FLOOR4 + 1)) && ((pos >= 55) && (pos <= RIGHT_BORDER))) { *height = FLOOR4; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR5) && (*height <= FLOOR5 + 1)) && ((pos >= LEFT_BORDER) && (pos <= 581))) { *height = FLOOR5; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR6) && (*height <= FLOOR6 + 1)) && ((pos >= 185) && (pos <= 428))) { *height = FLOOR6; *slimeSpeedVertical = 0; return true; }
	else if (((*height >= FLOOR7 - 5) && (*height <= FLOOR7 - 4)) && ((pos >= 105) && (pos <= 193))) { *height = FLOOR7; *slimeSpeedVertical = 0; return true; }
	else return false;
}



int slimeOnCoinRound1(int pos, int height, struct game game)
{
	if (((pos <= 65) && (pos >= 27) && (height <= FLOOR5) && (height >= FLOOR5-50)) && (game.coin1 == true)) return 1; 
	else return 0;
}

int slimeOnCoinRound2(int pos, int height, struct game game)
{
	if (((pos <= 398) && (pos >= 355) && (height <= FLOOR3) && (height >= FLOOR3-50)) && (game.coin2 == true)) return 1; 
	else return 0;
}

int slimeOnCoinRound3(int pos, int height, struct game game)
{
	if (((pos <= 613) && (pos >= 571) && (height <= FLOOR2) && (height >= FLOOR2 - 50)) && (game.coin3 == true)) return 1;
	else return 0;
}



int slimeOnCrownRound3(int pos, int height)
{
	if ((pos >= 75) && (pos <= 133) && (height <= 55)) return true;
	return false;
}

int slimeOnCrownRound2(int pos, int height)
{
	if ((pos >= 454) && (pos <= 508) && (height <= 55)) return true;
	else return false;
}

int slimeOnCrownRound1(int pos, int height)
{
	if ((pos >= 121) && (pos <= 175) && (height <= 55)) return true;
	else return false;
}



int slimeOnWaterfallRound3(int pos, int height)
{
	if ((pos >= 414) && (pos <= 433) && (height <= GROUND_LEVEL) && (height >= FLOOR1)) {
		return true;
	}
	if ((pos >= 418) && (pos <= 427) && (height <= FLOOR1) && (height >= FLOOR2)) {
		return true;
	}
	if ((pos >= 168) && (pos <= 178) && (height <= FLOOR1) && (height >= FLOOR2)) {
		return true;
	}
	if ((pos >= 38) && (pos <= 47) && (height <= FLOOR1) && (height >= FLOOR2)) {
		return true;
	}
	if ((pos >= 38) && (pos <= 47) && (height <= FLOOR2) && (height >= FLOOR3)) {
		return true;
	}
	if ((pos >= 89) && (pos <= 96) && (height <= FLOOR3) && (height >= FLOOR4)) {
		return true;
	}
	if ((pos >= 159) && (pos <= 165) && (height <= FLOOR4) && (height >= FLOOR5)) {
		return true;
	}
	if ((pos >= 299) && (pos <= 306) && (height <= FLOOR5) && (height >= FLOOR6)) {
		return true;
	}
	if ((pos >= 501) && (pos <= 508) && (height <= FLOOR5) && (height >= FLOOR6)) {
		return true;
	}
	else return false;
}

int slimeOnWaterfallRound2(int pos, int height)
{

	if ((pos >= 237) && (pos <= 254) && (height <= GROUND_LEVEL) && (height >= FLOOR1)) {
		return true;
	}
	if ((pos >= 394) && (pos <= 401) && (height <= FLOOR1) && (height >= FLOOR2)) {
		return true;
	}
	if ((pos >= 111) && (pos <= 119) && (height <= FLOOR2) && (height >= FLOOR3)) {
		return true;
	}
	if ((pos >= 110) && (pos <= 119) && (height <= FLOOR3) && (height >= FLOOR4)) {
		return true;
	}
	if ((pos >= 584) && (pos <= 593) && (height <= FLOOR3) && (height >= FLOOR4)) {
		return true;
	}
	if ((pos >= 310) && (pos <= 318) && (height <= FLOOR4) && (height >= FLOOR5)) {
		return true;
	}
	if ((pos >= 496) && (pos <= 506) && (height <= FLOOR4) && (height >= FLOOR5)) {
		return true;
	}
	if ((pos >= 135) && (pos <= 143) && (height <= FLOOR5) && (height >= FLOOR6)) {
		return true;
	}
	else return false;
}

int slimeOnWaterfallRound1(int pos, int height)
{	
	if ((pos >= 512) && (pos <= 529) && (height <= GROUND_LEVEL) && (height >= FLOOR1)) {
		return true;
	}
	if ((pos >= 118) && (pos <= 129) && (height <= FLOOR1) && (height >= FLOOR2)) {
		return true;
	}
	if ((pos >= 293) && (pos <= 302) && (height <= FLOOR1) && (height >= FLOOR2)) {
		return true;
	}
	if ((pos >= 347) && (pos <= 356) && (height <= FLOOR2) && (height >= FLOOR3)) {
		return true;
	}
	if ((pos >= 516) && (pos <= 527) && (height <= FLOOR2) && (height >= FLOOR3)) {
		return true;
	}
	if ((pos >= 118) && (pos <= 129) && (height <= FLOOR3) && (height >= FLOOR4)) {
		return true;
	}
	if ((pos >= 217) && (pos <= 226) && (height <= FLOOR3) && (height >= FLOOR4)) {
		return true;
	}
	if ((pos >= 517) && (pos <= 525) && (height <= FLOOR4) && (height >= FLOOR5)) {
		return true;
	}
	if ((pos >= 360) && (pos <= 369) && (height <= FLOOR5) && (height >= FLOOR6)) {
		return true;
	}
	else return false;
}



int waterfallTopRound1(int pos, int height)
{
	if ((slimeOnWaterfallRound1(pos, height)) && (slimeOnWaterfallRound1(pos, height - 1)==true)) return 0;
	if ((slimeOnWaterfallRound1(pos, height)) && (slimeOnWaterfallRound1(pos, height - 1)==false)) return 1;
}

int waterfallTopRound2(int pos, int height)
{
	if ((slimeOnWaterfallRound2(pos, height)) && (slimeOnWaterfallRound2(pos, height - 1) == true)) return 0;
	if ((slimeOnWaterfallRound2(pos, height)) && (slimeOnWaterfallRound2(pos, height - 1) == false)) return 1;
}

int waterfallTopRound3(int pos, int height)
{
	if ((slimeOnWaterfallRound3(pos, height)) && (slimeOnWaterfallRound3(pos, height - 1) == true)) return 0;
	if ((slimeOnWaterfallRound3(pos, height)) && (slimeOnWaterfallRound3(pos, height - 1) == false)) return 1;
}

int waterfallTop(struct game game, int pos, int height)
{
	if ((game.round == 1) && (waterfallTopRound1(pos, height) == 1)) return true;
	if ((game.round == 2) && (waterfallTopRound2(pos, height) == 1)) return true;
	if ((game.round == 3) && (waterfallTopRound3(pos, height) == 1)) return true;
	else return false;
}



int waterfallBotRound1(int pos, int height)
{
	if ((slimeOnWaterfallRound1(pos, height)) && (slimeOnWaterfallRound1(pos, height + 1) == true)) return 0;
	if ((slimeOnWaterfallRound1(pos, height)) && (slimeOnWaterfallRound1(pos, height + 1) == false)) return 1;
}

int waterfallBotRound2(int pos, int height)
{
	if ((slimeOnWaterfallRound2(pos, height)) && (slimeOnWaterfallRound2(pos, height + 1) == true)) return 0;
	if ((slimeOnWaterfallRound2(pos, height)) && (slimeOnWaterfallRound2(pos, height + 1) == false)) return 1;
}

int waterfallBotRound3(int pos, int height)
{
	if ((slimeOnWaterfallRound3(pos, height)) && (slimeOnWaterfallRound3(pos, height + 1) == true)) return 0;
	if ((slimeOnWaterfallRound3(pos, height)) && (slimeOnWaterfallRound3(pos, height + 1) == false)) return 1;
}

int waterfallBot(struct game game, int pos, int height)
{
	if ((game.round == 1) && (waterfallBotRound1(pos, height) == 1)) return true;
	if ((game.round == 2) && (waterfallBotRound2(pos, height) == 1)) return true;
	if ((game.round == 3) && (waterfallBotRound3(pos, height) == 1)) return true;
	else return false;
}



void DrawPlatformMiddle(SDL_Surface* screen, struct visuals vis, int x, int floor, int length)
{
	DrawRectangle(screen, x, SCREEN_HEIGHT - SLIME_HEIGHT * floor - 30 - 10 * floor, length, PLATFORM_HEIGHT, vis.darkBlue, vis.blue);
	DrawRectangle(screen, x + 5, SCREEN_HEIGHT - SLIME_HEIGHT * floor - 30 - 10 * floor + 4, length-10, 5, vis.lightBlue, vis.white);
}

void DrawFinalPlatform(SDL_Surface* screen, struct visuals vis, int x, int length)
{
	DrawRectangle(screen, x, FLOOR7+15, length, PLATFORM_HEIGHT, vis.darkBlue, vis.blue);
	DrawRectangle(screen, x + 5, FLOOR7+19, length - 10, 5, vis.lightBlue, vis.white);
}

void DrawPlatformRight(SDL_Surface* screen, struct visuals vis, int x, int floor)
{
	DrawRectangle(screen, x, SCREEN_HEIGHT - SLIME_HEIGHT * floor - 30 - 10 * floor, RIGHT_BORDER-x, PLATFORM_HEIGHT, vis.darkBlue, vis.blue);
	DrawRectangle(screen, x+5, SCREEN_HEIGHT - SLIME_HEIGHT * floor - 30 - 10 * floor + 4, RIGHT_BORDER-x-5, 5, vis.lightBlue, vis.white);
}

void DrawPlatformLeft(SDL_Surface* screen, struct visuals vis, int floor, int length)
{
	DrawRectangle(screen, LEFT_BORDER, SCREEN_HEIGHT - SLIME_HEIGHT * floor - 30 -10*floor, length, PLATFORM_HEIGHT, vis.darkBlue, vis.blue);
	DrawRectangle(screen, LEFT_BORDER, SCREEN_HEIGHT - SLIME_HEIGHT * floor - 30 - 10 * floor + 4, length - 5, 5, vis.lightBlue, vis.white);
}



void displayMapRound2(SDL_Surface* screen, struct visuals vis, struct game game, SDL_Surface *scepter, SDL_Surface *waterfallLong, SDL_Surface *waterfall, SDL_Surface* coin, SDL_Surface* coinPoints)
{
	DrawPlatformLeft(screen, vis, 1, (SCREEN_WIDTH / 3) * 2);
	DrawPlatformRight(screen, vis, SCREEN_WIDTH/2, 3);
	DrawPlatformMiddle(screen, vis, 74, 2, SCREEN_WIDTH-148);
	DrawPlatformLeft(screen, vis, 3, 142);
	DrawPlatformRight(screen, vis, 74, 4);
	DrawPlatformLeft(screen, vis, 5, (SCREEN_WIDTH / 6) * 5);
	DrawPlatformMiddle(screen, vis, 108, 6, SCREEN_WIDTH / 2);
	DrawFinalPlatform(screen, vis, 454, 54);
	DrawSurface(screen, scepter, 480, FLOOR7);
	if (game.coin2 == true) DrawSurface(screen, coin, 377, FLOOR3);
	if (game.coinPoints > 0) DrawSurface(screen, coinPoints, 377, FLOOR3);
	DrawSurface(screen, waterfallLong, 246, FLOOR0);
	DrawSurface(screen, waterfall, 397, FLOOR1);
	DrawSurface(screen, waterfall, 115, FLOOR2);
	DrawSurface(screen, waterfall, 115, FLOOR3);
	DrawSurface(screen, waterfall, 589, FLOOR3);
	DrawSurface(screen, waterfall, 500, FLOOR4);
	DrawSurface(screen, waterfall, 313, FLOOR4);
	DrawSurface(screen, waterfall, 140, FLOOR5);
}

void displayMapRound3(SDL_Surface* screen, struct visuals vis, struct game game, SDL_Surface *slimeQueen, SDL_Surface* waterfallLong, SDL_Surface* waterfall, SDL_Surface* coin, SDL_Surface* coinPoints)
{
	DrawPlatformMiddle(screen, vis, (SCREEN_WIDTH / 5) * 3, 1, 200);
	DrawPlatformLeft(screen, vis, 1, 200);
	DrawPlatformMiddle(screen, vis, 140, 2, SCREEN_WIDTH / 2);
	DrawPlatformRight(screen, vis, 554, 2);
	DrawPlatformLeft(screen, vis, 2, SCREEN_WIDTH / 8 * 1);
	DrawPlatformLeft(screen, vis, 3, 423);
	DrawPlatformMiddle(screen, vis, 368, 4, 200);
	DrawPlatformRight(screen, vis, 128, 5);
	DrawPlatformMiddle(screen, vis, 150, 6, 393);
	DrawFinalPlatform(screen, vis, 80, 50);
	DrawPlatformLeft(screen, vis, 4, 200);
	DrawSurface(screen, slimeQueen, 105, FLOOR7);
	if (game.coin3 == true) DrawSurface(screen, coin, 592, FLOOR2);
	if (game.coinPoints > 0) DrawSurface(screen, coinPoints, 592, FLOOR2);
	DrawSurface(screen, waterfallLong, 424, FLOOR0);
	DrawSurface(screen, waterfall, 424, FLOOR1);
	DrawSurface(screen, waterfall, 173, FLOOR1);
	DrawSurface(screen, waterfall, 43, FLOOR1);
	DrawSurface(screen, waterfall, 43, FLOOR2);
	DrawSurface(screen, waterfall, 93, FLOOR3);
	DrawSurface(screen, waterfall, 163, FLOOR4);
	DrawSurface(screen, waterfall, 505, FLOOR5);
	DrawSurface(screen, waterfall, 303, FLOOR5);
}

void displayMapRound1(SDL_Surface* screen, struct visuals vis, struct game game, SDL_Surface* crown, SDL_Surface *waterfallLong, SDL_Surface* waterfall, SDL_Surface* coin, SDL_Surface* coinPoints)
{
	DrawPlatformLeft(screen, vis, 1, SCREEN_WIDTH / 8 * 7);
	DrawPlatformRight(screen, vis, 70, 2);
	DrawPlatformLeft(screen, vis, 3, SCREEN_WIDTH / 8 * 7);
	DrawPlatformRight(screen, vis, 70, 4);
	DrawPlatformLeft(screen, vis, 5, SCREEN_WIDTH / 8 * 7);
	DrawPlatformMiddle(screen, vis, 200, 6, SCREEN_WIDTH/3);
	DrawFinalPlatform(screen, vis, 121, 54);
	DrawSurface(screen, crown, 147, FLOOR7);
	if (game.coin1 == true) DrawSurface(screen, coin, 45, FLOOR5);
	if (game.coinPoints > 0) DrawSurface(screen, coinPoints, 45, FLOOR5);
	DrawSurface(screen, waterfallLong, 520, FLOOR0);
	DrawSurface(screen, waterfall, 298, FLOOR1);
	DrawSurface(screen, waterfall, 124, FLOOR1);
	DrawSurface(screen, waterfall, 351, FLOOR2);
	DrawSurface(screen, waterfall, 520, FLOOR2);
	DrawSurface(screen, waterfall, 221, FLOOR3);
	DrawSurface(screen, waterfall, 124, FLOOR3);
	DrawSurface(screen, waterfall, 520, FLOOR4);
	DrawSurface(screen, waterfall, 365, FLOOR5);
}



void displayRound1(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Surface* charset, struct visuals vis, SDL_Texture* scrtex)
{
	SDL_FillRect(screen, NULL, vis.czarny);
	sprintf(vis.text, "ROUND I");
	DrawString(screen, SCREEN_WIDTH / 2 - strlen(vis.text) * 8 / 2, SCREEN_HEIGHT / 2, vis.text, charset);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_Delay(2000);
}

void displayRound2(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Surface* charset, struct visuals vis, SDL_Texture* scrtex)
{
	SDL_FillRect(screen, NULL, vis.czarny);
	sprintf(vis.text, "ROUND II");
	DrawString(screen, SCREEN_WIDTH / 2 - strlen(vis.text) * 8 / 2, SCREEN_HEIGHT / 2, vis.text, charset);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_Delay(2000);
}

void displayRound3(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Surface* charset, struct visuals vis, SDL_Texture* scrtex)
{
	SDL_FillRect(screen, NULL, vis.czarny);
	sprintf(vis.text, "ROUND III");
	DrawString(screen, SCREEN_WIDTH / 2 - strlen(vis.text) * 8 / 2, SCREEN_HEIGHT / 2, vis.text, charset);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_Delay(2000);
}

void displayNotImplemented(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Surface *charset, struct visuals vis, SDL_Texture *scrtex)
{
	SDL_FillRect(screen, NULL, vis.czarny);
	sprintf(vis.text, "Not Implemented");
	DrawString(screen, SCREEN_WIDTH / 2 - strlen(vis.text) * 8 / 2, SCREEN_HEIGHT /2 , vis.text, charset);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_Delay(2000);
}



void chooseLevel(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* charset, struct visuals vis)
{
	SDL_FillRect(screen, NULL, vis.czarny);
	sprintf(vis.text, "To choose a level, press   1 / 2 / 3   on your keyboard");
	DrawString(screen, SCREEN_WIDTH / 2 - strlen(vis.text) * 8 / 2, SCREEN_HEIGHT / 2, vis.text, charset);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_Delay(2000);
}

void displayWin(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Surface* winscreen, SDL_Surface* charset, struct visuals vis, struct game game, SDL_Texture* scrtex)
{
	SDL_FillRect(screen, NULL, vis.czarny);
	DrawSurface(screen, winscreen, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	sprintf(vis.text, "YOU BECAME THE SLIME KING!!!");
	DrawString(screen, SCREEN_WIDTH / 2 - strlen(vis.text) * 8 / 2, SCREEN_HEIGHT / 2 - 200, vis.text, charset);
	sprintf(vis.text, "POINTS : %d", game.points);
	DrawString(screen, SCREEN_WIDTH / 2 - strlen(vis.text) * 8 / 2, SCREEN_HEIGHT / 2 - 170, vis.text, charset);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_Delay(4000);
}

void displayMenu(SDL_Surface *screen, SDL_Surface *charset, struct visuals vis, struct game game)
{
	SDL_FillRect(screen, NULL, vis.czarny);
	DrawRectangle(screen, (screen->w-250) / 2, 130, 250, 250, vis.white, vis.blue);
	sprintf(vis.text, "<-~-~-~MENU~-~-~->");
	DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 160, vis.text, charset);
	sprintf(vis.text, "EXIT --> ESC");
	DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 190, vis.text, charset);
	sprintf(vis.text, "NEW GAME --> N");
	DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 220, vis.text, charset);
	sprintf(vis.text, "CHOOSE LEVEL --> C");
	DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 250, vis.text, charset);
	sprintf(vis.text, "SAVE --> S");
	DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 280, vis.text, charset);
	sprintf(vis.text, "LOAD --> L");
	DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 310, vis.text, charset);
	sprintf(vis.text, "POINTS --> P");
	DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 340, vis.text, charset);
}



#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char **argv) {
	int t1, t2, frames, rc, gravity, height = GROUND_LEVEL, pos = STARTING_POSITION_X, i;
	double delta, worldTime, fpsTimer, fps, distanceHorizontal, distanceVertical, slimeSpeedHorizontal, slimeSpeedVertical;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface* slime, * slimeStill, * life, * crown, * slimeQueen, * scepter, * waterfallLong, * waterfall, * winscreen, * coin, * coinPoints, * coinPoints1, * coinPoints2, * slimeRunLeft, * slimeRunLeft1, * slimeRunLeft2, * slimeRunRight, * slimeRunRight1, * slimeRunRight2, * slimeWaterfall, * slimeWaterfall1, * slimeWaterfall2, * slimeJumpUpRight, * slimeJumpDownRight, * slimeJumpUpLeft, * slimeJumpDownLeft, * slimeJumpUpRight1, * slimeJumpUpRight2, * slimeJumpUpLeft1, * slimeJumpUpLeft2, * slimeJumpDownRight1, * slimeJumpDownRight2, * slimeJumpDownLeft1, * slimeJumpDownLeft2;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	struct game game;
	struct visuals vis;



	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Slime King, Tamara Mruk, 197584");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);


	SDL_ShowCursor(SDL_DISABLE);

	charset = SDL_LoadBMP("./cs8x8.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	slimeStill = SDL_LoadBMP("./slime.bmp");
	if (slimeStill == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	life = SDL_LoadBMP("./life.bmp");
	if (life == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	crown = SDL_LoadBMP("./crown.bmp");
	if (crown == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeQueen = SDL_LoadBMP("./slimeQueen.bmp");
	if (slimeQueen == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	scepter = SDL_LoadBMP("./scepter.bmp");
	if (scepter == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	winscreen = SDL_LoadBMP("./winscreen.bmp");
	if (winscreen == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	waterfallLong = SDL_LoadBMP("./waterfallLong.bmp");
	if (waterfallLong == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	waterfall = SDL_LoadBMP("./waterfall.bmp");
	if (waterfall == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	coin = SDL_LoadBMP("./coin.bmp");
	if (coin == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeRunRight1 = SDL_LoadBMP("./slimerunright1.bmp");
	if (slimeRunRight1 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeRunRight2 = SDL_LoadBMP("./slimerunright2.bmp");
	if (slimeRunRight2 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeRunLeft1 = SDL_LoadBMP("./slimerunleft1.bmp");
	if (slimeRunLeft1 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeRunLeft2 = SDL_LoadBMP("./slimerunleft2.bmp");
	if (slimeRunLeft2 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeWaterfall1 = SDL_LoadBMP("./slimewaterfall1.bmp");
	if (slimeWaterfall1 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeWaterfall2 = SDL_LoadBMP("./slimewaterfall2.bmp");
	if (slimeWaterfall2 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeJumpUpRight1 = SDL_LoadBMP("./slimejumpupright1.bmp");
	if (slimeJumpUpRight1 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeJumpUpRight2 = SDL_LoadBMP("./slimejumpupright2.bmp");
	if (slimeJumpUpRight2 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeJumpDownRight1 = SDL_LoadBMP("./slimejumpdownright1.bmp");
	if (slimeJumpDownRight1 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeJumpDownRight2 = SDL_LoadBMP("./slimejumpdownright2.bmp");
	if (slimeJumpDownRight2 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeJumpUpLeft1 = SDL_LoadBMP("./slimejumpupleft1.bmp");
	if (slimeJumpUpLeft1 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeJumpUpLeft2 = SDL_LoadBMP("./slimejumpupleft2.bmp");
	if (slimeJumpUpLeft2 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeJumpDownLeft1 = SDL_LoadBMP("./slimejumpdownleft1.bmp");
	if (slimeJumpDownLeft1 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	slimeJumpDownLeft2 = SDL_LoadBMP("./slimejumpdownleft2.bmp");
	if (slimeJumpDownLeft2 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	coinPoints1 = SDL_LoadBMP("./coinpoints1.bmp");
	if (coinPoints1 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	coinPoints2 = SDL_LoadBMP("./coinpoints2.bmp");
	if (coinPoints2 == NULL) {
		printf("SDL_LoadBMP(slime.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	

	vis.czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	vis.darkBlue = SDL_MapRGB(screen->format, 41, 46, 120);
	vis.blue = SDL_MapRGB(screen->format, 102, 112, 255);
	vis.lightBlue = SDL_MapRGB(screen->format, 156, 203, 247);
	vis.white = SDL_MapRGB(screen->format, 255, 255, 255);
	vis.darkGreen = SDL_MapRGB(screen->format, 28, 66, 27);
	vis.green = SDL_MapRGB(screen->format, 101, 184, 99);
	vis.lightGreen = SDL_MapRGB(screen->format, 163, 237, 161);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	worldTime = 0;
	distanceHorizontal = 0;
	distanceVertical = 0;
	slimeSpeedHorizontal = 0;
	slimeSpeedVertical = 0;
	gravity = 200;
	game.running = true;
	game.round = 0;
	game.menu = true;
	game.notice = false;
	game.lives = 3;
	game.canLeft = true;
	game.canRight = true;
	game.startingPos = true;
	game.onWaterfall = false;
	game.onCrown = false;
	game.onPlatform = true;
	game.won = false;
	game.roundStart = true;
	game.points = 0;
	game.coin1 = true;
	game.coin2 = true;
	game.coin3 = true;
	game.coinPoints = 0;
	

	while(game.running) {

		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		fpsTimer += delta;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		if ((int)(worldTime * 10) % 2 == 0) {
			slimeRunLeft = slimeRunLeft1;
			slimeRunRight = slimeRunRight1;
			slimeWaterfall = slimeWaterfall1;		
			slimeJumpUpRight = slimeJumpUpRight1;
			slimeJumpUpLeft = slimeJumpUpLeft1;
			slimeJumpDownRight = slimeJumpDownRight1;
			slimeJumpDownLeft = slimeJumpDownLeft1;
			coinPoints = coinPoints1;
		}
		else {
			slimeRunLeft = slimeRunLeft2;
			slimeRunRight = slimeRunRight2;
			slimeWaterfall = slimeWaterfall2;
			coinPoints = coinPoints2;
			slimeJumpUpRight = slimeJumpUpRight2;
			slimeJumpUpLeft = slimeJumpUpLeft2;
			slimeJumpDownRight = slimeJumpDownRight2;
			slimeJumpDownLeft = slimeJumpDownLeft2;
		}
		if ((game.onWaterfall)&&(slimeSpeedVertical!=0)&&(gravity==0)) {
			if ((int)(worldTime * 10) % 2 == 0) {
				slimeWaterfall = slimeWaterfall1;
			}
			else slimeWaterfall = slimeWaterfall2;
		}
		if (slimeSpeedHorizontal > 0) {
			slime = slimeRunRight;
		}
		if (slimeSpeedHorizontal < 0) {
			slime = slimeRunLeft;
		}
		if (slimeSpeedHorizontal == 0) {
			slime = slimeStill;
		}
		if ((slimeSpeedVertical < 0)&&(slimeSpeedHorizontal>=0)) {
			slime = slimeJumpUpRight;
		}
		if ((slimeSpeedVertical > 0)&&(slimeSpeedHorizontal>=0)) {
			slime = slimeJumpDownRight;
		}
		if ((slimeSpeedVertical < 0) && (slimeSpeedHorizontal < 0)) {
			slime = slimeJumpUpLeft;
		}
		if ((slimeSpeedVertical > 0) && (slimeSpeedHorizontal < 0)) {
			slime = slimeJumpDownLeft;
		}
		if (game.onWaterfall) {
			slime = slimeWaterfall;
		}

		distanceHorizontal += slimeSpeedHorizontal * delta;
		distanceVertical += delta * slimeSpeedVertical;

		if (height < GROUND_LEVEL) slimeSpeedVertical += gravity * delta;
		if (game.menu) displayMenu(screen, charset, vis, game);
		if (game.won) { displayWin(renderer, screen, winscreen, charset, vis, game, scrtex); game.menu = true; game.won = false; }

		if(game.round != 0)
		{
			pos = (STARTING_POSITION_X + (20 * distanceHorizontal));


			if (pos <= 27) { game.canLeft = false; pos = 27; }
			else if (pos > 27) game.canLeft = true;
			if (pos >= 613) { game.canRight = false; pos = 613; }
			else if (pos < 613) game.canRight = true;

			height = GROUND_LEVEL + distanceVertical;
			//slimeOnPlatformRound1(pos, height, &slimeSpeedVertical);
			printf("P%d ", pos);
			printf("H%d ", height);

			if (game.coinPoints > 0) {
				game.coinPoints++;
				if (game.coinPoints >= 1000) {
					game.coinPoints = 0;
				}
			}

			if (game.startingPos) { pos = STARTING_POSITION_X; distanceHorizontal = 0; height = GROUND_LEVEL; distanceVertical = 0; game.startingPos = false; worldTime = -2; }

			switch (game.round) {
				case 1:
					if (game.roundStart) { displayRound1(renderer, screen, charset, vis, scrtex); game.roundStart = false; }
					displayMapRound1(screen, vis, game, crown, waterfallLong, waterfall, coin, coinPoints);
					slimeOnPlatformRound1(pos, &height, &slimeSpeedVertical);
					if (slimeOnCoinRound1(pos, height, game)) { game.points += 100; game.coin1 = false; game.coinPoints = 1; }
					if (slimeOnCrownRound1(pos, height)) { game.round++; game.startingPos = true; game.roundStart = true; game.points += 900; }
					DrawSurface(screen, slime, pos, height);
					break;
				case 2:
					if (game.roundStart) { displayRound2(renderer, screen, charset, vis, scrtex); game.roundStart = false; }
					displayMapRound2(screen, vis, game, scepter, waterfallLong, waterfall, coin, coinPoints);
					slimeOnPlatformRound2(pos, &height, &slimeSpeedVertical);
					if (slimeOnCoinRound2(pos, height, game)) { game.points += 100; game.coin2 = false; game.coinPoints = 1; }
					if (slimeOnCrownRound2(pos, height)) { game.round++; game.startingPos = true; game.roundStart = true; game.points += 900;}
					DrawSurface(screen, slime, pos, height);
					break;
				case 3:
					if (game.roundStart) { displayRound3(renderer, screen, charset, vis, scrtex); game.roundStart = false; }
					displayMapRound3(screen, vis, game, slimeQueen, waterfallLong, waterfall, coin, coinPoints);
					slimeOnPlatformRound3(pos, &height, &slimeSpeedVertical);
					if (slimeOnCoinRound3(pos, height, game)) { game.points += 100; game.coin3 = false; game.coinPoints = 1; }
					if (slimeOnCrownRound3(pos, height)) { game.round = false; game.won = true; game.points += 900;}
					DrawSurface(screen, slime, pos, height);
					break;
			}
		}
		

		DrawFrame(screen, 6, 44, SCREEN_WIDTH - 12, SCREEN_HEIGHT - 51, vis.blue);
		DrawFrame(screen, 4, 42, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 46, vis.white);
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, vis.white, vis.blue);
		sprintf(vis.text, "Czas = %.1lf s  %.0lf fps", worldTime, fps);
		DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 10, vis.text, charset);
		sprintf(vis.text, "Wykonane wymagania :");
		DrawString(screen, 50, 10, vis.text, charset);
		sprintf(vis.text, "1-5 + A, B, D, E, F");
		DrawString(screen, 50, 26, vis.text, charset);
		sprintf(vis.text, "Esc - wyjscie");
		DrawString(screen, screen->w / 2 - strlen(vis.text) * 8 / 2, 26, vis.text, charset);

		if (game.round != 0) {
			for (i = 0; i < game.lives; i++){
				DrawSurface(screen, life,(SCREEN_WIDTH-30)-i*50, 22);
			}
			sprintf(vis.text, "points:");
			DrawString(screen, 420, 10, vis.text, charset);
			sprintf(vis.text, "%d", game.points);
			DrawString(screen, 430, 26, vis.text, charset);

		}

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_FillRect(screen, NULL, vis.czarny);

		switch (game.round) {
		case 1:
			game.onWaterfall = slimeOnWaterfallRound1(pos, height);
			game.onPlatform = slimeOnPlatformRound1(pos, &height, &slimeSpeedVertical);
			game.onCrown = slimeOnCrownRound1(pos, height);
			break;
		case 2:
			game.onWaterfall = slimeOnWaterfallRound2(pos, height);
			game.onPlatform = slimeOnPlatformRound2(pos, &height, &slimeSpeedVertical);
			game.onCrown = slimeOnCrownRound2(pos, height);
			break;
		case 3:
			game.onWaterfall = slimeOnWaterfallRound3(pos, height);
			game.onPlatform = slimeOnPlatformRound3(pos, &height, &slimeSpeedVertical);
			game.onCrown = slimeOnCrownRound3(pos, height);
			break;
		}

		while (SDL_PollEvent(&event))
		{
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_n) { game.menu = false; game.round = 1; game.startingPos = true; game.roundStart = true; game.points = 0; }
				if (event.key.keysym.sym == SDLK_ESCAPE) game.running = false;
				if (event.key.keysym.sym == SDLK_1) { game.menu = false; game.round = 1; game.startingPos = true; game.roundStart = true; game.points = 0; }
				if (event.key.keysym.sym == SDLK_2) { game.menu = false; game.round = 2; game.startingPos = true; game.roundStart = true; game.points = 0;}
				if (event.key.keysym.sym == SDLK_3) { game.menu = false; game.round = 3; game.startingPos = true; game.roundStart = true; game.points = 0;}
				if (game.round != 0) {
					if ((event.key.keysym.sym == SDLK_d) && (game.canRight == true)) slimeSpeedHorizontal = 5.0;
					else if ((event.key.keysym.sym == SDLK_a) && (game.canLeft == true)) slimeSpeedHorizontal = -5.0;
					else if ( (event.key.keysym.sym == SDLK_SPACE) &&  ((height==GROUND_LEVEL)||(height==FLOOR1)||(height==FLOOR2)||(height==FLOOR3)||(height==FLOOR4)||(height==FLOOR5)||(height==FLOOR6)) ) slimeSpeedVertical = -130;
						if (game.onWaterfall) {
							if ((slimeOnWaterfallRound1(pos, height)) && (game.round == 1)&&(((event.key.keysym.sym == SDLK_w)&&(waterfallTopRound1(pos, height)!=1))||(event.key.keysym.sym == SDLK_s))) gravity = 0;
							else if ((slimeOnWaterfallRound2(pos, height)) && (game.round == 2) && (((event.key.keysym.sym == SDLK_w)&&(waterfallTopRound2(pos, height) != 1)) || (event.key.keysym.sym == SDLK_s))) gravity = 0;
							else if ((slimeOnWaterfallRound3(pos, height)) && (game.round == 3) && (((event.key.keysym.sym == SDLK_w)&&(waterfallTopRound3(pos, height) != 1)) || (event.key.keysym.sym == SDLK_s))) gravity = 0;

							if ((event.key.keysym.sym == SDLK_w)&&(waterfallTop(game, pos, height)==false)) { slimeSpeedVertical = -40; }
							else if ((event.key.keysym.sym == SDLK_s)&&(waterfallBot(game, pos, height)==false)) { slimeSpeedVertical = 40; }
						}	
				}
				else if (game.menu == true)
					if (event.key.keysym.sym == SDLK_c) chooseLevel(screen, scrtex, renderer, charset, vis);
					else if (event.key.keysym.sym == SDLK_s) displayNotImplemented(renderer, screen, charset, vis, scrtex);
					else if (event.key.keysym.sym == SDLK_l) displayNotImplemented(renderer, screen, charset, vis, scrtex);
					else if (event.key.keysym.sym == SDLK_p) displayNotImplemented(renderer, screen, charset, vis, scrtex);
				break;
			case SDL_KEYUP:
				slimeSpeedHorizontal = 0;
				slimeSpeedVertical = 0;
				if (game.onWaterfall==false) gravity = 200;
				break;
			case SDL_QUIT:
				game.running = false;
				break;
			};
		};
		frames++;
	};
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
};
