#ifndef _STRUCTURE
#define _STRUCTURE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <winsock2.h>
#include <mysql.h>

#define WINDOW_POS_X    200
#define WINDOW_POS_Y    100
#define SCREEN_FORMAT   2
#define MAX_VARCHAR 255
#define MAX_TEXT 1000

typedef enum BindType{
    BIND_INPUT,
    BIND_OUTPUT
}BindType;

typedef enum typeRenderText{
    TEXT_SOLID,
    TEXT_BLENDED
}typeRenderText;

typedef struct Colors {
    Uint8 blue[4];
    Uint8 lightblue[4];
    Uint8 green[4];
    Uint8 yellow[4];
    Uint8 red[4];
    Uint8 pink[4];
    SDL_Color black;
    SDL_Color white;
} Colors;

typedef struct Config {
    int width;
    int height;
    char host[MAX_VARCHAR];
    char user[MAX_VARCHAR];
    char password[MAX_TEXT];
    char database[MAX_VARCHAR];
    char fontCambriab[MAX_VARCHAR];
    char fontSixty[MAX_VARCHAR];
    char fontTimes[MAX_VARCHAR];
} Config;

/* Model */

typedef char Varchar[MAX_VARCHAR];
typedef char Text[MAX_TEXT];
/**
*@struct MySqlTable : pour avoir les informations d'une table, n�cessaire lors d'une requ�te pr�par�e
*@var (char) tableName : le nom de la table dans une base de donn�es
*@var (char **) listFieldsNames : la liste des champs de la table
*@var (int) numberField : le nombre de champs de la table
*@var (unsigned int *) listFieldsTypes : la liste des type MYSQL_TYPE_ correspondant chaque index au nom du champ concern�
*/
typedef struct MySqlTable {
    Varchar         tableName;
    Varchar         *listFieldsNames;
    int             numberField;
    unsigned int    *listFieldsTypes;
} MySqlTable;

/* Query to the database*/
//Les structures pour la partie bind
typedef struct MySqlParamsBind {
    int             paramsTypes;
    char            *paramsString;
    int             paramsStrLen;
    int             paramNumber;
    double          paramDouble;
    MYSQL_TIME      paramsDateTime;
    short           paramsIsNull;
    unsigned long   paramsLengths;
    short           paramError;
} MySqlParamsBind;

typedef struct MySqlStmtManager {
    MYSQL_STMT      *stmt;
    short           ifStmtIsInit;
    char            *currentQuery;
    Varchar         *tablesNames;
    int             numberTables;
    Varchar         *paramsNames;
    int             numberParams;
    MySqlParamsBind *params;
    MYSQL_BIND      *buffersBind;
    BindType        BindInOut;
} MySqlStmtManager;

// Concernant les requ�tes pr�par�es
/**
*@struct SelectQuery : to get informations of select query
*
*@var (MYSQ_RES) - *result : mysql data structure that content result's informations of select query
*@var (char ***) - listColumnsRows : list of columns and rows correspond of result of query
*@var (Varchar *) - listFields : list of fields names of query
*@var (unsigned int) - numberFields : number of fields of the query select's result
*@var (unsigned int) - numberRows : number of rows of query select's result
*@var (short) - resultWithFieldsList : bool if listColumnsRows content list of fields
*/
typedef struct SelectQuery {
    MYSQL_RES       *result;
    char            ***listColumnsRows;
    Varchar         *listFields;
    unsigned int    numberFields;
    unsigned int    numberRows;
    short           resultWithFieldsList;
    short           mysqlResultBool;
} SelectQuery;

/**
*@struct InsertParamFinder : to help to get fields of params values
*@var (Varchar*) listFieldsParenthesis : list of content ? or values in specific parenthesis
*@var (short) - listBeforeWordValues : bool if there are list of fields in parenthesis before word "VALUES"
*@var (int) - numberFields : number of fields of columns that is specifies in insert query
*@var (int*) - indexsOfQParenthesis : list of index corredpond to the ? marks placements
*@var (Varchar*) listFieldsParenthesis : list of fields in specific parenthesis
*/
typedef struct InsertParamFinder{
    Varchar *listContentParenthesis;
    short   listBeforeWordValues;
    int     numberFields;
    int     *indexsOfQParenthesis;
    Varchar *listFieldsParenthesis;
}InsertParamFinder;

typedef struct Query {
    MySqlStmtManager    stmtManager;
    SelectQuery         selectQuery;
    InsertParamFinder   paramFinder;
} Query;

typedef struct Model {
    MYSQL       *mysql;
    short       ifMysqlIsInit;
    MySqlTable  *tables;
    Varchar     *listAllTables;
    int         numberAllTables;
    Query       query;
} Model;

/* --View-- */

typedef struct Position{
    int startX;
    int startY;
    int stepX;
    int stepY;
} Position;

typedef struct DisplayManager {
    int currentPage;
    int nbElements;
    int nbRows;
    int nbColumns;
    int elementH;
    int elementW;
    Position pos;
} DisplayManager;


/* -- Input text -- */
typedef struct ListFields {
    Varchar *list;
    int numberFields;
} ListFields;

typedef struct InputChar {
    SDL_Rect charRect;
    char oneChar[3];
} InputChar;

typedef struct ListInputText {
    InputChar inputChar;
    struct ListInputText *next;
} ListInputText;

typedef enum KeyAction{
    STAND_BY,
    C_RIGHT,
    C_LEFT,
    ADD_CHAR,
    B_DELETE,
    R_DELETE,
    SB_DELETE
} KeyAction;

typedef struct TextsInput{
    ListInputText *listChar;
    SDL_Rect firstRect;
    int size;
    int nbChar;
    int cursor;
    int start;
    int end;
    int maxLength;
    KeyAction action;
} TextsInput;

typedef struct InputManager{
    SDL_Rect rectInputs[3];
    TextsInput textInput;
    char label[MAX_VARCHAR];
    char error[MAX_VARCHAR];
    int active;
} InputManager;



/* -- Create -- */

typedef struct TextRender{
    SDL_Rect rect;
    int length;
    char text[MAX_VARCHAR];
}TextRender;

typedef struct CreateInfo{
    SDL_Rect titleRect;
    SDL_Rect questionRect;
    SDL_Rect answerRect;
    SDL_Rect dateRect;
    SDL_Color textColor;
    char tableName[MAX_VARCHAR];
    char title[MAX_VARCHAR];
    char question[MAX_VARCHAR];
    char answer[MAX_VARCHAR];
    char datetime[MAX_VARCHAR];
    char childTable[MAX_VARCHAR];
    int idParent;
}CreateInfo;

typedef struct CreateButtons{
    char buttonText0[20];
    char buttonText1[20];
    SDL_Rect manageButtons[2];
    SDL_Rect rectCreate;
    SDL_Rect rectDelete;
    Uint8 manageColor[2][4];
    SDL_Color colorTCreate;
    SDL_Color colorTDelete;
    int activeDel;
}CreateButtons;

typedef struct CreatePage{
    SDL_Rect *elementButtons;
    SDL_Rect *elementName;
    SDL_Rect *elementDate;
    Uint8 mainPColor[4];
    int nbTotalElements;
    int nbElementMaxPerPage;
    int nbElementsPage;
    Uint8 pageBColor[4];
    SDL_Rect pageButtons[2];
    char next[MAX_VARCHAR];
    char before[MAX_VARCHAR];
    int indexDate;
    int page;
}CreatePage;

/* -- Form -- */
typedef struct QueryForm{
    int idParent;
    int numberFields;
    Varchar tableName;
    Varchar query;
    Varchar *fields;
    Varchar *values;
} QueryForm;

/* --App-- */
typedef struct App {
    SDL_Window      *screen;
    SDL_Renderer    *renderer;
    Colors          colors;
    Config          config;
    Model           model;
} App;



#endif // _STRUCTURE
