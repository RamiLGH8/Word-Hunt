#pragma once
#include <stdio.h>
#include <string.h>
struct MText* timeText = NULL;
struct MText* nameText = NULL;
struct MText {
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect desRect;
	char text[100];
};
struct MText* createText(TTF_Font* font, SDL_Color fg,SDL_Renderer* renderer,const char* text,int x,int y) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, fg);
	SDL_Texture* texture = NULL;
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(font, text, &srcRest.w, &srcRest.h);
	srcRest.x = 0;
	srcRest.y = 0;
	desRect.x = x;
	desRect.y = y;
	desRect.w = srcRest.w;
	desRect.h = srcRest.h;
	struct MText* mText = (struct MText*)malloc(sizeof(struct MText));
	mText->srcRect = srcRest;
	mText->desRect = desRect;
	mText->texture = texture;
	strcpy_s(mText->text,100, text);
	return mText;
}
void drawText(struct MText* mText,SDL_Renderer *renderer) {
	SDL_RenderCopy(renderer, mText->texture, &mText->srcRect, &mText->desRect);
}



