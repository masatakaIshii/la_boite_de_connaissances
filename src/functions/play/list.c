/*
** Filename : list.c
**
** Made by  : Baptiste LEGO
**
** Description  : play mode main functions
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "../../headers/common.h"
#include "../../headers/play/box.h"
#include "../../headers/play/list.h"
#include "../../headers/model/listModel.h"

void listMenu(App *app, SelectQuery boxes, int page, int i){
    SDL_Rect *listButtons = NULL;
    SDL_Event event;
    int done = 0;

    // Getting data
    SelectQuery lists = getLists(app, boxes.listColumnsRows[(i+page) + page*9][0]);
    char *boxTitle = boxes.listColumnsRows[(i+page) + page*9][1];
    listButtons = malloc(lists.numberRows * sizeof(SDL_Rect));

    while (!done) {
        SDL_WaitEvent(&event);
        commonEvents(app, event, &done);
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT){
                    // Print a list
                }
            break;
        }
        displayHomeBox(app, lists, listButtons, boxTitle);
    }

    free(listButtons);
}


/*///////////////////////// DISPLAY \\\\\\\\\\\\\\\\\\\\\\\\\\*/


void displayHomeBox(App *app, SelectQuery lists, SDL_Rect *listButtons, char* boxTitle){
    SDL_Rect textPos;

    // Setting background
    SDL_SetRenderDrawColor(app->renderer, app->colors.blue[0], app->colors.blue[1], app->colors.blue[2], app->colors.blue[3]);
    SDL_RenderClear(app->renderer);

    // Write title
    textPos.h = hRatio9(app, 1.5);
    textPos.y = 0;

    if(strlen(boxTitle) < 6){
        textPos.w = wRatio16(app, 2);
        textPos.x = wRatio16(app, 7);
    }
    else if(strlen(boxTitle) < 12){
        textPos.w = wRatio16(app, 4);
        textPos.x = wRatio16(app, 6);
    }
    else{
        textPos.w = wRatio16(app, 6);
        textPos.x = wRatio16(app, 5);
    }

    renderText(app, textPos, app->config.fontCambriab, boxTitle, 80, TEXT_BLENDED, app->colors.white);

    // Creating lists
    if(lists.numberRows > 0)
        createListPage(app, lists, listButtons, lists.numberRows);
    else{
        textPos.h = hRatio9(app, 2);
        textPos.w = wRatio16(app, 10);
        textPos.x = wRatio16(app, 3);
        textPos.y = hRatio9(app, 3);
        renderText(app, textPos, app->config.fontCambriab, "Cette boite est vide", 50, TEXT_BLENDED, app->colors.white);
    }

    SDL_RenderPresent(app->renderer);
}

int createListPage(App *app, SelectQuery lists, SDL_Rect *buttons, int size){
    int x, y, i = 0;
    int xBox = 0, yBox = 0;

    for(x = 0; x < 5; x++){
        for(y = 0; y < 2; y++){
            if(i >= size)
                break;

            // Create square
            xBox = ((app->config.width / 3) * y) + 10*y + app->config.width/5;
            yBox = ((app->config.height / 8) * x) + 10*x + app->config.height/6;
            buttons[i] = createRect(app, app->config.height / 8, app->config.height / 8, xBox, yBox, hexToRgb(lists.listColumnsRows[i][5]));

            // Create texts
            renderButtonLabel(app, lists.listColumnsRows[i], buttons[i]);
            i++;
        }
        if(i >= size)
            break;
    }

    return i; // Return number of elements printed
}
