/*
** Filename : list.c
**
** Made by  : Baptiste LEGO
**
** Description  : list menu main functions
*/

#include "../../headers/common.h"
#include "../../headers/play/box.h"
#include "../../headers/play/list.h"
#include "../../headers/play/game.h"
#include "../../headers/model/listModel.h"

void listMenu(App *app, SelectQuery boxes, int page, int listNumber){
    SDL_Rect *listButtons = NULL;
    int *id = NULL;
    int i, done = 0;
    int numberOfLists = 0;
    SDL_Event event;
    SelectQuery lists;

    // Getting data
    lists = getListsFromBox(app, boxes.listColumnsRows[(listNumber+page) + page*9][ID]);

    listButtons = malloc(lists.numberRows * sizeof(SDL_Rect));
    verifyPointer(app, listButtons, "Can't allocate memory for listButtons\n");

    id = malloc(sizeof(int) * lists.numberRows);
    verifyPointer(app, id, "Can't allocate memory for id\n");

    while (!done) {
        SDL_WaitEvent(&event);
        commonEvents(app, event, &done);
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT){
                    // Display a list
                    for(i = 0; i < numberOfLists; i++){
                        if(inRect(listButtons[i], event.button.x, event.button.y))
                            game(app, lists.listColumnsRows[i], id[i]);
                    }
                }
            break;
        }
        numberOfLists = displayHomeBox(app, lists, listButtons, boxes.listColumnsRows[(listNumber+page) + page*9][NAME], id);
    }

    free(listButtons);
    free(id);
}


/*///////////////////////// DISPLAY \\\\\\\\\\\\\\\\\\\\\\\\\\*/


int displayHomeBox(App *app, SelectQuery lists, SDL_Rect *listButtons, char* boxTitle, int *id){
    SDL_Rect textPos;
    int numberOfLists = 0;

    // Setting background
    SDL_SetRenderDrawColor(app->renderer, app->colors.blue[0], app->colors.blue[1], app->colors.blue[2], app->colors.blue[3]);
    SDL_RenderClear(app->renderer);

    // Write title
    writeTitle(app, boxTitle);
    writeReturnKey(app);

    // Creating lists
    if(lists.numberRows > 0)
        numberOfLists =  createListPage(app, lists, listButtons, id, lists.numberRows);
    else{
        textPos.h = hRatio9(app, 2);
        textPos.w = wRatio16(app, 10);
        textPos.x = wRatio16(app, 3);
        textPos.y = hRatio9(app, 3);
        renderText(app, textPos, app->config.fontCambriab, "Cette boite est vide", 50, TEXT_BLENDED, app->colors.white);
    }

    SDL_RenderPresent(app->renderer);

    return numberOfLists;
}

int createListPage(App *app, SelectQuery lists, SDL_Rect *buttons, int *id, int size){
    int x, y, i = 0;
    int xBox = 0, yBox = 0;

    for(x = 0; x < 5; x++){
        for(y = 0; y < 2; y++){
            if(i >= size)
                break;

            // Create square
            xBox = ((app->config.width / 3) * y) + 10*y + app->config.width/5;
            yBox = ((app->config.height / 8) * x) + 10*x + app->config.height/6;
            buttons[i] = createRect(app, app->config.height / 8, app->config.height / 8, xBox, yBox, hexToRgb(lists.listColumnsRows[i][L_COLOR]));
            id[i] = (int)strtol(lists.listColumnsRows[i][ID], NULL, 0);

            // Create texts
            renderButtonLabel(app, lists.listColumnsRows[i], buttons[i]);
            i++;
        }
        if(i >= size)
            break;
    }

    // Return number of lists printed
    return i;
}
