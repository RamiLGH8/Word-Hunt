#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT  720
#define HOME 0
#define ONE_PLAYER_MODE 1
#define COMPUTER_MODE 2
#define VS_COMPUTER_MODE 3
#define MENU 4
#define BEST_SCORE 5
#define WIN_MODE 6
#define LOST_MODE 7
#define INPUT_MODE 8
#define INIT_MODE 9
#define FIN_MODE 10
#define FIN_STATE_MODE 11
#define LOST_MODE 12
#define YOU_WON_MODE 13
#define BOT_WON_MODE 14
#define ERROR_MODE 15
#define MOUSE_KEY 16
#define MENUU 17
#define NO_SAVED 18
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "MText.h"
#include "onePlayer.h"
#include "robot.h"
#include "game.h"
#include "MText.h"
#include "playerVsBot.h"

int NLetters;

TTF_Font *FontTimer,*FontRoboto,*FontSofia,*FontSofia2,*FontSofia1, * Font0,*Font1,*Font2,*Font3;
SDL_Color Color1= { 255, 255, 255},Color2={0,0,0},Color3={152, 112, 78};
SDL_Texture * timer,* Lettres,* Best,* Names[5],* Scores[5],* Dates[5],* UserNameTexture,*MouseKey,*Background,*Menu,*Btn,*YouWon,*UserInput,*circleBtn,*InitStringTexture,*FinalStringTexture,*FinalStateTexture,*stringss,*PauseMenu;
// time variables
unsigned int lastTime, currentTime, OutTime, saveTime , pauseTime = 0;
bool savegame = false;

// local date
time_t t;
struct tm tm;
int index;

//sounds
 Mix_Music *backgroundSound,*wonSound,*lostSound,*clickSound,*errorSound,*keySound;

// score
char line[256],Text[1], Time[50], BestScore[50], Score[50] ,lettres[50],Top[50], Date[50], Name[50], UserName[8], date[11],saveInit[9],saveFin[9],InitString[9],strings[20],FinalString[9],FinalState[9],InitString2[9],FinalString2[9],FinalState2[9];

// buttons
struct Button*menuButtons2[2],*pauseBtn,*backBtn,*doneBtn,*done2Btn,*inputBtn,*numberButtons[6],*homeBtn;
int currentGameMode = HOME,selectedGameMode = HOME;
double previousTimeTaken ;
int start;

int N, n,j,taille,i,index,l,car;

struct image* gameImages[8],* botImages[8],* finalImages[8];


SDL_Texture* loadTexture(char* path);
struct image* createImage(const char * path,int x,int y,int w,int h,int id);
void loadMedia(int newTexture, char* path);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* loadTexture(char* path);
SDL_Texture* textures[] = {NULL};

bool gagner,lost,mouse,entered;

typedef struct Player{
	int score;
    char name[9];
    char date[11];
} Player;

// top players
Player TopPlayers[5], User;

struct Point{
	float x;
	float y;
};

struct Point createPoin(float x, float y) {
	struct Point p;
	p.x = x;
	p.y = y;
	return p;
}

int poinIntRect(float px, float py, float rx, float ry, float rw, float rh) {
	if (px >= rx &&
		px <= rx + rw &&
		py >= ry &&
		py <= ry + rh) {
		return 1;
	}
	return 0;
}

struct image {
    SDL_Rect rect;
	char path[50];
	int id;
};

struct Button {
	struct MText* mText;
	struct SDL_Rect rect;
	int id;
};

struct image* createImage(const char * path,int x,int y,int w,int h,int id) {
	struct image* newImage = (struct image*)malloc(sizeof(struct image));
	strcpy_s(newImage->path,100,path);
	newImage->rect.x = x ;
	newImage->rect.y = y;
	newImage->rect.w = w;
	newImage->rect.h = h;
	newImage->id = id;
	return newImage;
}

struct Button* createButton(const char * text,int x,int y,int w,int h,int id) {
	TTF_Font* font = TTF_OpenFont("fonts/sofia.ttf", 32);
	struct Button* button = (struct Button*)malloc(sizeof(struct Button));
	button->mText = createText(font,Color1, renderer, text, x+10, y+10);
	button->rect.x = x - 10;
	button->rect.y = y;
	button->rect.w = w;
	button->rect.h = h;
	button->id = id;
	return button;
}

struct Button* createButton2(const char * text,int x,int y,int w,int h,int id) {
	TTF_Font* font = TTF_OpenFont("fonts/timer.ttf", 42);
	struct Button* button = (struct Button*)malloc(sizeof(struct Button));
	button->mText = createText(font,Color1, renderer, text, x+10, y+10);
	button->rect.x = x-50 ;
	button->rect.y = y-20;
	button->rect.w = w;
	button->rect.h = h;
	button->id = id;
	return button;
}

void drawContent(struct Button * menuButtons2 ,struct Button * pauseBtn , struct Button * backBtn,struct Button * doneBtn,struct Button * done2Btn,struct Button *numberButtons,
                              struct Button *homeBtn){
    switch (currentGameMode) {
            case HOME:
                break;
            case MENU:
                break;
            case MENUU:
                drawMenu2(menuButtons2);
                break;
            case BEST_SCORE:
                drawButton(backBtn);
                break;
            case LOST_MODE:
                drawButton(homeBtn);
                break;
            case WIN_MODE:
                drawButton(backBtn);
                drawButton(doneBtn);
                break;
            case INPUT_MODE:
                drawNumbers(numberButtons);
                drawButton(backBtn);
                break;
            case INIT_MODE:
                drawButton(doneBtn);
                drawButton(backBtn);
                break;
            case FIN_MODE:
                drawButton(done2Btn);
                drawButton(backBtn);
                break;
            case FIN_STATE_MODE:
                drawButton(doneBtn);
                drawButton(backBtn);
                break;
            case YOU_WON_MODE:
                drawButton(homeBtn);
                break;
            case BOT_WON_MODE:
                drawButton(homeBtn);
                break;
            case ERROR_MODE:
                drawButton(homeBtn);
                break;
            case NO_SAVED:
                drawButton(homeBtn);
                break;
            case MOUSE_KEY:
                drawButton(backBtn);
                break;
            default:
                drawButton(pauseBtn);
        }
}

SDL_Texture* loadTexture(char* path){
    SDL_Surface* loadedSurface = NULL;
    SDL_Texture* newTexture = NULL;
    loadedSurface = IMG_Load(path);
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

SDL_Texture *initialize_texture_for_text(TTF_Font *Font, SDL_Color color, const char* Text, SDL_Renderer *renderer) {
	SDL_Surface * TextSurface = TTF_RenderText_Solid(Font, Text, color);
	SDL_Texture * TextTexture = SDL_CreateTextureFromSurface(renderer, TextSurface);
    SDL_FreeSurface(TextSurface);
    return TextTexture;
}

SDL_Texture *initialize_texture_from_file(const char* file_name, SDL_Renderer *renderer) {
    SDL_Surface *image = IMG_Load(file_name);
    SDL_Texture * image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return image_texture;
}

void path(char *a,char *chaine,int i){
    strcpy(a,"img/");
    strncat(a,&chaine[i],1);
    strcat(a,".png");
}

void path2(char *a,char *chaine,int i){
    strcpy(a,"img/");
    strncat(a,&chaine[i],1);
    strncat(a,&chaine[i],1);
    strcat(a,".png");
}

int checkMouseClick(SDL_Event e,struct image** image,int N) {
    for(int i=0;i<N;i++){
        if (poinIntRect(e.button.x,e.button.y, image[i]->rect.x, image[i]->rect.y,image[i]->rect.h,image[i]->rect.w))
             return i;
    }
    return -1;
}

void drawButton(struct Button* button) {
    SDL_RenderCopy(renderer, Btn, NULL, &button->rect);
	drawText(button->mText,renderer);
}

void drawButton2(struct Button* button) {
    SDL_RenderCopy(renderer, circleBtn, NULL, &button->rect);
	drawText(button->mText,renderer);
}

void checkMouseClickHome(SDL_Event e) {
    //selectedGameMode = ONE_PLAYER_MODE;
      if ((e.button.x > 497) && (e.button.x < 736) &&
                  (e.button.y > 186) && (e.button.y < 226)) {
                    currentGameMode = MOUSE_KEY;
            start=1;
            savegame = false;
            lost=false;
            gagner=false;
            lastTime=0;

        }
    //selectedGameMode = COMPUTER_MODE;
        if ((e.button.x > 497) && (e.button.x < 736) &&
                (e.button.y > 263) && (e.button.y < 304)) {
                NLetters=0;
                strcpy(InitString, "");
                strcpy(FinalString, "");
                strcpy(FinalState, "");
                currentGameMode=INIT_MODE;
         }
         //selectedGameMode = VS_COMPUTER_MODE;
          if ((e.button.x > 497) && (e.button.x < 736) &&
                (e.button.y > 343) && (e.button.y < 384)) {
               		currentGameMode =  VS_COMPUTER_MODE;
                    selectedGameMode= VS_COMPUTER_MODE;
                    start=1;
                    pile2=NULL;
                    gagner=false;
                    lost=false;
         }
         //selectedGameMode = BEST SCORE;
          if ((e.button.x > 497) && (e.button.x < 736) &&
                (e.button.y > 603) && (e.button.y < 647)) {
               		currentGameMode= BEST_SCORE;
         }
}

void checkMouseClickMenu(SDL_Event e) {
    //resume btn
    if ((e.button.x > 497) && (e.button.x < 686) &&
        (e.button.y > 254) && (e.button.y < 296)) {
       currentGameMode = selectedGameMode;
    }
    //back home btn
    if ((e.button.x > 497) && (e.button.x < 686) &&
        (e.button.y > 453) && (e.button.y < 495)) {
       currentGameMode = HOME;
       start=0;
    }
}

void checkMouseClickMenu2(SDL_Event e,struct Button** buttons) {
    //resume btn
	if (poinIntRect(e.button.x, e.button.y, buttons[0]->rect.x, buttons[0]->rect.y, buttons[0]->rect.w, buttons[0]->rect.h)) {
		currentGameMode = selectedGameMode;
	}
	//back home btn
	if (poinIntRect(e.button.x, e.button.y, buttons[1]->rect.x, buttons[1]->rect.y, buttons[1]->rect.w, buttons[1]->rect.h)) {
		currentGameMode = HOME;
		start=0;
	}
}

int checkMouseClickNumbers(SDL_Event e,struct Button** buttons) {
	if (poinIntRect(e.button.x, e.button.y, buttons[0]->rect.x, buttons[0]->rect.y, buttons[0]->rect.w, buttons[0]->rect.h)) {
        return 2;
	}else if (poinIntRect(e.button.x, e.button.y, buttons[1]->rect.x, buttons[1]->rect.y, buttons[1]->rect.w, buttons[1]->rect.h)) {
        return 3;
	}else if (poinIntRect(e.button.x, e.button.y, buttons[2]->rect.x, buttons[2]->rect.y, buttons[2]->rect.w, buttons[2]->rect.h)) {
        return 4;
	}else if (poinIntRect(e.button.x, e.button.y, buttons[3]->rect.x, buttons[3]->rect.y, buttons[3]->rect.w, buttons[3]->rect.h)) {
        return 5;
	}else if (poinIntRect(e.button.x, e.button.y, buttons[4]->rect.x, buttons[4]->rect.y, buttons[4]->rect.w, buttons[4]->rect.h)) {
       	return 6;
	}else if (poinIntRect(e.button.x, e.button.y, buttons[5]->rect.x, buttons[5]->rect.y, buttons[5]->rect.w, buttons[5]->rect.h)) {
        return 7;
	}
    return -1;
}

void drawHome( struct Button** buttons ) {
	for (int i = 0; i < 6; i++)
        drawButton(buttons[i]);
}

void drawMenu( struct Button** buttons ) {
	for (int i = 0; i < 3; i++)
        drawButton(buttons[i]);
}

void drawMenu2( struct Button** buttons ) {
	for (int i = 0; i < 2; i++)
        drawButton(buttons[i]);
}

void drawNumbers( struct Button** buttons ) {
	for (int i = 0; i < 6; i++)
        drawButton2(buttons[i]);
}

void current_time(){
    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);
    int day = tm_struct->tm_mday;
    int month = tm_struct->tm_mon;
    int y = tm_struct->tm_year;
    int h = tm_struct->tm_hour;
    int m = tm_struct->tm_min;
   printf("Current local time and date: %d/%d/%d  %d:%d",day,month+1,y+1900, h,m);
}

int random(int lower, int upper){
    return  (rand() % (upper - lower + 1)) + lower;
}

Player* get_scores(Player Tab[]){
    FILE *fptr;
    if ((fptr = fopen("file.bin","rb")) == NULL){
        printf("Error! opening file");
        exit(1);
    }
    for(int n =0; n< 5; n++){
        fread(&Tab[n], sizeof(Player), 1, fptr);
    }
    fclose(fptr);
    return Tab;
}

int leaderboard_check(Player Tab2[], int score){
    for (int i = 0; i < 5; i++){
        if (Tab2[i].score > score||Tab2[i].score==0){
            return i;
        }
    }
    return -1;
}

Player* update_PlayersArray(Player TopPlayers[],Player User){
	int i;
	Player Tab[5];
	if (leaderboard_check(TopPlayers, User.score) != -1){
		for (i = 0 ; i < leaderboard_check(TopPlayers, User.score); i++){
			Tab[i] = TopPlayers[i];
		}
		Tab[i] = User;
		for (i ; i < 4; i++){
			Tab[i+1] = TopPlayers[i];
		}
		for (i = 0; i < 5; i++){
			TopPlayers[i] = Tab[i];
		}
	}
	return TopPlayers;
}

Player* update_scores(Player Tab[]){
    FILE *fptr;
    if ((fptr = fopen("file.bin","wb")) == NULL){
        printf("Error! opening file");
        exit(1);
    }
    for(int n = 0; n < 5; n++)
        fwrite(&Tab[n], sizeof(struct Player), 1, fptr);
    fclose(fptr);
    return Tab;
}

void update_save(char *initChaine,char* initSate, char *finChaine,char *finState, unsigned int Times,int num) {
    FILE *fptr;
    if ((fptr = fopen("save.bin", "wb")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    fwrite(initChaine, 1, 256, fptr);
    fwrite(initSate, 1, 256, fptr);
    fwrite(finChaine, 1, 256, fptr);
    fwrite(finState, 1, 256, fptr);
    fwrite(&Times, sizeof(unsigned int), 1, fptr);
    fwrite(&num, sizeof(int), 1, fptr);
    fclose(fptr);
}

int read_save(char *initChaine,char* initState, char *finChaine,char *finState,int num) {
    FILE *fptr;
    unsigned int Times;
    if ((fptr = fopen("save.bin", "rb")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    fread(initChaine, 1, 256, fptr);
    fread(initState, 1, 256, fptr);
    fread(finChaine, 1, 256, fptr);
    fread(finState, 1, 256, fptr);
    fread(&Times, sizeof(unsigned int), 1, fptr);
    fread(&num, sizeof(int), 1, fptr);
    fclose(fptr);
    return Times;
}

#endif // GAME_H_INCLUDED
