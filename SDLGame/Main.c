#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "MText.h"
#include "game.h"
#include "onePlayer.h"
#include "robot.h"
#include "playerVsBot.h"



int main(int argc, char* args[])
{
    // Load the icon from an image file
    SDL_Surface* icon = IMG_Load("Pictures/icon.png");
    // Set the window icon
    SDL_SetWindowIcon(window, icon);
    t = time(NULL);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    window = SDL_CreateWindow("WORDS GAME",
                                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                                 SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    // Main font and color
    Font0 = TTF_OpenFont("fonts/basic.ttf", 20);
    Font1 = TTF_OpenFont("fonts/basic.ttf", 24);
    Font2 = TTF_OpenFont("fonts/basic.ttf", 32);
	Font3 = TTF_OpenFont("fonts/basic.ttf", 44);
	FontTimer = TTF_OpenFont("fonts/timer.ttf", 20);
	FontSofia = TTF_OpenFont("fonts/sofia.ttf", 20);
	FontSofia1 = TTF_OpenFont("fonts/sofia.ttf", 28);
	FontSofia2 = TTF_OpenFont("fonts/sofia.ttf", 44);
	FontRoboto = TTF_OpenFont("fonts/roboto.ttf", 20);
	// some textures
    Background = initialize_texture_from_file("Pictures/background.png", renderer);
    Menu = initialize_texture_from_file("Pictures/MenuBtns.png", renderer);
    MouseKey = initialize_texture_from_file("Pictures/MouseKey.png", renderer);
    PauseMenu = initialize_texture_from_file("Pictures/PauseMenu.png", renderer);
    Btn = initialize_texture_from_file("Pictures/btn.png", renderer);
    circleBtn = initialize_texture_from_file("Pictures/circleBtn.png", renderer);
    Best = NULL;
    get_scores(TopPlayers);
   // Load audio files
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2 , 2048);
    backgroundSound = Mix_LoadMUS("audio/old city theme.ogg");
    wonSound = Mix_LoadWAV("audio/won.wav");
    lostSound = Mix_LoadWAV("audio/lost.wav");
    clickSound = Mix_LoadWAV("audio/click.wav");
    errorSound = Mix_LoadWAV("audio/error.mp3");
    keySound = Mix_LoadWAV("audio/key.wav");
    Mix_PlayMusic(backgroundSound, -1);
    // menu buttons
    menuButtons2[0] = createButton("Resume", 500, 320, 200, 50, 0);
    menuButtons2[1] = createButton("Back home", 500, 420, 200, 50, 0);
    // numbers btn
    numberButtons[0]=createButton2("2",400,250,150,100,0);
    numberButtons[1]=createButton2("3",550,250,150,100,0);
    numberButtons[2]=createButton2("4",700,250,150,100,0);
    numberButtons[3]=createButton2("5",400,450,150,100,0);
    numberButtons[4]=createButton2("6",550,450,150,100,0);
    numberButtons[5]=createButton2("7",700,450,150,100,0);
    // pause button
    pauseBtn = createButton("Pause", 10, 20, 150, 50, 0);
    // back button
    backBtn = createButton("Back", 10, 20, 150, 50, 0);
    // done button
    doneBtn = createButton("Done", 700, 350, 150, 50, 0);
    // done2 button
    done2Btn = createButton("Done", 200, 350, 150, 50, 0);
    // home button
    homeBtn = createButton("Home", 10, 20, 150, 50, 0);
    // game loop
    bool running = true;
    start=0;
    SDL_Event e;
    entered=false;
    int x,y;
    while (running){
        x = e.motion.x;
        y = e.motion.y;
        if (currentGameMode == HOME){
            OutTime = SDL_GetTicks() - pauseTime;
        }
        if(currentGameMode==MENU){
            if (savegame)
                    pauseTime = SDL_GetTicks() - currentTime - OutTime + saveTime;
            else
                    pauseTime = SDL_GetTicks() - currentTime - OutTime;
        }
        if(currentGameMode==MOUSE_KEY){
            OutTime = SDL_GetTicks() - pauseTime;
        }
        if(currentGameMode==WIN_MODE){
            OutTime = SDL_GetTicks() - pauseTime;
            UserNameTexture = createText(Font2,Color2,renderer,UserName,240,360);
        }
        if(currentGameMode==FIN_MODE){
            OutTime = SDL_GetTicks() - pauseTime;
            FinalStringTexture = createText(Font2,Color2,renderer,FinalString,240,360);
        }
        if(currentGameMode==FIN_STATE_MODE){
            OutTime = SDL_GetTicks() - pauseTime;
            FinalStateTexture = createText(Font2,Color2,renderer,FinalState,240,360);
        }
        if(currentGameMode==INIT_MODE){
            OutTime = SDL_GetTicks() - pauseTime;
            InitStringTexture = createText(Font2,Color2,renderer,InitString,240,360);
        }
        if(currentGameMode==INPUT_MODE){
            UserInput = initialize_texture_for_text(Font2,Color1, UserName, renderer);
        }
        // checking events
        if (SDL_PollEvent(&e)){
                switch(e.type){
                    case SDL_WINDOWEVENT_CLOSE:
                        if(window){
                            SDL_DestroyWindow(window);
                            window=NULL;
                            running= false;
                        }
                    break;
                    case SDL_QUIT:
                        running = false;
                    break;
                    // keyboard events
                    case SDL_KEYDOWN:
                        Mix_PlayChannel(-1, keySound, 0);
                        switch (e.key.keysym.sym){
                            case SDLK_ESCAPE:
                                running = false;
                            break;
                            case SDLK_a:
                                if (strlen(UserName) < 8  && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'a';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='A';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='A';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                }
                                break;
                            case SDLK_b:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'b';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='B';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='B';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_c:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'c';
                                        UserName[NLetters+1] = '\0';
                                         NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='C';
                                        InitString[NLetters+1]='\0';
                                         NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='C';
                                        FinalString[NLetters+1]='\0';
                                         NLetters++;
                                    }

                                }
                                break;
                            case SDLK_d:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'd';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='D';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='D';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_e:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'e';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                         InitString[NLetters]='E';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                         FinalString[NLetters]='E';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_f:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'f';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                         InitString[NLetters]='F';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                         FinalString[NLetters]='F';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_g:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'g';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='G';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='G';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_h:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'h';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='H';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='H';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_i:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'i';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='I';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='I';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_j:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'j';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='J';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='J';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_k:
                                if (strlen(UserName) < 8&& strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'k';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                         InitString[NLetters]='K';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='K';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_l:
                                if (strlen(UserName) < 8&& strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'l';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='L';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='L';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_m:
                                if (strlen(UserName) < 8&& strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'm';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='M';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='M';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_n:
                                if (strlen(UserName) < 8&& strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'n';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='N';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='N';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_o:
                                if (strlen(UserName) < 8&& strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'o';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='O';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='O';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_p:
                                if (strlen(UserName) < 8&& strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'p';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='P';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='P';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_q:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'q';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='Q';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='Q';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_r:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'r';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='R';
                                        InitString[NLetters+1]='\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='R';
                                        FinalString[NLetters+1]='\0';
                                        NLetters++;
                                    }

                                }
                                break;
                            case SDLK_s:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 's';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='S';
                                        InitString[NLetters+1]='\0';NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='S';
                                        FinalString[NLetters+1]='\0';NLetters++;
                                    }

                                }
                                break;
                            case SDLK_t:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 't';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='T';
                                        InitString[NLetters+1]='\0';NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='T';
                                        FinalString[NLetters+1]='\0';NLetters++;
                                    }

                                }
                                break;
                            case SDLK_u:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'u';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='U';
                                        InitString[NLetters+1]='\0';NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='U';
                                        FinalString[NLetters+1]='\0';NLetters++;
                                    }

                                }
                                break;
                            case SDLK_v:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'v';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='V';
                                        InitString[NLetters+1]='\0';NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='V';
                                        FinalString[NLetters+1]='\0';NLetters++;
                                    }

                                }
                                break;
                            case SDLK_w:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'w';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='W';
                                        InitString[NLetters+1]='\0';NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='W';
                                        FinalString[NLetters+1]='\0';NLetters++;
                                    }

                                }
                                break;
                            case SDLK_x:
                                if (strlen(UserName) < 8&& strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'x';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='X';
                                        InitString[NLetters+1]='\0';NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='X';
                                        FinalString[NLetters+1]='\0';NLetters++;
                                    }

                                }
                                break;
                            case SDLK_y:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'y';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='Y';
                                        InitString[NLetters+1]='\0';NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='Y';
                                        FinalString[NLetters+1]='\0';NLetters++;
                                    }

                                }
                                break;
                            case SDLK_z:
                                if (strlen(UserName) < 8 && strlen(InitString) < 8 && strlen(FinalString) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = 'z';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                        InitString[NLetters]='Z';
                                        InitString[NLetters+1]='\0';NLetters++;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters]='Z';
                                        FinalString[NLetters+1]='\0';NLetters++;
                                    }

                                }
                                break;
                            case SDLK_KP_1:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '1';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_STATE_MODE){
                                        FinalState[NLetters] = '1';
                                        FinalState[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=0;
                                    }
                                }
                                break;
                            case SDLK_1:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '1';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_STATE_MODE){
                                        FinalState[NLetters] = '1';
                                        FinalState[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=0;
                                    }
                                }
                                break;
                            case SDLK_KP_2:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '2';
                                        UserName[NLetters+1] = '\0';
                                         NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=1;
                                    }
                                }
                                break;
                            case SDLK_2:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '2';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=1;
                                    }

                                }
                                break;
                            case SDLK_KP_3:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '3';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=2;
                                    }
                                }
                                break;
                            case SDLK_3:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '3';
                                        UserName[NLetters+1] = '\0';  NLetters++;

                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=2;
                                    }
                                }
                                break;
                            case SDLK_KP_4:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '4';
                                        UserName[NLetters+1] = '\0';NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=3;
                                    }
                                }
                                break;
                            case SDLK_4:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '4';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=3;
                                    }
                                }
                                break;
                            case SDLK_KP_5:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '5';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=4;
                                    }
                                }
                                break;
                            case SDLK_5:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '5';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=4;
                                    }
                                }
                                break;
                            case SDLK_KP_6:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                     if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '6';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=5;
                                    }
                                }
                                break;
                            case SDLK_6:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                     if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '6';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=5;
                                    }
                                }
                                break;
                            case SDLK_KP_7:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '7';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=6;
                                    }
                                }
                                break;
                            case SDLK_7:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '7';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=6;
                                    }
                                }
                                break;
                            case SDLK_KP_8:
                                if (strlen(UserName) < 8&& strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '8';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=7;
                                    }
                                }
                                break;
                            case SDLK_8:
                                if (strlen(UserName) < 8&& strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '8';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=7;
                                    }
                                }
                                break;
                            case SDLK_KP_9:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '9';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=8;
                                    }
                                }
                                break;
                            case SDLK_9:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '9';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=8;
                                    }
                                }
                                break;
                            case SDLK_KP_0:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '0';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_STATE_MODE){
                                        FinalState[NLetters] = '0';
                                        FinalState[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=555;
                                    }
                                }
                                break;
                            case SDLK_0:
                                if (strlen(UserName) < 8 && strlen(FinalState) < 8){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters] = '0';
                                        UserName[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==FIN_STATE_MODE){
                                        FinalState[NLetters] = '0';
                                        FinalState[NLetters+1] = '\0';
                                        NLetters++;
                                    }
                                    if(currentGameMode==ONE_PLAYER_MODE){
                                        car=555;
                                    }
                                }
                                break;
                            case SDLK_BACKSPACE:
                                if (strlen(UserName) > 0 || strlen(FinalState) > 0  || strlen(InitString) >0 || strlen(FinalString) >0){
                                    if(currentGameMode==WIN_MODE){
                                        UserName[NLetters-1] = '\0';
                                        NLetters--;
                                    }
                                    if(currentGameMode==INIT_MODE){
                                       InitString[NLetters-1]='\0';
                                       NLetters--;
                                    }
                                    if(currentGameMode==FIN_MODE){
                                        FinalString[NLetters-1]='\0';
                                        NLetters--;
                                    }
                                    if(currentGameMode==FIN_STATE_MODE){
                                        FinalState[NLetters -1] = '\0';
                                        NLetters--;
                                    }
                                }
                                break;
                        }
                        if(currentGameMode==ONE_PLAYER_MODE&&mouse==false&&gagner==false&&car<=6){
                                Rotation(ctemp2,car,n,cbn,ett);
                                int y=CheckRotation(ctemp2,car,n,cbn,ett);
                                if(y==-1){
                                    Mix_PlayChannel(-1, errorSound, 0);
                                }
                                strcpy((char *)ctemp2->combinaison,cbn);
                                strcpy((char *)ctemp2->etat,ett);
                                for(int j=0;j<N;j++){
                                    if(ctemp2->etat[j]=='1'){
                                        char a[20];
                                        path(a,ctemp2->combinaison,j);
                                        textures[j]=loadTexture(a);
                                    }
                                    else{
                                        char a[20];
                                        path2(a,ctemp2->combinaison,j);
                                        textures[j]=loadTexture(a);
                                    }
                                }
                                /*la condition d'arret */
                                if(strcmp(ctemp2->combinaison,ctemp->combinaison)==0 && strcmp(ctemp2->etat,ctemp->etat )==0){
                                    if (atoi(Top)<TopPlayers[4].score||TopPlayers[4].score==0 ){
                                        gagner=true;
                                        Mix_PlayChannel(-1, wonSound, 0);
                                        UserName[0] = '\0';
                                        NLetters = 0;
                                    }else{
                                        Mix_PlayChannel(-1, lostSound, 0);
                                        lost=true;
                                    }
                                }
                            }
                    break;
                    // check mouse event
                    case SDL_MOUSEBUTTONDOWN:
                        Mix_PlayChannel(-1, clickSound, 0);
                        switch(currentGameMode){
                            case HOME:
                                checkMouseClickHome(e);
                                    // check load game button
                                 if ((e.button.x > 497) && (e.button.x < 736) &&
                                        (e.button.y > 521) && (e.button.y < 565)) {
                                        start=1;
                                        savegame = true;
                                        currentGameMode = ONE_PLAYER_MODE;
                                 }
                                // check exit button
                                if ((e.button.x > 1093) && (e.button.x < 1191) &&
                                        (e.button.y > 11) && (e.button.y < 56)) {
                                    running=false;
                                 }
                            break;
                            case MENU:
                                checkMouseClickMenu(e);
                                //save btn
                                if(selectedGameMode==ONE_PLAYER_MODE){
                                    if ((e.button.x > 497) && (e.button.x < 686) &&
                                        (e.button.y > 353) && (e.button.y < 395)) {
                                               update_save(temp2->combinaison,temp2->etat ,ctemp->combinaison,ctemp->etat, currentTime,n);
                                      }
                                }
                            break;
                            case MENUU:
                                checkMouseClickMenu2(e,menuButtons2);
                                break;
                            case ONE_PLAYER_MODE:
                                if(mouse==true){
                                    car=checkMouseClick(e,gameImages,N);
                                    if(car!=-1&&gagner==false){
                                        Rotation(ctemp2,car,n,cbn,ett);
                                        int y=CheckRotation(ctemp2,car,n,cbn,ett);
                                        if(y==-1){
                                            Mix_PlayChannel(-1, errorSound, 0);
                                        }
                                        strcpy((char *)ctemp2->combinaison,cbn);
                                        strcpy((char *)ctemp2->etat,ett);
                                        for(int j=0;j<N;j++){
                                            if(ctemp2->etat[j]=='1'){
                                                char a[20];
                                                path(a,ctemp2->combinaison,j);
                                                textures[j]=loadTexture(a);
                                            }
                                            else{
                                                char a[20];
                                                path2(a,ctemp2->combinaison,j);
                                                textures[j]=loadTexture(a);
                                            }
                                        }
                                        /*la condition d'arret */
                                        if(strcmp(ctemp2->combinaison,ctemp->combinaison)==0 && strcmp(ctemp2->etat,ctemp->etat )==0){
                                            if (atoi(Top)<TopPlayers[4].score||TopPlayers[4].score==0 ){
                                                gagner=true;
                                                Mix_PlayChannel(-1, wonSound, 0);
                                                UserName[0] = '\0';
                                                NLetters = 0;
                                            }else{
                                                Mix_PlayChannel(-1, lostSound, 0);
                                                lost=true;
                                            }
                                        }
                                    }
                                }
                            break;
                            case VS_COMPUTER_MODE:
                                car=checkMouseClick(e,gameImages,N);
                                if(car!=-1&&gagner==false ){
                                   Rotation(first_noeud,car,n,new_string,new_state);
                                   int y=CheckRotation(first_noeud,car,n,new_string,new_state);
                                   if(y==-1){
                                        Mix_PlayChannel(-1, errorSound, 0);
                                    }
                                   strcpy((char *)first_noeud->combinaison,new_string);
                                   strcpy((char *)first_noeud->etat,new_state);
                                   for(int j=0;j<N;j++){
                                        if(first_noeud->etat[j]=='1'){
                                            char a[20];
                                            path(a,first_noeud->combinaison,j);
                                            textures[j]=loadTexture(a);
                                        }
                                        else{
                                            char a[20];
                                            path2(a,first_noeud->combinaison,j);
                                            textures[j]=loadTexture(a);
                                        }
                                    }
                                /*la condition d'arret c de trouver que la combinaison li sbtha m3a l5er hya li lzm tl7e9lh*/
                                   if(strcmp(first_noeud->combinaison,final_string)==0 && strcmp(first_noeud->etat,final_state )==0){
                                        if(lost==false){
                                            gagner=true;
                                            Mix_PlayChannel(-1, wonSound, 0);
                                        }else{
                                            gagner=true;
                                        }
                                    }
                               }
                            break;
                        }
                      // check pause btn
                        if (currentGameMode == 1 ){
                             if (poinIntRect(e.button.x, e.button.y, pauseBtn->rect.x, pauseBtn->rect.y, pauseBtn->rect.w, pauseBtn->rect.h)){
                                currentGameMode = MENU;
                            }
                        }
                        if(currentGameMode == 3 ||currentGameMode == 2){
                             if (poinIntRect(e.button.x, e.button.y, pauseBtn->rect.x, pauseBtn->rect.y, pauseBtn->rect.w, pauseBtn->rect.h)){
                                currentGameMode = MENUU;
                            }
                        }
                        // check the back btn
                        if (currentGameMode == BEST_SCORE|| currentGameMode==MOUSE_KEY||currentGameMode==INIT_MODE){
                             if (poinIntRect(e.button.x, e.button.y, backBtn->rect.x, backBtn->rect.y, backBtn->rect.w, backBtn->rect.h))
                                    currentGameMode = HOME;
                        }
                        if (currentGameMode == INPUT_MODE){
                             if (poinIntRect(e.button.x, e.button.y, backBtn->rect.x, backBtn->rect.y, backBtn->rect.w, backBtn->rect.h))
                                    currentGameMode = INIT_MODE;
                        }
                        if (currentGameMode == FIN_MODE){
                             if (poinIntRect(e.button.x, e.button.y, backBtn->rect.x, backBtn->rect.y, backBtn->rect.w, backBtn->rect.h))
                                    currentGameMode = INPUT_MODE;
                        }
                        if (currentGameMode == FIN_STATE_MODE){
                             if (poinIntRect(e.button.x, e.button.y, backBtn->rect.x, backBtn->rect.y, backBtn->rect.w, backBtn->rect.h))
                                    currentGameMode = FIN_MODE;
                        }
                        // done btn
                        if (currentGameMode == WIN_MODE){
                            if (poinIntRect(e.button.x, e.button.y, doneBtn->rect.x, doneBtn->rect.y, doneBtn->rect.w, doneBtn->rect.h)){
                                if(strlen(UserName)<1){
                                    currentGameMode=ERROR_MODE;
                                }else{
                                    tm = *localtime(&t);
                                    sprintf(date, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
                                    strcpy(User.name, UserName);
                                    strcpy(User.date, date);
                                    User.score = atoi(Top);
                                    update_PlayersArray(TopPlayers, User);
                                    update_scores(TopPlayers);
                                    currentGameMode = BEST_SCORE;
                                }
                            }
                        }
                        if (currentGameMode == INIT_MODE){
                            if (poinIntRect(e.button.x, e.button.y, doneBtn->rect.x, doneBtn->rect.y, doneBtn->rect.w, doneBtn->rect.h)){
                                strcpy(InitString2,InitString);
                                NLetters=0;
                                strcpy(InitString, "");
                                strcpy(FinalString, "");
                                strcpy(FinalState, "");
                                currentGameMode = INPUT_MODE;
                            }
                        }
                        if (currentGameMode == FIN_MODE){
                            if (poinIntRect(e.button.x, e.button.y, done2Btn->rect.x, done2Btn->rect.y, done2Btn->rect.w, done2Btn->rect.h)){
                                strcpy(FinalString2,FinalString);
                                NLetters=0;
                                strcpy(InitString, "");
                                strcpy(FinalString, "");
                                strcpy(FinalState, "");
                                currentGameMode = FIN_STATE_MODE;
                            }
                        }
                        if (currentGameMode == FIN_STATE_MODE){
                            if (poinIntRect(e.button.x, e.button.y, doneBtn->rect.x, doneBtn->rect.y, doneBtn->rect.w, doneBtn->rect.h)){
                                strcpy(FinalState2,FinalState);
                                if(strlen(InitString2)<3 || strlen(InitString2)>7 || strlen(FinalString2)<3|| strlen(FinalString2)>7 ){
                                    Mix_PlayChannel(-1, errorSound, 0);
                                    currentGameMode=ERROR_MODE;
                                }else{
                                    start=1;
                                    currentGameMode = COMPUTER_MODE;
                                    selectedGameMode = COMPUTER_MODE;
                                }
                            }
                        }
                        if (currentGameMode == INPUT_MODE){
                            n=checkMouseClickNumbers(e,numberButtons);
                            if(n!=-1){
                                NLetters=0;
                                strcpy(InitString, "");
                                strcpy(FinalString, "");
                                strcpy(FinalState, "");
                                currentGameMode=FIN_MODE;
                            }
                        }
                        //home btn
                        if(currentGameMode==YOU_WON_MODE || currentGameMode== BOT_WON_MODE||currentGameMode==LOST_MODE|| currentGameMode==ERROR_MODE||currentGameMode==NO_SAVED){
                             if (poinIntRect(e.button.x, e.button.y, homeBtn->rect.x, homeBtn->rect.y, homeBtn->rect.w, homeBtn->rect.h))
                                    currentGameMode = HOME;
                        }
                        //mouse btn
                        if(currentGameMode==MOUSE_KEY){
                              if ((e.button.x > 345) && (e.button.x < 509) &&
                                    (e.button.y > 335) && (e.button.y < 473)) {
                                    currentGameMode = ONE_PLAYER_MODE;
                                    selectedGameMode = ONE_PLAYER_MODE;
                                    mouse=true;
                                    entered=true;
                             }
                             if ((e.button.x > 694) && (e.button.x < 858) &&
                                    (e.button.y > 335) && (e.button.y < 473)) {
                                    currentGameMode = ONE_PLAYER_MODE;
                                    selectedGameMode = ONE_PLAYER_MODE;
                                    mouse=false;
                                    entered=true;
                             }
                        }
                break;
            }
        }
        SDL_RenderClear(renderer);
        // Background
        SDL_Rect background_destination = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_RenderCopy(renderer, Background, NULL, &background_destination);
        //Draw content
        drawContent(menuButtons2,pauseBtn,backBtn,doneBtn,done2Btn,numberButtons,homeBtn);
        if(currentGameMode==1 && start==1){
            if(savegame==true){
                    if(entered==false){
                        savegame==false;
                        currentGameMode=NO_SAVED;
                    }else{
                        saveTime = read_save(temp2->combinaison,temp2->etat,ctemp->combinaison,ctemp->etat,n);
                        lastTime = saveTime;
                        N=strlen(temp2->combinaison);
                        sprintf_s(Time, 100, "TIME: ");
                        timer= createText(FontRoboto,Color1, renderer, Time, 300, 30);
                        drawText(timer, renderer);
                    }
            }else{
                // trouver n et N random
                N=random(4,7);
                n=random(N/2,N-1);
                sprintf_s(Time, 100, "TIME: 0s");
                timer= createText(FontRoboto,Color1, renderer, Time, 300, 30);
                drawText(timer, renderer);
                temp=NULL;
                /*Mettre l'etat de la premiere combinaison a 11..1*/
                for( j=0;j<N;j++){
                    ce[j]='1';
                }
                ce[j]='\0';
                /*Donner une combinaison aleatoir*/
                random_string(s, N);
                /*la creation des combinaisons possibles*/
                temp=malloc(sizeof(cellule));
                strcpy((char *)temp->combinaison,s);
                strcpy((char *)temp->etat,ce);
                temp->suiv=NULL;
                 /* trouver tous les combinaisons */
                all_Combinaison_Cellule(temp,n);
                ctemp=temp->suiv;
                ctemp2=temp;
                /*calculer le nombres de combinaisons possibles*/
                i=0;
                temp2=temp;
                while(temp!=NULL){
                    i++;
                    temp=temp->suiv;
                }
                /*calculer le numero de la combinaison a atteindre aleatoirement*/
                srand(time(NULL));
                index=rand() % i;
                /*trouver la combinaison final*/
                for(l=1;l<=index;l++){
                        ctemp=ctemp->suiv;
                }
            }
            start=0;
            /* load first string images */
            for(int i=0;i<N;i++){
                    char a[20];
                     if(temp2->etat[i]=='1')
                            path(a,temp2->combinaison,i);
                    else
                            path2(a,temp2->combinaison,i);
                    gameImages[i] = createImage(a, 80+(SCREEN_WIDTH/N)*i , SCREEN_HEIGHT-500, SCREEN_WIDTH/(3*N) , SCREEN_HEIGHT/N, i);
                    textures[i] = loadTexture(gameImages[i]->path);
            }
            /*load final string images */
            for(int i=0;i<N;i++){
                    char a[20];
                    if(ctemp->etat[i]=='1')
                            path(a,ctemp->combinaison,i);
                    else
                            path2(a,ctemp->combinaison,i);
                    finalImages[i] = createImage(a, 80+(SCREEN_WIDTH/N)*i, SCREEN_HEIGHT-250, SCREEN_WIDTH/(3*N) , SCREEN_HEIGHT/N, i);
                    textures[i+N] = loadTexture(finalImages[i]->path);
            }
        }
        if(currentGameMode==2 && start ==1){
                // allouer la combainison initiale
                liste cbni=NULL;
                strcpy(combinaison_initiale,InitString2);
                N=strlen(combinaison_initiale);
                strcpy(etat_initial,"11111");
                // allouer la comb initiale
                cbni=malloc(sizeof(cellule));
                strcpy((char *)cbni->combinaison,combinaison_initiale);
                strcpy((char *)cbni->etat,etat_initial);
                cbni->suiv=NULL;
                i=0;
                while(rotat==false && i<strlen(etat_initial)){
                    if(etat_initial[i]!='1'){
                        rotat=true;
                    }
                    i++;
                }
                // trouver tous les combainaison a partire de la 1ere comb
                all_Combinaison_Cellule(cbni,n);
                // recuperer la combinaison finale
                liste extcbnf=NULL;
                strcpy(combinaison_finale,FinalString2);
                strcpy(etat_final,FinalState2);
                extcbnf=malloc(sizeof(cellule));
                strcpy((char *)extcbnf->combinaison,combinaison_finale);
                strcpy((char *)extcbnf->etat,etat_final);
                extcbnf->suiv=NULL;

                // copy la comb initiale a la tete de liste 2
                liste tete2=NULL;
                tete2=malloc(sizeof(cellule2));
                strcpy((char *)tete2->combinaison,(char *)cbni->combinaison);
                strcpy((char *)tete2->etat,(char *)cbni->etat);
                tete2->suiv=NULL;

                liste2 precedent=tete2;
                liste copy_cbni=cbni->suiv;
                while(copy_cbni!=NULL){
                    Ajouter_Nouv2(precedent,copy_cbni->combinaison,copy_cbni->etat);
                    precedent=precedent->suiv;
                    copy_cbni=copy_cbni->suiv;
                }
                liste tempp=NULL;
                liste2 ctete2=tete2;
                copy_cbni=cbni;
                while(copy_cbni!=NULL){
                    tempp=malloc(sizeof(cellule));
                    Creer_Combinaison_Cellule(copy_cbni,tempp,n);
                    ctete2->suiv2=tempp;
                    copy_cbni=copy_cbni->suiv;
                    ctete2=ctete2->suiv;
                }
                // detruire liste
                Detruire_Liste(cbni);
                ctete2=tete2;
                while(strcmp(ctete2->combinaison,combinaison_finale)!=0 && strcmp(ctete2->etat,etat_final)!=0 ){
                    ctete2=ctete2->suiv;
                }
                pile=malloc(sizeof(cellule));
                strcpy((char *)pile->combinaison,combinaison_finale);
                strcpy((char *)pile->etat,etat_final);
                pile->suiv=NULL;
                liste2 k=Recuperer_Parent(tete2,ctete2);
                Empiler(&pile,k->combinaison,k->etat);
                while(strcmp((char *)k->combinaison,(char *)tete2->combinaison)!=0 && strcmp((char *)k->etat,(char *)tete2->etat)!=0){
                    ctete2=k;
                    k=Recuperer_Parent(tete2,ctete2);
                    Empiler(&pile,k->combinaison,k->etat);
                }
                if(pile!=NULL){
                    pile=pile->suiv;
                }
                start=0;
                /* load first string images */
                for(int i=0;i<N;i++){
                        char a[20];
                        path(a,combinaison_initiale,i);
                        gameImages[i] = createImage(a, 80+(SCREEN_WIDTH/N)*i, SCREEN_HEIGHT-550, SCREEN_WIDTH/(3*N) , SCREEN_HEIGHT/N, i);
                        textures[i] = loadTexture(gameImages[i]->path);
                }
                /*load final string images */
                for(int i=0;i<N;i++){
                        char a[20];
                        if(etat_final[i]=='1')
                                path(a,combinaison_finale,i);
                        else
                                path2(a,combinaison_finale,i);
                        finalImages[i] = createImage(a, 80+(SCREEN_WIDTH/N)*i, SCREEN_HEIGHT-250, SCREEN_WIDTH/(3*N) , SCREEN_HEIGHT/N, i);
                        textures[i+N] = loadTexture(finalImages[i]->path);
                }
        }
        if(currentGameMode==3&& start==1){
             sprintf_s(Time, 100, "TIME: 0s");
             timer= createText(FontRoboto,Color1, renderer, Time, 300, 30);
             drawText(timer, renderer);
             N=random(4,7);
             n=random(N/2,N-1);
             /*Donner une combinaison aleatoir*/
             random_string(initial_string, N);
             int i;
             for(i=0;i<N;i++){
                 initial_state[i]='1';
            }
             initial_state[i]='\0';
             first_noeud=NULL;
             first_noeud=malloc(sizeof(cellule));
             strcpy((char *)first_noeud->combinaison,initial_string);
             strcpy((char *)first_noeud->etat,initial_state);
             first_noeud->suiv=NULL;
             all_Combinaison_Cellule(first_noeud,n);

             tete=NULL;
             tete=malloc(sizeof(cellule2));
             strcpy((char *)tete->combinaison,(char *)first_noeud->combinaison);
             strcpy((char *)tete->etat,(char *)first_noeud->etat);
             tete->suiv=NULL;
             precedent=tete;
             copy_first_noeud=first_noeud->suiv;
             while(copy_first_noeud!=NULL){
                Ajouter_Nouv2(precedent,copy_first_noeud->combinaison,copy_first_noeud->etat);
                precedent=precedent->suiv;
                copy_first_noeud=copy_first_noeud->suiv;
            }

            parc_temp=NULL;
            ctete=tete;
            copy_first_noeud=first_noeud;
            while(copy_first_noeud!=NULL){
                parc_temp=malloc(sizeof(cellule));
                Creer_Combinaison_Cellule(copy_first_noeud,parc_temp,n);
                ctete->suiv2=parc_temp;
                copy_first_noeud=copy_first_noeud->suiv;
                ctete=ctete->suiv;
            }
            copy_first_noeud=first_noeud;
            int number_of_combinaisons=0;
            while(copy_first_noeud!=NULL){
                number_of_combinaisons++;
                copy_first_noeud=copy_first_noeud->suiv;
            }
            srand(time(NULL));
            int index=rand() % number_of_combinaisons;
            copy_first_noeud=first_noeud;
            for(i=1;i<=index;i++){
                 copy_first_noeud=copy_first_noeud->suiv;
            }
            strcpy(final_string,(char *)copy_first_noeud->combinaison);
            strcpy(final_state,(char *)copy_first_noeud->etat);

            ctete=tete;
            while(strcmp(ctete->combinaison,final_string)!=0 || strcmp(ctete->etat,final_state)!=0 ){
                ctete=ctete->suiv;
            }
           pile2=malloc(sizeof(cellule));
           strcpy((char *)pile2->combinaison,final_string);
           strcpy((char *)pile2->etat,final_state);
           pile2->suiv=NULL;
           liste2 parent=Recuperer_Parent(tete,ctete);
           Empiler(&pile2,parent->combinaison,parent->etat);
           while(strcmp((char *)parent->combinaison,(char *)tete->combinaison)!=0 ||  strcmp((char *)parent->etat,(char *)tete->etat)!=0){
                   ctete=parent;
                   parent=Recuperer_Parent(tete,ctete);
                   Empiler(&pile2,parent->combinaison,parent->etat);
            }
          /* load first string images */
            for(int i=0;i<N;i++){
                    char a[20];
                    path(a,initial_string,i);
                    gameImages[i] = createImage(a, 80+(SCREEN_WIDTH/N)*i , SCREEN_HEIGHT-600, SCREEN_WIDTH/(3*N) , SCREEN_HEIGHT/N, i);
                    textures[i] = loadTexture(gameImages[i]->path);
            }
            /* load first bot images */
            for(int i=0;i<N;i++){
                    char a[20];
                    path(a,initial_string,i);
                    botImages[i] = createImage(a, 80+(SCREEN_WIDTH/N)*i , SCREEN_HEIGHT-400, SCREEN_WIDTH/(3*N) , SCREEN_HEIGHT/N, i);
                    textures[i+2*N] = loadTexture(botImages[i]->path);
            }
            /*load final string images */
            for(int i=0;i<N;i++){
                    char a[20];
                    if(final_state[i]=='1')
                            path(a,final_string,i);
                    else
                            path2(a,final_string,i);
                    finalImages[i] = createImage(a, 80+(SCREEN_WIDTH/N)*i, SCREEN_HEIGHT-200, SCREEN_WIDTH/(3*N) , SCREEN_HEIGHT/N, i);
                    textures[i+N] = loadTexture(finalImages[i]->path);
            }
           start=0;
        }
        if(currentGameMode==1){
            if (savegame)
				currentTime = SDL_GetTicks() - pauseTime - OutTime + saveTime;
			else
				currentTime = SDL_GetTicks() - pauseTime - OutTime;
            if (currentTime > lastTime + 1000) {
                lastTime = currentTime - currentTime%1000;
                sprintf_s(Time, 100, "TIME: %ds", currentTime/1000);
                sprintf_s(Top, 100, "%ds", currentTime/1000);
                timer= createText(FontRoboto,Color1, renderer, Time, 300, 30);
            }
            drawText(timer, renderer);
             for(int i=0;i<N;i++){
                SDL_RenderCopy(renderer,textures[i],NULL,&gameImages[i]->rect);
            }
            for(int i=0;i<N;i++){
                SDL_RenderCopy(renderer,textures[i+N],NULL,&finalImages[i]->rect);
            }
            //Score
            sprintf(Score,"BEST:%ds", TopPlayers[0].score);
            Scores[0] = createText(FontRoboto,Color1,renderer,Score,600,30);
            drawText(Scores[0],renderer);
            //lettres
            sprintf(lettres,"Letters(n):%d",n);
            Lettres= createText(FontRoboto,Color1,renderer,lettres,1000,30);
            drawText(Lettres,renderer);
        }
        if(currentGameMode==2){
            currentTime = SDL_GetTicks() - pauseTime - OutTime;
            if (currentTime > lastTime + 1000 && pile!=NULL) {
                for(int j=0;j<N;j++){
                    if(pile->etat[j]=='1'){
                        char a[20];
                        path(a,pile->combinaison,j);
                        textures[j]=loadTexture(a);
                    }
                    else{
                        char a[20];
                        path2(a,pile->combinaison,j);
                        textures[j]=loadTexture(a);
                    }
                }
                pile=pile->suiv;
                if(pile==NULL){
                    Mix_PlayChannel(-1, wonSound, 0);
                    pile=NULL;
                }
            }
            lastTime = currentTime - currentTime%1000;
            for(int i=0;i<N;i++){
                SDL_RenderCopy(renderer,textures[i],NULL,&gameImages[i]->rect);
            }
            for(int i=0;i<N;i++){
                SDL_RenderCopy(renderer,textures[i+N],NULL,&finalImages[i]->rect);
            }
        }
        if(currentGameMode==3){
            currentTime = SDL_GetTicks() - pauseTime - OutTime;
            if (currentTime > lastTime + 1000 && pile2!=NULL) {
                for(int j=0;j<N;j++){
                    if(pile2->etat[j]=='1'){
                        char a[20];
                        path(a,pile2->combinaison,j);
                        textures[j+2*N]=loadTexture(a);
                    }
                    else{
                        char a[20];
                        path2(a,pile2->combinaison,j);
                        textures[j+2*N]=loadTexture(a);
                    }
                }
                pile2=pile2->suiv;
                if(pile2==NULL){
                    Mix_PlayChannel(-1, lostSound, 0);
                    lost=true;
                }
            }
            if(pile2!=NULL){
                if (currentTime > lastTime + 1000) {
                    lastTime = currentTime - currentTime%1000;
                    sprintf_s(Time, 100, "TIME: %ds", currentTime/1000);
                    sprintf_s(Top, 100, "%ds", currentTime/1000);
                    timer= createText(FontRoboto,Color1, renderer, Time, 300, 30);
                }
                drawText(timer, renderer);
                lastTime = currentTime - currentTime%1000;
                //lettres
                sprintf(lettres,"Letters(n):%d",n);
                Lettres= createText(FontRoboto,Color1,renderer,lettres,1000,30);
                drawText(Lettres,renderer);
            }
            for(int i=0;i<N;i++){
                SDL_RenderCopy(renderer,textures[i],NULL,&gameImages[i]->rect);
            }
            for(int i=0;i<N;i++){
                SDL_RenderCopy(renderer,textures[i+N],NULL,&finalImages[i]->rect);
            }
            for(int i=0;i<N;i++){
                SDL_RenderCopy(renderer,textures[i+2*N],NULL,&botImages[i]->rect);
            }
        }
        if(currentGameMode==INIT_MODE){
            InitStringTexture = createText(Font2,Color2,renderer,InitString,440,350);
            drawText(InitStringTexture,renderer);
             //First string
            sprintf(strings,"Type the First string:");
            stringss = createText(FontSofia1,Color1,renderer,strings,250,25);
            drawText(stringss,renderer);
        }
        if(currentGameMode==FIN_MODE){
            FinalStringTexture = createText(Font2,Color2,renderer,FinalString,440,350);
            drawText(FinalStringTexture,renderer);
             //Final string
            sprintf(strings,"Type the Final string:");
            stringss = createText(FontSofia1,Color1,renderer,strings,250,25);
            drawText(stringss,renderer);
        }
        if(currentGameMode==FIN_STATE_MODE){
            FinalStateTexture = createText(Font2,Color2,renderer,FinalState,440,350);
            drawText(FinalStateTexture,renderer);
             //Final strate
            sprintf(strings,"Type the Final state:");
            stringss = createText(FontSofia1,Color1,renderer,strings,250,25);
            drawText(stringss,renderer);
        }
        if(currentGameMode==WIN_MODE){
            UserNameTexture = createText(Font2,Color2,renderer,UserName,440,350);
            drawText(UserNameTexture,renderer);
            //Final string
            sprintf(strings,"Write your name:");
            stringss = createText(FontSofia1,Color1,renderer,strings,250,25);
            drawText(stringss,renderer);
        }
        if(currentGameMode==INPUT_MODE){
                //Final string
                sprintf(strings,"Number of letters to rotate:");
                stringss = createText(FontSofia1,Color1,renderer,strings,250,25);
                drawText(stringss,renderer);
         }
        if(currentGameMode==BEST_SCORE){
            // SCORES
			for (i = 0; i < 5; i++){
				sprintf(Name, "%d-%s",i+1, TopPlayers[i].name);
                sprintf(Score,"[%ds]", TopPlayers[i].score);
                sprintf(Date,"(%s)", TopPlayers[i].date);
                Names[i]=createText(Font2,Color2,renderer,Name,400,250+50*i);
                Scores[i]=createText(Font2,Color3,renderer,Score,600,250+50*i);
                Dates[i]=createText(Font2,Color2,renderer,Date,700,250+50*i);
                drawText(Names[i], renderer);
                drawText(Scores[i], renderer);
                drawText(Dates[i], renderer);
			}
        }
        if(currentGameMode==LOST_MODE){
            sprintf(strings,"You are not in top 5");
            stringss = createText(FontSofia2,Color3,renderer,strings,380,320);
            drawText(stringss,renderer);
        }
        if(currentGameMode==YOU_WON_MODE){
            sprintf(strings,"You won , Congrats !!");
            stringss = createText(FontSofia2,Color3,renderer,strings,380,320);
            drawText(stringss,renderer);
        }
        if(currentGameMode==BOT_WON_MODE){
            sprintf(strings,"Bot won , Try again !!");
            stringss = createText(FontSofia2,Color3,renderer,strings,380,320);
            drawText(stringss,renderer);
        }
        if(currentGameMode==ERROR_MODE){
            sprintf(strings,"Error !! , Try again .");
            stringss = createText(FontSofia2,Color3,renderer,strings,380,320);
            drawText(stringss,renderer);
        }
        if(currentGameMode==NO_SAVED){
            sprintf(strings,"Nothing Saved !! , Try again .");
            stringss = createText(FontSofia2,Color3,renderer,strings,320,320);
            drawText(stringss,renderer);
        }
        if(currentGameMode==HOME){
            SDL_Rect background_destination = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, Menu, NULL, &background_destination);
        }
        if(currentGameMode==MOUSE_KEY){
            SDL_Rect background_destination = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, MouseKey, NULL, &background_destination);
        }
        if(currentGameMode==MENU){
            SDL_Rect background_destination = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, PauseMenu, NULL, &background_destination);
        }
        // Show the rendered content
        SDL_RenderPresent(renderer);
        if((lost==true&&gagner==true&&currentGameMode==3)){
            SDL_Delay(1000);
            currentGameMode=BOT_WON_MODE;
        }
        if(gagner==true&&lost==false&&currentGameMode==3){
            SDL_Delay(1000);
            currentGameMode=YOU_WON_MODE;
        }
        if(gagner==true&&currentGameMode==1){
            SDL_Delay(2000);
            currentGameMode=WIN_MODE;
        }
        if(lost==true&&currentGameMode==1){
            SDL_Delay(2000);
            currentGameMode=LOST_MODE;
        }
        SDL_Delay(30);
    }
    // Free resources
    for(int i=0;i<N;i++){
        SDL_DestroyTexture(textures[i]);
        textures[i] = NULL;
    }
    for(int i=0;i<5;i++){
        SDL_DestroyTexture(Names[i]);
        SDL_DestroyTexture(Scores[i]);
        SDL_DestroyTexture(Dates[i]);
    }
    SDL_DestroyTexture(UserNameTexture);
    SDL_DestroyTexture(InitStringTexture);
    SDL_DestroyTexture(FinalStringTexture);
    SDL_DestroyTexture(circleBtn);
    SDL_DestroyTexture(UserInput);
    SDL_DestroyTexture(YouWon);
    SDL_DestroyTexture(PauseMenu);
    SDL_DestroyTexture(MouseKey);
    SDL_DestroyTexture(stringss);
    SDL_DestroyTexture(FinalStateTexture);
    SDL_DestroyTexture(Background);
    SDL_DestroyTexture(Btn);
    SDL_DestroyTexture(Lettres);
    SDL_DestroyTexture(timer);
    SDL_DestroyTexture(Best);
    IMG_Quit();
    Mix_FreeChunk(wonSound);
    Mix_FreeChunk(clickSound);
    Mix_FreeMusic(backgroundSound);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
