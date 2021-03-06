/*
** Filename : card.c
**
** Made by  : Baptiste LEGO
**
** Description  : card functions
*/

#include <time.h>
#include "../../headers/common.h"
#include "../../headers/play/card.h"
#include "../../headers/model/cardModel.h"

enum {WIN, FAIL};

void newCard(App *app, SelectQuery cards, char *backgroundColor){
    SDL_Rect pageButton;
    SDL_Event event;
    int done = 0;
    int cardRow;

    // TAKE RANDOM CARD
    cardRow = randomCard(app, cards);
    if(cardRow < 0){
        printf("Error\n");
        quitApp(app);
        exit(EXIT_FAILURE);
    }

    while (!done) {
        SDL_WaitEvent(&event);
        commonEvents(app, event, &done);
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT){
                    if(inRect(pageButton, event.button.x, event.button.y)){
                        cardResponse(app, cards.listColumnsRows[cardRow], backgroundColor);
                        done = 1;
                    }
                }
            break;
        }
        displayQuestion(app, cards.listColumnsRows[cardRow], &pageButton, backgroundColor);
    }
}

void cardResponse(App *app, char **card, char *backgroundColor){
    SDL_Rect pageButtons[2];
    SDL_Event event;
    int done = 0;

    while (!done) {
        SDL_WaitEvent(&event);
        commonEvents(app, event, &done);
        done = 0; // Block the return button --> you have to say if it's true or false to leave
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT){
                    if(inRect(pageButtons[WIN], event.button.x, event.button.y)){
                        setNewCardValidation(app, card[ID]);
                        done = 1;
                    }
                    if(inRect(pageButtons[FAIL], event.button.x, event.button.y)){
                        done = 1;
                    }
                }
            break;
        }
        displayResponse(app, card, pageButtons, backgroundColor);
    }
}

int randomCard(App *app, SelectQuery cards){
    int i, random, counter = 0;

    random = rand() % numberOfValidCards(app, cards);

    for(i = 0; i < cards.numberRows; i++){
        if(counter == random && isValidCard(cards.listColumnsRows[i]))
            return i;
        if(isValidCard(cards.listColumnsRows[i]))
            counter++;
    }

    return -1;
}

int numberOfValidCards(App *app, SelectQuery cards){
    int i, counter = 0;

    for(i = 0; i < cards.numberRows; i++){
        if(isValidCard(cards.listColumnsRows[i]))
            counter++;
    }

    return counter;
}

int isValidCard(char **card){
    time_t nowSeconds = time(NULL);
    int diffSeconds, diffHour, diffMin;

    diffSeconds = (int)nowSeconds - intConvertor(card[VALIDATED_DATE]);
    diffMin = diffSeconds / 60;
    diffHour = diffSeconds / 3600;

    switch (intConvertor(card[COUNT])){
        case 0:
            return 1;
        break;
        case 1:
            if(diffMin < 10)
                return 0;
        break;
        case 2:
            if(diffHour < 24)
                return 0;
        break;
        case 3:
            if(diffHour < 7*24)
                return 0;
        break;
        case 4:
            if(diffHour < 30*24)
                return 0;
        break;
        case 5:
            if(diffHour < 68*24)
                return 0;
        break;
    }

    if(intConvertor(card[COUNT]) > 5)
        return 0;

    return 1;
}


/*///////////////////////// DISPLAY \\\\\\\\\\\\\\\\\\\\\\\\\\*/


void displayQuestion(App *app, char **card, SDL_Rect *pageButton, char *backgroundColor){
    Uint8 *rgbBackground = hexToRgb(backgroundColor);
    SDL_Rect questionPos = {wRatio16(app, 1), hRatio9(app, 3), wRatio16(app, 14), hRatio9(app, 2)};

    // Setting background
    SDL_SetRenderDrawColor(app->renderer, rgbBackground[0], rgbBackground[1], rgbBackground[2], rgbBackground[3]);
    SDL_RenderClear(app->renderer);

    // Texts
    writeTitle(app, card[NAME]);
    writeReturnKey(app);

    // Write Question
    renderText(app, questionPos, app->config.fontCambriab, card[QUESTION], 50, TEXT_BLENDED, app->colors.white);

    // Button
    *pageButton = createRect(app, wRatio16(app, 5), hRatio9(app, 1), wRatio16(app, 5.5), hRatio9(app, 7) + 40, app->colors.yellow);
    renderText(app, *pageButton, app->config.fontCambriab, "VOIR REPONSE", 70, TEXT_BLENDED, app->colors.white);

    // Refresh screen
    SDL_RenderPresent(app->renderer);
    free(rgbBackground);
}

void displayResponse(App *app, char **card, SDL_Rect *pageButtons, char *backgroundColor){
    Uint8 *rgbBackground = hexToRgb(backgroundColor);
    SDL_Rect answerPos = {wRatio16(app, 1), hRatio9(app, 3), wRatio16(app, 14), hRatio9(app, 2)};

    // Setting background
    SDL_SetRenderDrawColor(app->renderer, rgbBackground[0], rgbBackground[1], rgbBackground[2], rgbBackground[3]);
    SDL_RenderClear(app->renderer);

    // Texts
    writeTitle(app, card[NAME]);

    // Write Answer
    renderText(app, answerPos, app->config.fontCambriab, card[ANSWER], 50, TEXT_BLENDED, app->colors.white);

    // Button
    pageButtons[WIN] = createRect(app, wRatio16(app, 3), hRatio9(app, 1), wRatio16(app, 4), hRatio9(app, 7) + 40, app->colors.green);
    renderText(app, pageButtons[WIN], app->config.fontCambriab, "J'AI BON !", 70, TEXT_BLENDED, app->colors.white);

    pageButtons[FAIL] = createRect(app, wRatio16(app, 3), hRatio9(app, 1), wRatio16(app, 9), hRatio9(app, 7) + 40, app->colors.red);
    renderText(app, pageButtons[FAIL], app->config.fontCambriab, "J'AI FAUX :(", 70, TEXT_BLENDED, app->colors.white);

    // Refresh screen
    SDL_RenderPresent(app->renderer);
    free(rgbBackground);
}
