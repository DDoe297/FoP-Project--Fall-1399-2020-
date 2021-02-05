#include <ncurses.h>
#include <form.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <mxml.h>
#include <curl/curl.h>
#define week 604800
typedef struct
{
    int id;
    char username[50];
    char password[50];
    int healthCondition;
    char firstName[50];
    char lastName[50];
    char nationalCode[11];
    char email[50];
    int businessOwner;
    int alive;
    time_t lastCodeChange;
} user;
typedef struct
{
    int id;
    char storeName[50];
    char storeStreet[50];
    char storeCity[50];
} businessOwner;
typedef struct
{
    int id;
    char bloodType[10];
    char age[10];
    char height[10];
    char weight[10];
} regular;
typedef struct
{
    int userId;
    int from;
    int to;
    time_t date;
} codeChange;
typedef struct
{
    int p1;
    int p2;
    int p1Code;
    int p2Code;
    time_t date;
} regularVisit;
typedef struct
{
    int regularId;
    int businessId;
    int regularCode;
    int businessCode;
    time_t date;
} businessVisit;
typedef struct
{
    int hospitalLocationId;
    char hospitalName[50];
    char hospitalCity[50];
} hospital;
typedef struct
{
    char text[1000];
    int senderId;
    time_t date;
} message;
int totalCases;
char *itoa(long int num);
int getCodeById(int userId);
user *retrieveUser(const char *username);
user *retrieveUserById(int userId);
regular *retrieveRegular(int id);
businessOwner *retrieveBusinessOwner(int id);
int addUser(user *user);
void addbusinessOwner(int id, businessOwner *owner);
void addRegular(int id, regular *regularUser);
int editUser(int userId, const char *edit, const char *value);
int editbusinessOwner(int id, const char *edit, const char *value);
int editRegular(int id, const char *edit, const char *value);
codeChange *codeChangeRetrieve(int userId, int *numberOfItems);
void regularVisitEdit(int regularId1, int regularId2);
regularVisit *regularVisitRetrieve(int userId, int *numberOfItems);
void businessVisitEdit(int regularId, int businessId);
businessVisit *businessVisitRetrieveForUser(int userId, int *numberOfItems);
businessVisit *businessVisitRetrieveForbusiness(int businessId, int *numberOfItems);
int getCodeById(int userId);
void userCodeChangeById(int userId, int change);
user **getRegularList(int *numberOfItems);
user **getbusinessOwnerList(int *numberOfItems);
int getUserCount(void);
void codeChangeRecursive(int user, int to, int timeSpan);
void startMenu(void);
void editInfo(int userId);
char *editInfoPopup(char *item, int x, int y);
char *editInfoPopupPassword(int x, int y);
void createAccount(void);
void userMenu(int user);
void shopMenu(int user);
void login(void);
void allShops(void);
void allUsers(void);
int min(int, int);
char *strlwr(char *string);
char *trimWhitespace(char *string);
void forgetPassword(void);
int checkHealth(void);
int getTotalCases(void);
void avoidCovid19(void);
void adminMenu(void);
hospital *getHospitalsListForUser(int *numberOfItems, int locationId);
void showHospitalsForAdmin(void);
void showHospitalsForUser(int x, int y);
hospital *getHospitalsListForAdmin(int *numberOfItems);
void addHospital(hospital *add);
void addHospitalMenu(void);
void errorPopup(char *message, int x, int y);
int validate(char *pin);
int getLocationID(int x, int y);
void showCodeChangesHistory(int userId);
void weeklyCodeChange(void);
void addRegularVisit(int userId);
void addBussinessVisit(int userId);
void showRegularMeetsHistory(int userId);
void showBussinessMeetsHistory(int userId);
void showVisitors(int userId);
void addMessage(message *newMessage);
message *getMessageList(int *numberOfItems);
void messageMenu(int userId);
void messagePopup(char *message, int x, int y);
void messageList(void);
int main(void)
{
    totalCases = getTotalCases();
    weeklyCodeChange();
    startMenu();
    return 0;
}
void startMenu(void)
{
    int maxY, maxX, boxY, boxX;
    initscr();
    clear();
    noecho();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 7) / 2;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "Hello and Welcome To GetWell App.");
    refresh();
    WINDOW *box = newwin(7, 50, boxY, boxX);
    WINDOW *shadow = newwin(7, 50, boxY + 1, boxX + 2);
    keypad(box, true);
    char *choices[] = {"Login", "Create Account", "Forget Password", "How To Avoid COVID-19", "Exit"};
    int choice, highlight = 0;
    box(box, 0, 0);
    wbkgd(box, COLOR_PAIR(2));
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    while (true)
    {
        for (int i = 0; i < 5; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, i + 1, 1, choices[i]);
            wattroff(box, A_REVERSE);
        }
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            break;
        case KEY_DOWN:
            if (highlight < 4)
            {
                highlight++;
            }
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            break;
        }
    }
    wrefresh(box);
    switch (highlight)
    {
    case 0:
        login();
        break;
    case 1:
        createAccount();
        break;
    case 2:
        forgetPassword();
        break;
    case 3:
        avoidCovid19();
        break;
    case 4:
        clear();
        endwin();
        exit(0);
        break;
    default:
        break;
    }
    endwin();
}
void avoidCovid19(void)
{
    int maxY, maxX, boxY, boxX;
    initscr();
    clear();
    noecho();
    start_color();
    curs_set(0);
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 9) / 2;
    boxX = (maxX - 86) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "How to prevent COVID-19");
    mvprintw(boxY + 10, boxX, "Prees any key to exit.");
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(9, 86, boxY, boxX);
    WINDOW *shadow = newwin(9, 86, boxY + 1, boxX + 2);
    WINDOW *insideBox = derwin(box, 7, 84, 1, 1);
    box(box, 0, 0);
    wbkgd(box, COLOR_PAIR(2));
    wbkgd(insideBox, COLOR_PAIR(2));
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    wrefresh(box);
    wrefresh(insideBox);
    wattron(insideBox, A_BOLD);
    mvwprintw(insideBox, 0, 0, "To prevent the spread of COVID-19:");
    wattroff(insideBox, A_BOLD);
    mvwprintw(insideBox, 1, 0, "Clean your hands often. Use soap and water, or an alcohol-based hand rub.");
    mvwprintw(insideBox, 2, 0, "Maintain a safe distance from anyone who is coughing or sneezing.");
    mvwprintw(insideBox, 3, 0, "Wear a mask when physical distancing is not possible.");
    mvwprintw(insideBox, 4, 0, "Don't touch your eyes, nose or mouth.");
    mvwprintw(insideBox, 5, 0, "Cover your nose and mouth with your bent elbow or a tissue when you cough or sneeze.");
    mvwprintw(insideBox, 6, 0, "Stay home if you feel unwell.");
    mvwprintw(insideBox, 7, 0, "If you have a fever, cough and difficulty breathing, seek medical attention.");
    wrefresh(insideBox);
    getch();
    endwin();
    startMenu();
}
char *editInfoPopup(char *item, int x, int y)
{
    int ch;
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    curs_set(1);
    noecho();
    WINDOW *popUp;
    FIELD *fields[2];
    FORM *form;
    for (int i = 0; i < 2; i++)
    {
        fields[i] = NULL;
    }
    popUp = newwin(4, 50, y + 3, x);
    wclear(popUp);
    wbkgd(popUp, COLOR_PAIR(2));
    box(popUp, 0, 0);
    mvwprintw(popUp, 1, 1, item);
    fields[0] = new_field(1, 46, 0, 0, 0, 0);
    set_field_back(fields[0], A_UNDERLINE | COLOR_PAIR(2));
    set_field_fore(fields[0], COLOR_PAIR(2));
    field_opts_on(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
    form = new_form(fields);
    set_form_win(form, popUp);
    set_form_sub(form, derwin(popUp, 1, 48, 2, 1));
    post_form(form);
    wrefresh(popUp);
    keypad(popUp, true);
    while (ch != '\n')
    {
        ch = wgetch(popUp);
        switch (ch)
        {
        case KEY_DOWN:
        case KEY_UP:
            break;
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case '\n':
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_NEXT_FIELD);
            if (strlen(trimWhitespace(field_buffer(fields[0], 0))) != 0)
            {
                wclear(popUp);
                box(popUp, 0, 0);
                mvwprintw(popUp, 1, 1, "Info changed successfuly.");
                mvwprintw(popUp, 2, 1, "Press any key to continue.");
                curs_set(0);
                wrefresh(popUp);
                getch();
                unpost_form(form);
                free_form(form);
                return trimWhitespace(field_buffer(fields[0], 0));
            }
            else
            {
                wclear(popUp);
                box(popUp, 0, 0);
                mvwprintw(popUp, 1, 1, "Invalid Input.");
                mvwprintw(popUp, 2, 1, "Press any key to continue.");
                curs_set(0);
                wrefresh(popUp);
                getch();
                unpost_form(form);
                free_form(form);
                free_field(fields[0]);
                return NULL;
            }
            break;
        case KEY_END:
            ch = '\n';
            break;
        default:
            form_driver(form, ch);
            break;
        }
    }
}
void errorPopup(char *message, int x, int y)
{
    int ch;
    start_color();
    init_pair(2, 0, 12);
    curs_set(0);
    noecho();
    WINDOW *popUp;
    popUp = newwin(4, 50, y + 3, x);
    wclear(popUp);
    wbkgd(popUp, COLOR_PAIR(2));
    box(popUp, 0, 0);
    mvwprintw(popUp, 1, 1, message);
    mvwprintw(popUp, 2, 1, "Press any key to continue.");
    wrefresh(popUp);
    getch();
}
char *editInfoPopupPassword(int x, int y)
{
    int ch;
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    noecho();
    curs_set(1);
    char password[50], passwordConfirmation[50];
    WINDOW *popUp;
    FIELD *fields[3];
    FORM *form;
    popUp = newwin(5, 50, y + 3, x);
    wclear(popUp);
    wbkgd(popUp, COLOR_PAIR(2));
    box(popUp, 0, 0);
    mvwprintw(popUp, 1, 1, "Please enter your new password and confirmation:");
    fields[0] = new_field(1, 46, 0, 0, 0, 0);
    fields[1] = new_field(1, 46, 1, 0, 0, 0);
    fields[2] = NULL;
    for (int i = 0; i < 2; i++)
    {
        set_field_back(fields[i], A_UNDERLINE | COLOR_PAIR(2));
        set_field_fore(fields[i], COLOR_PAIR(2));
        field_opts_on(fields[i], O_VISIBLE | O_AUTOSKIP | O_EDIT | O_ACTIVE);
        field_opts_off(fields[i], O_PUBLIC);
    }
    form = new_form(fields);
    set_form_win(form, popUp);
    set_form_sub(form, derwin(popUp, 2, 48, 2, 1));
    post_form(form);
    wrefresh(popUp);
    keypad(popUp, true);
    while (ch != '\n')
    {
        ch = wgetch(popUp);
        switch (ch)
        {
        case KEY_DOWN:
        case KEY_UP:
            form_driver(form, REQ_NEXT_FIELD);
            break;
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case '\n':
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_NEXT_FIELD);
            if (strlen(trimWhitespace(field_buffer(fields[0], 0))) != 0 && strcmp(trimWhitespace(field_buffer(fields[0], 0)), trimWhitespace(field_buffer(fields[1], 0))) == 0)
            {
                wclear(popUp);
                box(popUp, 0, 0);
                mvwprintw(popUp, 1, 1, "Info changed successfuly.");
                mvwprintw(popUp, 2, 1, "Press any key to continue.");
                curs_set(0);
                wrefresh(popUp);
                getch();
                unpost_form(form);
                free_form(form);
                free_field(fields[1]);
                return trimWhitespace(field_buffer(fields[0], 0));
            }
            else
            {
                wclear(popUp);
                box(popUp, 0, 0);
                mvwprintw(popUp, 1, 1, "Invalid Input.");
                mvwprintw(popUp, 2, 1, "Press any key to continue.");
                curs_set(0);
                wrefresh(popUp);
                unpost_form(form);
                free_form(form);
                free_field(fields[1]);
                getch();
                return NULL;
            }
            break;
        case KEY_END:
            ch = '\n';
            break;
        default:
            form_driver(form, ch);
            break;
        }
    }
}
void editInfo(int userId)
{
    int maxY, maxX, boxY, boxX, size, ch;
    user *tempUser = NULL;
    bool select = false;
    bool working = true;
    tempUser = retrieveUserById(userId);
    char **choices;
    char *regularChoices[] = {"First Name", "Last Name", "National Code", "Email", "Password", "Blood Type", "Age", "Weight", "Height"};
    char *businessOwnerChoices[] = {"First Name", "Last Name", "National Code", "Email", "Password", "Store Name", "Store Street", "Store City"};
    if (tempUser->businessOwner)
    {
        size = 8;
        choices = businessOwnerChoices;
    }
    else
    {
        size = 9;
        choices = regularChoices;
    }
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    noecho();
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - (size + 5)) / 2;
    boxX = (maxX - 50) / 2;
    WINDOW *box = newwin(size + 5, 50, boxY, boxX);
    WINDOW *innerBox = derwin(box, size + 2, 48, 2, 1);
    WINDOW *shadow = newwin(size + 5, 50, boxY + 1, boxX + 2);
    wattron(stdscr, A_BOLD);
    mvwprintw(stdscr, boxY + size + 6, boxX, "Prees End Key to Exit.");
    refresh();
    mvwprintw(box, 1, 1, "Edit Info");
    keypad(innerBox, true);
    int choice, highlight = 0;
    box(box, 0, 0);
    box(innerBox, 0, 0);
    wbkgd(box, COLOR_PAIR(2));
    wbkgd(innerBox, COLOR_PAIR(2));
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    wrefresh(box);
    wrefresh(innerBox);
    curs_set(0);
    while (working)
    {
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(innerBox, A_REVERSE);
            }
            mvwprintw(innerBox, i + 1, 1, choices[i]);
            wattroff(innerBox, A_REVERSE);
        }
        choice = wgetch(innerBox);
        switch (choice)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            break;
        case KEY_DOWN:
            if (highlight < size - 1)
            {
                highlight++;
            }
            break;
        case '\n':
            select = true;
            break;
        case KEY_END:
            working = false;
            break;
        default:
            break;
        }
        wrefresh(innerBox);
        if (select)
        {
            if (size == 9)
            {
                switch (highlight)
                {
                case 0:
                    editUser(userId, "firstName", editInfoPopup("Please enter your new first name:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 1:
                    editUser(userId, "lastName", editInfoPopup("Please enter your new last name:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 2:
                    editUser(userId, "nationalCode", editInfoPopup("Please enter your new national code:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 3:
                    editUser(userId, "email", editInfoPopup("Please enter your new email:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 4:
                    editUser(userId, "password", editInfoPopupPassword(boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 5:
                    editRegular(userId, "bloodType", editInfoPopup("Please enter your new blood type:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 6:
                    editUser(userId, "age", editInfoPopup("Please enter your new age:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 7:
                    editUser(userId, "weight", editInfoPopup("Please enter your new weight:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 8:
                    editUser(userId, "height", editInfoPopup("Please enter your new height:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch (highlight)
                {
                case 0:
                    editUser(userId, "firstName", editInfoPopup("Please enter your new first name:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 1:
                    editUser(userId, "lastName", editInfoPopup("Please enter your new last name:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 2:
                    editUser(userId, "nationalCode", editInfoPopup("Please enter your new national code:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 3:
                    editUser(userId, "email", editInfoPopup("Please enter your new email:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 4:
                    editUser(userId, "password", editInfoPopupPassword(boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 5:
                    editbusinessOwner(userId, "storeName", editInfoPopup("Please enter your new store name:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 6:
                    editbusinessOwner(userId, "storeStreet", editInfoPopup("Please enter your store street:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                case 7:
                    editbusinessOwner(userId, "storeCity", editInfoPopup("Please enter your store city:", boxX, boxY));
                    box(box, 0, 0);
                    box(innerBox, 0, 0);
                    wrefresh(box);
                    wrefresh(innerBox);
                    break;
                default:
                    break;
                }
            }
        }
        select = false;
    }
    endwin();
    if (size == 9)
    {
        userMenu(userId);
    }
    else
    {
        shopMenu(userId);
    }
}
void createAccount(void)
{
    int ch, maxY, maxX, boxY, boxX;
    WINDOW *windows[3];
    FIELD *fields[8];
    FORM *form;
    user tempUser;
    int id;
    char tempPassword[50];
    char tempRepeatPassword[50];
    bool location = false;
    bool allFilled = true;
    bool complete = false;
    initscr();
    clear();
    noecho();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 19) / 2;
    boxX = (maxX - 52) / 2;
    wbkgd(stdscr, COLOR_PAIR(1));
    refresh();
    curs_set(0);
    WINDOW *infoPopup = newwin(3, 52, boxY + 3, boxX);
    while (true)
    {
        wclear(infoPopup);
        wbkgd(infoPopup, COLOR_PAIR(2));
        box(infoPopup, 0, 0);
        mvwprintw(infoPopup, 1, 1, "Press R for Regular User and L for Location.");
        wrefresh(infoPopup);
        ch = getch();
        if (ch == 'l')
        {
            location = true;
            break;
        }
        else if (ch == 'r')
        {
            break;
        }
        else
        {
            mvwprintw(infoPopup, 1, 1, "Invalid Input, Press any key to continue.    ");
            wrefresh(infoPopup);
            getch();
        }
    }
    delwin(infoPopup);
    curs_set(1);
    windows[0] = newwin(19, 52, boxY, boxX);
    windows[1] = newwin(19, 52, boxY + 1, boxX + 2);
    windows[2] = derwin(windows[0], 16, 50, 2, 1);
    wbkgd(windows[1], COLOR_PAIR(3));
    wrefresh(windows[1]);
    wattron(windows[0], A_BOLD);
    mvwprintw(windows[0], 1, 1, "Create Account:");
    wattroff(windows[0], A_BOLD);
    wattron(stdscr, A_BOLD);
    mvwprintw(stdscr, boxY + 20, boxX, "Prees End Key To Go Back");
    wattroff(stdscr, A_BOLD);
    wrefresh(stdscr);
    box(windows[0], 0, 0);
    wbkgd(windows[0], COLOR_PAIR(2));
    wrefresh(windows[0]);
    wbkgd(windows[2], COLOR_PAIR(2));
    wrefresh(windows[2]);
    fields[0] = new_field(1, 46, 1, 0, 0, 0);
    fields[1] = new_field(1, 46, 3, 0, 0, 0);
    fields[2] = new_field(1, 46, 5, 0, 0, 0);
    fields[3] = new_field(1, 46, 7, 0, 0, 0);
    fields[4] = new_field(1, 46, 9, 0, 0, 0);
    fields[5] = new_field(1, 46, 11, 0, 0, 0);
    fields[6] = new_field(1, 46, 13, 0, 0, 0);
    fields[7] = NULL;
    for (int i = 0; i < 7; i++)
    {
        if (i != 1 && i != 2)
        {
            set_field_back(fields[i], A_UNDERLINE | COLOR_PAIR(2));
            set_field_fore(fields[i], COLOR_PAIR(2));
            field_opts_on(fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
        }
        else
        {
            set_field_back(fields[i], A_UNDERLINE | COLOR_PAIR(2));
            set_field_fore(fields[i], COLOR_PAIR(2));
            field_opts_on(fields[i], O_VISIBLE | O_AUTOSKIP | O_EDIT | O_ACTIVE);
            field_opts_off(fields[i], O_PUBLIC);
        }
    }
    form = new_form(fields);
    set_form_win(form, windows[2]);
    set_form_sub(form, derwin(windows[2], 14, 48, 1, 1));
    post_form(form);
    mvwprintw(windows[2], 1, 1, "Username:");
    mvwprintw(windows[2], 3, 1, "Password:");
    mvwprintw(windows[2], 5, 1, "Password Confirmation:");
    mvwprintw(windows[2], 7, 1, "First Name:");
    mvwprintw(windows[2], 9, 1, "Last Name:");
    mvwprintw(windows[2], 11, 1, "National Code:");
    mvwprintw(windows[2], 13, 1, "Email:");
    wmove(windows[2], 2, 1);
    box(windows[2], 0, 0);
    wrefresh(windows[2]);
    keypad(stdscr, TRUE);
    while (ch != '\n')
    {
        ch = getch();
        switch (ch)
        {
        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case '\n':
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_NEXT_FIELD);
            stpcpy(tempPassword, trimWhitespace(field_buffer(fields[1], 0)));
            stpcpy(tempRepeatPassword, trimWhitespace(field_buffer(fields[2], 0)));
            for (int i = 0; i < 7; i++)
            {
                if (strlen(trimWhitespace(field_buffer(fields[i], 0))) == 0)
                {
                    allFilled = false;
                }
            }
            if (!strcmp(tempPassword, tempRepeatPassword) && retrieveUser(trimWhitespace(field_buffer(fields[0], 0))) == NULL && allFilled)
            {
                tempUser.alive = true;
                tempUser.businessOwner = location;
                strcpy(tempUser.username, trimWhitespace(field_buffer(fields[0], 0)));
                strcpy(tempUser.password, trimWhitespace(field_buffer(fields[1], 0)));
                strcpy(tempUser.firstName, trimWhitespace(field_buffer(fields[3], 0)));
                strcpy(tempUser.lastName, trimWhitespace(field_buffer(fields[4], 0)));
                strcpy(tempUser.nationalCode, trimWhitespace(field_buffer(fields[5], 0)));
                strcpy(tempUser.email, trimWhitespace(field_buffer(fields[6], 0)));
                id = addUser(&tempUser);
            }
            else
            {
                infoPopup = newwin(4, 52, boxY + 3, boxX);
                wclear(infoPopup);
                wbkgd(infoPopup, COLOR_PAIR(2));
                box(infoPopup, 0, 0);
                mvwprintw(infoPopup, 1, 1, "Invalid Input");
                mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
                wrefresh(infoPopup);
                ch = getch();
                for (int i = 0; i < 7; i++)
                {
                    set_field_buffer(fields[i], 0, "");
                }
                form_driver(form, REQ_FIRST_FIELD);
                ch = 0;
            }
            box(windows[2], 0, 0);
            box(windows[0], 0, 0);
            wrefresh(windows[0]);
            wrefresh(windows[2]);
            break;
        case KEY_END:
            ch = '\n';
            allFilled = false;
        default:
            form_driver(form, ch);
            break;
        }
        wrefresh(windows[2]);
    }
    unpost_form(form);
    free_form(form);
    for (int i = 0; i < 7; i++)
    {
        free_field(fields[i]);
        fields[i] = NULL;
    }
    if (!location && allFilled)
    {
        regular tempRegular;
        wclear(windows[2]);
        wbkgd(windows[2], COLOR_PAIR(2));
        fields[0] = new_field(1, 46, 1, 0, 0, 0);
        fields[1] = new_field(1, 46, 3, 0, 0, 0);
        fields[2] = new_field(1, 46, 5, 0, 0, 0);
        fields[3] = new_field(1, 46, 7, 0, 0, 0);
        for (int i = 0; i < 4; i++)
        {
            set_field_back(fields[i], A_UNDERLINE | COLOR_PAIR(2));
            set_field_fore(fields[i], COLOR_PAIR(2));
            field_opts_on(fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
        }
        form = new_form(fields);
        set_form_win(form, windows[2]);
        set_form_sub(form, derwin(windows[2], 14, 48, 1, 1));
        post_form(form);
        mvwprintw(windows[2], 1, 1, "Blood Type:");
        mvwprintw(windows[2], 3, 1, "Age:");
        mvwprintw(windows[2], 5, 1, "Height:");
        mvwprintw(windows[2], 7, 1, "Weight:");
        wmove(windows[2], 2, 1);
        box(windows[2], 0, 0);
        wrefresh(windows[2]);
        keypad(stdscr, TRUE);
        ch = 0;
        while (ch != '\n')
        {
            ch = getch();
            WINDOW *infoPopup = newwin(5, 52, boxY + 3, boxX);
            switch (ch)
            {
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_UP:
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_LEFT:
                form_driver(form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_NEXT_CHAR);
                break;
            case KEY_BACKSPACE:
            case 127:
                form_driver(form, REQ_DEL_PREV);
                break;
            case '\n':
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_NEXT_FIELD);
                tempRegular.id = id;
                tempUser.businessOwner = location;
                strcpy(tempRegular.bloodType, trimWhitespace(field_buffer(fields[0], 0)));
                strcpy(tempRegular.age, trimWhitespace(field_buffer(fields[1], 0)));
                strcpy(tempRegular.height, trimWhitespace(field_buffer(fields[2], 0)));
                strcpy(tempRegular.weight, trimWhitespace(field_buffer(fields[3], 0)));
                addRegular(id, &tempRegular);
                complete = true;
            case KEY_END:
                ch = '\n';
            default:
                form_driver(form, ch);
                break;
            }
            wrefresh(windows[2]);
        }
        unpost_form(form);
        free_form(form);
        for (int i = 0; i < 5; i++)
        {
            free_field(fields[i]);
            fields[i] = NULL;
        }
    }
    else if (allFilled)
    {
        businessOwner tempOwner;
        wclear(windows[2]);
        wbkgd(windows[2], COLOR_PAIR(2));
        fields[0] = new_field(1, 46, 1, 0, 0, 0);
        fields[1] = new_field(1, 46, 3, 0, 0, 0);
        fields[2] = new_field(1, 46, 5, 0, 0, 0);
        for (int i = 0; i < 3; i++)
        {
            set_field_back(fields[i], A_UNDERLINE | COLOR_PAIR(2));
            set_field_fore(fields[i], COLOR_PAIR(2));
            field_opts_on(fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
        }
        form = new_form(fields);
        set_form_win(form, windows[2]);
        set_form_sub(form, derwin(windows[2], 14, 48, 1, 1));
        post_form(form);
        mvwprintw(windows[2], 1, 1, "Location name:");
        mvwprintw(windows[2], 3, 1, "Location Street:");
        mvwprintw(windows[2], 5, 1, "Location City:");
        wmove(windows[2], 2, 1);
        box(windows[2], 0, 0);
        wrefresh(windows[2]);
        keypad(stdscr, TRUE);
        ch = 0;
        while (ch != '\n')
        {
            ch = getch();
            WINDOW *infoPopup = newwin(5, 52, boxY + 3, boxX);
            switch (ch)
            {
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_UP:
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_LEFT:
                form_driver(form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_NEXT_CHAR);
                break;
            case KEY_BACKSPACE:
            case 127:
                form_driver(form, REQ_DEL_PREV);
                break;
            case '\n':
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_NEXT_FIELD);
                for (int i = 0; i < 3; i++)
                {
                    if (strlen(trimWhitespace(field_buffer(fields[i], 0))) == 0)
                    {
                        allFilled = false;
                    }
                }
                if (allFilled)
                {
                    strcpy(tempOwner.storeName, trimWhitespace(field_buffer(fields[0], 0)));
                    strcpy(tempOwner.storeStreet, trimWhitespace(field_buffer(fields[1], 0)));
                    strcpy(tempOwner.storeCity, trimWhitespace(field_buffer(fields[2], 0)));
                    addbusinessOwner(id, &tempOwner);
                    complete = true;
                }
                else
                {
                    infoPopup = newwin(4, 52, boxY + 3, boxX);
                    wclear(infoPopup);
                    wbkgd(infoPopup, COLOR_PAIR(2));
                    box(infoPopup, 0, 0);
                    mvwprintw(infoPopup, 1, 1, "Invalid Input");
                    mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
                    wrefresh(infoPopup);
                    ch = getch();
                    for (int i = 0; i < 7; i++)
                    {
                        set_field_buffer(fields[i], 0, "");
                    }
                    form_driver(form, REQ_FIRST_FIELD);
                    ch = 0;
                }
                box(windows[2], 0, 0);
                box(windows[0], 0, 0);
                wrefresh(windows[0]);
                wrefresh(windows[2]);
                break;
            case KEY_END:
                ch = '\n';
            default:
                form_driver(form, ch);
                break;
            }
            wrefresh(windows[2]);
        }
        unpost_form(form);
        free_form(form);
        for (int i = 0; i < 4; i++)
        {
            free_field(fields[i]);
            fields[i] = NULL;
        }
    }
    if (complete)
    {
        curs_set(0);
        infoPopup = newwin(4, 52, boxY + 3, boxX);
        wclear(infoPopup);
        wbkgd(infoPopup, COLOR_PAIR(2));
        box(infoPopup, 0, 0);
        mvwprintw(infoPopup, 1, 1, "Account Created Succesfully.");
        mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
        wrefresh(infoPopup);
        getch();
        startMenu();
    }
    else
    {
        curs_set(0);
        infoPopup = newwin(4, 52, boxY + 3, boxX);
        wclear(infoPopup);
        wbkgd(infoPopup, COLOR_PAIR(2));
        box(infoPopup, 0, 0);
        mvwprintw(infoPopup, 1, 1, "Account Creation Unsusscessful.");
        mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
        wrefresh(infoPopup);
        getch();
        startMenu();
    }
    endwin();
}
void userMenu(int userId)
{
    int maxY, maxX, boxY, boxX, tempHealthCode;
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    init_pair(4, 88, 240);
    init_pair(5, 208, 240);
    init_pair(6, 220, 240);
    init_pair(7, 34, 240);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 12) / 2;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    user *temp = retrieveUserById(userId);
    mvprintw(boxY - 3, boxX, "Hello %s", temp->firstName);
    switch (temp->healthCondition)
    {
    case 1:
        attron(COLOR_PAIR(7));
        mvprintw(boxY - 2, boxX, "You are healthy");
        attroff(COLOR_PAIR(7));
        break;
    case 2:
        attron(COLOR_PAIR(6));
        mvprintw(boxY - 2, boxX, "You are suspicous");
        attroff(COLOR_PAIR(6));
        break;
    case 3:
        attron(COLOR_PAIR(5));
        mvprintw(boxY - 2, boxX, "You are dangerous");
        attroff(COLOR_PAIR(5));
        break;
    case 4:
        attron(COLOR_PAIR(4));
        mvprintw(boxY - 2, boxX, "You are ill");
        attroff(COLOR_PAIR(4));
        break;
    default:
        break;
    }
    mvprintw(boxY - 1, boxX, "There are currently %d Covid-19 Patients", totalCases);
    refresh();
    WINDOW *box = newwin(12, 50, boxY, boxX);
    WINDOW *shadow = newwin(12, 50, boxY + 1, boxX + 2);
    keypad(box, true);
    char *choices[] = {"Check my health", "Edit Info", "Add person visit", "Add location visit", "Code Changes History", "Person Visits History", "Location Visits History", "Hospitals Nearby", "Send Message To Admin", "Logout", "Exit"};
    int choice, highlight = 0;
    box(box, 0, 0);
    wbkgd(box, COLOR_PAIR(2));
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    while (true)
    {
        for (int i = 0; i < 10; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, i + 1, 1, choices[i]);
            wattroff(box, A_REVERSE);
        }
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            break;
        case KEY_DOWN:
            if (highlight < 9)
            {
                highlight++;
            }
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            break;
        }
    }
    wrefresh(box);
    switch (highlight)
    {
    case 0:
        if (temp->healthCondition == 4)
        {
            errorPopup("You can't test.", boxX, boxY);
            box(box, 0, 0);
            wrefresh(box);
        }
        else
        {
            tempHealthCode = checkHealth();
            if (tempHealthCode > temp->healthCondition)
            {
                codeChangeRecursive(userId, tempHealthCode, week);
            }
            else
            {
                userCodeChangeById(userId, temp->healthCondition);
            }
        }
        userMenu(userId);
        break;
    case 1:
        editInfo(userId);
        userMenu(userId);
        break;
    case 2:
        addRegularVisit(userId);
        userMenu(userId);
        break;
    case 3:
        addBussinessVisit(userId);
        userMenu(userId);
        break;
    case 4:
        showCodeChangesHistory(userId);
        userMenu(userId);
        break;
    case 5:
        showRegularMeetsHistory(userId);
        userMenu(userId);
        break;
    case 6:
        showBussinessMeetsHistory(userId);
        userMenu(userId);
        break;
    case 7:
        showHospitalsForUser(boxX, boxY);
        userMenu(userId);
        break;
    case 8:
        messageMenu(userId);
        userMenu(userId);
        break;
    case 9:
        clear();
        endwin();
        startMenu();
        break;
    case 10:
        clear();
        endwin();
        exit(0);
        break;
    default:
        break;
    }
    endwin();
}
void shopMenu(int userId)
{
    int maxY, maxX, boxY, boxX;
    user *currentUser = retrieveUserById(userId);
    businessOwner *owner = retrieveBusinessOwner(userId);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    init_pair(4, 88, 240);
    init_pair(5, 34, 240);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 7) / 2;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 3, boxX, "Hello %s", currentUser->firstName);
    mvprintw(boxY - 2, boxX, "%s", owner->storeName);
    switch (currentUser->healthCondition)
    {
    case 1:
        attron(COLOR_PAIR(5));
        mvprintw(boxY - 1, boxX, "Your health status is Green");
        attroff(COLOR_PAIR(5));
        break;
    case 2:
        attron(COLOR_PAIR(4));
        mvprintw(boxY - 1, boxX, "Your health status is Red");
        attroff(COLOR_PAIR(4));
        break;
    default:
        break;
    }
    refresh();
    WINDOW *box = newwin(7, 50, boxY, boxX);
    WINDOW *shadow = newwin(7, 50, boxY + 1, boxX + 2);
    keypad(box, true);
    char *choices[] = {"Clear My Shop", "Edit Info", "My Visitors", "Logout", "Exit"};
    int choice, highlight = 0;
    box(box, 0, 0);
    wbkgd(box, COLOR_PAIR(2));
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    while (true)
    {
        for (int i = 0; i < 5; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, i + 1, 1, choices[i]);
            wattroff(box, A_REVERSE);
        }
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            break;
        case KEY_DOWN:
            if (highlight < 4)
            {
                highlight++;
            }
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            break;
        }
    }
    wrefresh(box);
    switch (highlight)
    {
    case 0:
        if (currentUser->healthCondition == 2)
        {
            currentUser->healthCondition = 1;
            userCodeChangeById(userId, 1);
            errorPopup("Your Shop is Cleared.", boxX, boxY);
        }
        shopMenu(userId);
        break;
    case 1:
        editInfo(userId);
        shopMenu(userId);
        break;
    case 2:
        showVisitors(userId);
        shopMenu(userId);
        break;
    case 3:
        clear();
        endwin();
        startMenu();
        break;
    case 4:
        clear();
        endwin();
        exit(0);
        break;
    default:
        break;
    }
    endwin();
}
void adminMenu(void)
{
    int maxY, maxX, boxY, boxX;
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 8) / 2;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 2, boxX, "Hello Admin");
    mvprintw(boxY - 1, boxX, "There are currently %d Covid-19 Patients", totalCases);
    refresh();
    WINDOW *box = newwin(8, 50, boxY, boxX);
    WINDOW *shadow = newwin(8, 50, boxY + 1, boxX + 2);
    keypad(box, true);
    char *choices[] = {"See All Users", "See All Shops", "Add Hospital", "See All Hospitals", "See Messages", "Logout", "Exit"};
    int choice, highlight = 0;
    box(box, 0, 0);
    wbkgd(box, COLOR_PAIR(2));
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    while (true)
    {
        for (int i = 0; i < 6; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, i + 1, 1, choices[i]);
            wattroff(box, A_REVERSE);
        }
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            break;
        case KEY_DOWN:
            if (highlight < 5)
            {
                highlight++;
            }
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            break;
        }
    }
    wrefresh(box);
    switch (highlight)
    {
    case 0:
        allUsers();
        break;
    case 1:
        allShops();
        break;
    case 2:
        addHospitalMenu();
        adminMenu();
        break;
    case 3:
        showHospitalsForAdmin();
        adminMenu();
        break;
    case 4:
        messageList();
        adminMenu();
    case 5:
        clear();
        endwin();
        startMenu();
        break;
    case 6:
        clear();
        endwin();
        exit(0);
        break;
    default:
        break;
    }
    endwin();
}
void forgetPassword(void)
{
    int ch, maxY, maxX, boxY, boxX;
    user *tempUser;
    bool allFilled = true, complete = false;
    initscr();
    clear();
    noecho();
    start_color();
    curs_set(1);
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 15) / 2;
    boxX = (maxX - 52) / 2;
    wbkgd(stdscr, COLOR_PAIR(1));
    refresh();
    WINDOW *box, *shadow, *insideBox, *infoPopup;
    box = newwin(15, 52, boxY, boxX);
    shadow = newwin(15, 52, boxY + 1, boxX + 2);
    insideBox = derwin(box, 12, 50, 2, 1);
    wbkgd(box, COLOR_PAIR(2));
    wbkgd(shadow, COLOR_PAIR(3));
    wbkgd(insideBox, COLOR_PAIR(2));
    box(box, 0, 0);
    box(insideBox, 0, 0);
    wattron(box, A_BOLD);
    mvwprintw(box, 1, 2, "Forget Password");
    wattroff(box, A_BOLD);
    wattron(stdscr, A_BOLD);
    mvwprintw(stdscr, boxY + 16, boxX, "Press End Key to exit.");
    wattroff(stdscr, A_BOLD);
    wrefresh(stdscr);
    wrefresh(shadow);
    wrefresh(box);
    wrefresh(insideBox);
    FIELD *fields[6];
    FORM *form;
    fields[0] = new_field(1, 46, 1, 0, 0, 0);
    fields[1] = new_field(1, 46, 3, 0, 0, 0);
    fields[2] = new_field(1, 46, 5, 0, 0, 0);
    fields[3] = new_field(1, 46, 7, 0, 0, 0);
    fields[4] = new_field(1, 46, 9, 0, 0, 0);
    fields[5] = NULL;
    for (int i = 0; i < 5; i++)
    {
        if (i != 3 && i != 4)
        {
            set_field_back(fields[i], A_UNDERLINE | COLOR_PAIR(2));
            set_field_fore(fields[i], COLOR_PAIR(2));
            field_opts_on(fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
        }
        else
        {
            set_field_back(fields[i], A_UNDERLINE | COLOR_PAIR(2));
            set_field_fore(fields[i], COLOR_PAIR(2));
            field_opts_on(fields[i], O_VISIBLE | O_AUTOSKIP | O_EDIT | O_ACTIVE);
            field_opts_off(fields[i], O_PUBLIC);
        }
    }
    form = new_form(fields);
    set_form_win(form, insideBox);
    set_form_sub(form, derwin(insideBox, 10, 48, 1, 1));
    post_form(form);
    mvwprintw(insideBox, 1, 1, "Username:");
    mvwprintw(insideBox, 3, 1, "Email:");
    mvwprintw(insideBox, 5, 1, "National Code:");
    mvwprintw(insideBox, 7, 1, "New Password:");
    mvwprintw(insideBox, 9, 1, "New Password Confirmation:");
    wmove(insideBox, 2, 1);
    wrefresh(insideBox);
    keypad(stdscr, TRUE);
    while (ch != '\n')
    {
        ch = getch();
        switch (ch)
        {
        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case '\n':
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_NEXT_FIELD);
            tempUser = retrieveUser(trimWhitespace(field_buffer(fields[0], 0)));
            for (int i = 0; i < 5; i++)
            {
                if (strlen(trimWhitespace(field_buffer(fields[i], 0))) == 0)
                {
                    allFilled = false;
                }
            }
            if (allFilled && tempUser != NULL && strcmp(tempUser->email, trimWhitespace(field_buffer(fields[1], 0))) == 0 && strcmp(tempUser->nationalCode, trimWhitespace(field_buffer(fields[2], 0))) == 0 && strcmp(trimWhitespace(field_buffer(fields[3], 0)), trimWhitespace(field_buffer(fields[4], 0))) == 0)
            {
                editUser(tempUser->id, "password", trimWhitespace(field_buffer(fields[3], 0)));
                complete = true;
            }
            else
            {
                infoPopup = newwin(4, 52, boxY + 3, boxX);
                wclear(infoPopup);
                wbkgd(infoPopup, COLOR_PAIR(2));
                box(infoPopup, 0, 0);
                mvwprintw(infoPopup, 1, 1, "Invalid Input");
                mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
                wrefresh(infoPopup);
                ch = getch();
                for (int i = 0; i < 7; i++)
                {
                    set_field_buffer(fields[i], 0, "");
                }
                form_driver(form, REQ_FIRST_FIELD);
                ch = 0;
            }
            box(box, 0, 0);
            box(insideBox, 0, 0);
            wrefresh(box);
            wrefresh(insideBox);
            delwin(infoPopup);
            break;
        case KEY_END:
            ch = '\n';
        default:
            form_driver(form, ch);
            break;
        }
        wrefresh(insideBox);
    }
    unpost_form(form);
    free_form(form);
    for (int i = 0; i < 7; i++)
    {
        free_field(fields[i]);
        fields[i] = NULL;
    }
    if (complete)
    {
        curs_set(0);
        infoPopup = newwin(4, 52, boxY + 3, boxX);
        wclear(infoPopup);
        wbkgd(infoPopup, COLOR_PAIR(2));
        box(infoPopup, 0, 0);
        mvwprintw(infoPopup, 1, 1, "Password Changed Successfully");
        mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
        wrefresh(infoPopup);
        getch();
        startMenu();
    }
    else
    {
        curs_set(0);
        infoPopup = newwin(4, 52, boxY + 3, boxX);
        wclear(infoPopup);
        wbkgd(infoPopup, COLOR_PAIR(2));
        box(infoPopup, 0, 0);
        mvwprintw(infoPopup, 1, 1, "Nothing Changed");
        mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
        wrefresh(infoPopup);
        getch();
        startMenu();
    }
}
void login(void)
{
    int ch, maxY, maxX, boxY, boxX;
    WINDOW *windows[4];
    FIELD *fields[3];
    FORM *form;
    bool usernameCheck = false;
    bool passwordCheck = false;
    bool admin = false;
    bool continueLoop = true;
    char tempUsername[50];
    char tempPassword[50];
    user *temp;
    initscr();
    clear();
    curs_set(1);
    noecho();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 11) / 2;
    boxX = (maxX - 52) / 2;
    wbkgd(stdscr, COLOR_PAIR(1));
    wattron(stdscr, A_BOLD);
    mvwprintw(stdscr, boxY + 12, boxX, "Prees end key to exit.");
    wattroff(stdscr, A_BOLD);
    refresh();
    windows[0] = newwin(11, 52, boxY, boxX);
    windows[1] = newwin(11, 52, boxY + 1, boxX + 2);
    windows[2] = derwin(windows[0], 8, 50, 2, 1);
    windows[3] = derwin(windows[2], 6, 48, 1, 1);
    wbkgd(windows[1], COLOR_PAIR(3));
    wrefresh(windows[1]);
    wattron(windows[0], A_BOLD);
    mvwprintw(windows[0], 1, 1, "Login:");
    wattroff(windows[0], A_BOLD);
    wbkgd(windows[2], COLOR_PAIR(2));
    wbkgd(windows[3], COLOR_PAIR(2));
    wbkgd(windows[0], COLOR_PAIR(2));
    box(windows[0], 0, 0);
    fields[0] = new_field(1, 46, 2, 1, 0, 0);
    fields[1] = new_field(1, 46, 4, 1, 0, 0);
    fields[2] = NULL;
    set_field_back(fields[0], A_UNDERLINE | COLOR_PAIR(2));
    set_field_back(fields[1], A_UNDERLINE | COLOR_PAIR(2));
    set_field_fore(fields[0], COLOR_PAIR(2));
    set_field_fore(fields[1], COLOR_PAIR(2));
    set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
    set_field_opts(fields[1], O_VISIBLE | O_EDIT | O_ACTIVE);
    form = new_form(fields);
    set_form_win(form, windows[2]);
    set_form_sub(form, windows[3]);
    post_form(form);
    mvwprintw(windows[3], 1, 1, "Username:");
    mvwprintw(windows[3], 3, 1, "Password:");
    wmove(windows[2], 2, 1);
    box(windows[2], 0, 0);
    wrefresh(windows[2]);
    wrefresh(windows[0]);
    keypad(stdscr, TRUE);
    while (ch != '\n' && continueLoop)
    {
        ch = getch();
        WINDOW *infoPopup = newwin(5, 52, boxY + 3, boxX);
        switch (ch)
        {
        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case '\n':
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_NEXT_FIELD);
            stpcpy(tempUsername, trimWhitespace(field_buffer(fields[0], 0)));
            stpcpy(tempPassword, trimWhitespace(field_buffer(fields[1], 0)));
            if (strlen(tempUsername) == 0 || strlen(tempPassword) == 0)
            {
                curs_set(0);
                keypad(infoPopup, true);
                char *choices[2] = {"Yes", "No"};
                int highlight = 0, choice;
                wbkgd(infoPopup, COLOR_PAIR(2));
                box(infoPopup, 0, 0);
                mvwprintw(infoPopup, 1, 1, "Invalid Input");
                mvwprintw(infoPopup, 2, 1, "Try Again?");
                wrefresh(infoPopup);
                while (1)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (i == highlight)
                        {
                            wattron(infoPopup, A_REVERSE);
                        }
                        mvwprintw(infoPopup, 3, 21 + 5 * i, "%s", choices[i]);
                        wattroff(infoPopup, A_REVERSE);
                    }
                    wrefresh(infoPopup);
                    choice = wgetch(infoPopup);
                    switch (choice)
                    {
                    case KEY_RIGHT:
                    case KEY_LEFT:
                        highlight = !highlight;
                        break;
                    default:
                        break;
                    }
                    if (choice == '\n')
                    {
                        switch (highlight)
                        {
                        case 0:
                            ch = 0;
                            set_field_buffer(fields[0], 0, "");
                            set_field_buffer(fields[1], 0, "");
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                }
                delwin(infoPopup);
                curs_set(1);
                box(windows[0], 0, 0);
                box(windows[2], 0, 0);
                wrefresh(windows[0]);
                wrefresh(windows[2]);
                wrefresh(windows[3]);
            }
            else
            {
                temp = retrieveUser(tempUsername);
                if (temp != NULL)
                {
                    usernameCheck = true;
                    if (!strcmp(tempPassword, temp->password))
                    {
                        passwordCheck = true;
                    }
                    else
                    {
                        curs_set(0);
                        keypad(infoPopup, true);
                        char *choices[2] = {"Yes", "No"};
                        int highlight = 0, choice;
                        wbkgd(infoPopup, COLOR_PAIR(2));
                        box(infoPopup, 0, 0);
                        mvwprintw(infoPopup, 1, 1, "Wrong Password");
                        mvwprintw(infoPopup, 2, 1, "Try Again?");
                        wrefresh(infoPopup);
                        while (1)
                        {
                            for (int i = 0; i < 2; i++)
                            {
                                if (i == highlight)
                                {
                                    wattron(infoPopup, A_REVERSE);
                                }
                                mvwprintw(infoPopup, 3, 21 + 5 * i, "%s", choices[i]);
                                wattroff(infoPopup, A_REVERSE);
                            }
                            wrefresh(infoPopup);
                            choice = wgetch(infoPopup);
                            switch (choice)
                            {
                            case KEY_RIGHT:
                            case KEY_LEFT:
                                highlight = !highlight;
                                break;
                            default:
                                break;
                            }
                            if (choice == '\n')
                            {
                                switch (highlight)
                                {
                                case 0:
                                    ch = 0;
                                    set_field_buffer(fields[0], 0, "");
                                    set_field_buffer(fields[1], 0, "");
                                    break;
                                default:
                                    break;
                                }
                                break;
                            }
                        }
                        delwin(infoPopup);
                        curs_set(1);
                        box(windows[0], 0, 0);
                        box(windows[2], 0, 0);
                        wrefresh(windows[0]);
                        wrefresh(windows[2]);
                        wrefresh(windows[3]);
                    }
                }
                else if (strcmp(tempUsername, "Admin") == 0)
                {
                    if (validate(tempPassword))
                    {
                        admin = true;
                    }
                    else
                    {
                        curs_set(0);
                        keypad(infoPopup, true);
                        char *choices[2] = {"Yes", "No"};
                        int highlight = 0, choice;
                        wbkgd(infoPopup, COLOR_PAIR(2));
                        box(infoPopup, 0, 0);
                        mvwprintw(infoPopup, 1, 1, "Wrong Password");
                        mvwprintw(infoPopup, 2, 1, "Try Again?");
                        wrefresh(infoPopup);
                        while (1)
                        {
                            for (int i = 0; i < 2; i++)
                            {
                                if (i == highlight)
                                {
                                    wattron(infoPopup, A_REVERSE);
                                }
                                mvwprintw(infoPopup, 3, 21 + 5 * i, "%s", choices[i]);
                                wattroff(infoPopup, A_REVERSE);
                            }
                            wrefresh(infoPopup);
                            choice = wgetch(infoPopup);
                            switch (choice)
                            {
                            case KEY_RIGHT:
                            case KEY_LEFT:
                                highlight = !highlight;
                                break;
                            default:
                                break;
                            }
                            if (choice == '\n')
                            {
                                switch (highlight)
                                {
                                case 0:
                                    ch = 0;
                                    set_field_buffer(fields[0], 0, "");
                                    set_field_buffer(fields[1], 0, "");
                                    break;
                                default:
                                    break;
                                }
                                break;
                            }
                        }
                        delwin(infoPopup);
                        curs_set(1);
                        box(windows[0], 0, 0);
                        box(windows[2], 0, 0);
                        wrefresh(windows[0]);
                        wrefresh(windows[2]);
                        wrefresh(windows[3]);
                    }
                }
                else
                {
                    curs_set(0);
                    keypad(infoPopup, true);
                    char *choices[2] = {"Yes", "No"};
                    int highlight = 0, choice;
                    wbkgd(infoPopup, COLOR_PAIR(2));
                    box(infoPopup, 0, 0);
                    mvwprintw(infoPopup, 1, 1, "Wrond Username");
                    mvwprintw(infoPopup, 2, 1, "Try Again?");
                    wrefresh(infoPopup);
                    while (1)
                    {
                        for (int i = 0; i < 2; i++)
                        {
                            if (i == highlight)
                            {
                                wattron(infoPopup, A_REVERSE);
                            }
                            mvwprintw(infoPopup, 3, 21 + 5 * i, "%s", choices[i]);
                            wattroff(infoPopup, A_REVERSE);
                        }
                        wrefresh(infoPopup);
                        choice = wgetch(infoPopup);
                        switch (choice)
                        {
                        case KEY_RIGHT:
                        case KEY_LEFT:
                            highlight = !highlight;
                            break;
                        default:
                            break;
                        }
                        if (choice == '\n')
                        {
                            switch (highlight)
                            {
                            case 0:
                                ch = 0;
                                set_field_buffer(fields[0], 0, "");
                                set_field_buffer(fields[1], 0, "");
                                break;
                            default:
                                break;
                            }
                            break;
                        }
                    }
                    delwin(infoPopup);
                    curs_set(1);
                    box(windows[0], 0, 0);
                    box(windows[2], 0, 0);
                    wrefresh(windows[0]);
                    wrefresh(windows[2]);
                    wrefresh(windows[3]);
                }
            }
            break;
        case KEY_END:
            continueLoop = false;
            break;
        default:
            form_driver(form, ch);
            break;
        }
        wrefresh(windows[2]);
    }
    endwin();
    if (usernameCheck && passwordCheck)
    {
        if (temp->businessOwner)
        {
            weeklyCodeChange();
            shopMenu(temp->id);
            endwin();
        }
        else
        {
            if (temp->alive)
            {
                weeklyCodeChange();
                userMenu(temp->id);
            }
            else
            {
                errorPopup("A dead man tells no tales.", boxX, boxY);
                startMenu();
            }
            endwin();
        }
    }
    else if (admin)
    {
        adminMenu();
    }
    else
    {
        errorPopup("Login Failed", boxX, boxY);
        startMenu();
    }
}
void allShops(void)
{
    int maxY, maxX, boxY, boxX, usersCount = 0;
    user **users = getbusinessOwnerList(&usersCount);
    businessOwner *tempOwner;
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(8, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(usersCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d shops:", usersCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(8));
    wrefresh(shadow);
    init_pair(3, 88, 12);
    init_pair(6, 34, 12);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "ID");
        mvwprintw(box, 1, 5, "Name");
        mvwprintw(box, 1, 15, "Status");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            if (users[i + j]->healthCondition == 2)
            {
                tempOwner = retrieveBusinessOwner(users[i + j]->id);
                wattron(box, COLOR_PAIR(3));
                mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                mvwprintw(box, i + 2, 5, "%s", tempOwner->storeName);
                mvwprintw(box, i + 2, 15, "Red");
                wattroff(box, COLOR_PAIR(3));
            }
            else if (users[i + j]->healthCondition == 1)
            {
                tempOwner = retrieveBusinessOwner(users[i + j]->id);
                wattron(box, COLOR_PAIR(6));
                mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                mvwprintw(box, i + 2, 5, "%s", tempOwner->storeName);
                mvwprintw(box, i + 2, 15, "Green");
                wattroff(box, COLOR_PAIR(6));
            }
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < usersCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < usersCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;
        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
    adminMenu();
}
void allUsers(void)
{
    int maxY, maxX, boxY, boxX, usersCount = 0;
    user **users = getRegularList(&usersCount);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(8, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(usersCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d users:", usersCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(8));
    wrefresh(shadow);
    init_pair(3, 88, 12);
    init_pair(4, 208, 12);
    init_pair(5, 220, 12);
    init_pair(6, 34, 12);
    init_pair(7, 232, 12);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        usersCount = 0;
        users = getRegularList(&usersCount);
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "ID");
        mvwprintw(box, 1, 5, "Name");
        mvwprintw(box, 1, 15, "Status");
        mvwprintw(box, 1, 26, "Last Code Change");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            if (users[i + j]->alive)
            {
                if (users[i + j]->healthCondition == 4)
                {
                    wattron(box, COLOR_PAIR(3));
                    mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                    mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                    mvwprintw(box, i + 2, 15, "Ill");
                    mvwprintw(box, i + 2, 26, "%s", asctime(localtime(&users[i + j]->lastCodeChange)));
                    wattroff(box, COLOR_PAIR(3));
                }
                else if (users[i + j]->healthCondition == 3)
                {
                    wattron(box, COLOR_PAIR(4));
                    mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                    mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                    mvwprintw(box, i + 2, 15, "Dangerous");
                    mvwprintw(box, i + 2, 26, "%s", asctime(localtime(&users[i + j]->lastCodeChange)));

                    wattroff(box, COLOR_PAIR(4));
                }
                else if (users[i + j]->healthCondition == 2)
                {
                    wattron(box, COLOR_PAIR(5));
                    mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                    mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                    mvwprintw(box, i + 2, 15, "Suspicuos");
                    mvwprintw(box, i + 2, 26, "%s", asctime(localtime(&users[i + j]->lastCodeChange)));

                    wattroff(box, COLOR_PAIR(5));
                }
                else if (users[i + j]->healthCondition == 1)
                {
                    wattron(box, COLOR_PAIR(6));
                    mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                    mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                    mvwprintw(box, i + 2, 15, "Healthy");
                    mvwprintw(box, i + 2, 26, "%s", asctime(localtime(&users[i + j]->lastCodeChange)));

                    wattroff(box, COLOR_PAIR(6));
                }
            }
            else
            {
                wattron(box, COLOR_PAIR(7));
                mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                mvwprintw(box, i + 2, 15, "Dead");
                wattroff(box, COLOR_PAIR(7));
            }
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        WINDOW *infoPopup = newwin(4, 50, boxY + ((size - 1) / 2), boxX);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < usersCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < usersCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;
        case '\n':
            if (users[highlight + j]->alive)
            {
                box(infoPopup, 0, 0);
                wbkgd(infoPopup, COLOR_PAIR(2));
                mvwprintw(infoPopup, 1, 1, "Please enter user's new health condition");
                mvwprintw(infoPopup, 2, 1, "H for Heald or D for Dead");
                wrefresh(infoPopup);
                option = getchar();
                switch (option)
                {
                case 'h':
                case 'H':
                    if (users[highlight + j]->healthCondition == 4)
                    {
                        userCodeChangeById(users[highlight + j]->id, 2);
                    }
                    break;
                case 'd':
                case 'D':
                    editUser(users[highlight + j]->id, "alive", "0");
                    break;
                default:
                    break;
                }
                delwin(infoPopup);
                refresh();
                wrefresh(box);
                break;
            }
        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
    adminMenu();
}
int min(int a, int b)
{
    return a <= b ? a : b;
}
char *strlwr(char *string)
{
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] >= 'A' && string[i] <= 'Z')
        {
            string[i] += 32;
        }
    }
    return string;
}
char *trimWhitespace(char *string)
{
    char *end;
    while (isspace(*string))
    {
        string++;
    }
    if (*string == 0)
    {
        return string;
    }
    end = string + strnlen(string, 128) - 1;
    while (end > string && isspace(*end))
    {
        end--;
    }
    *(end + 1) = '\0';
    return string;
}
char *itoa(long int num)
{
    char *numString = malloc(50 * sizeof(char));
    sprintf(numString, "%ld", num);
    return numString;
}
user *retrieveUser(const char *username)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    file = fopen("user.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlFindElement(xml, xml, NULL, "username", username, MXML_DESCEND);
    if (node != NULL)
    {
        user *tempUser = (user *)malloc(sizeof(user));
        mxml_node_t *child = mxmlGetFirstChild(node);
        tempUser->id = atoi(mxmlGetElement(node));
        strcpy(tempUser->username, username);
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->password, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        tempUser->healthCondition = atoi(mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->firstName, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->lastName, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->nationalCode, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->email, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        tempUser->businessOwner = atoi(mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        tempUser->alive = atoi(mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        tempUser->lastCodeChange = atoi(mxmlGetOpaque(child));
        return tempUser;
    }
    else
    {
        return NULL;
    }
}
user *retrieveUserById(int userId)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    file = fopen("user.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlFindElement(xml, xml, itoa(userId), NULL, NULL, MXML_DESCEND);
    if (node != NULL)
    {
        user *tempUser = (user *)malloc(sizeof(user));
        mxml_node_t *child = mxmlGetFirstChild(node);
        tempUser->id = atoi(mxmlGetElement(node));
        strcpy(tempUser->username, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->password, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        tempUser->healthCondition = atoi(mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->firstName, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->lastName, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->nationalCode, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempUser->email, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        tempUser->businessOwner = atoi(mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        tempUser->alive = atoi(mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        tempUser->lastCodeChange = atoi(mxmlGetOpaque(child));
        return tempUser;
    }
    else
    {
        return NULL;
    }
}
regular *retrieveRegular(int id)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    regular *tempRegular = (regular *)malloc(sizeof(regular));
    file = fopen("regular.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlFindElement(xml, xml, itoa(id), NULL, NULL, MXML_DESCEND);
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        mxml_node_t *child = mxmlGetFirstChild(node);
        tempRegular->id = id;
        child = mxmlGetNextSibling(child);
        strcpy(tempRegular->bloodType, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempRegular->age, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempRegular->height, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempRegular->weight, mxmlGetOpaque(child));
        return tempRegular;
    }
}
businessOwner *retrieveBusinessOwner(int id)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    businessOwner *tempbusinessOwner = (businessOwner *)malloc(sizeof(businessOwner));
    file = fopen("businessOwner.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlFindElement(xml, xml, itoa(id), NULL, NULL, MXML_DESCEND);
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        mxml_node_t *child = mxmlGetFirstChild(node);
        tempbusinessOwner->id = id;
        strcpy(tempbusinessOwner->storeName, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempbusinessOwner->storeStreet, mxmlGetOpaque(child));
        child = mxmlGetNextSibling(child);
        strcpy(tempbusinessOwner->storeCity, mxmlGetOpaque(child));
        return tempbusinessOwner;
    }
}
int addUser(user *userAdd)
{
    FILE *file;
    FILE *userCounter;
    mxml_node_t *xml = NULL;
    mxml_node_t *node = NULL;
    mxml_node_t *data = NULL;
    int id;
    file = fopen("user.xml", "r");
    userCounter = fopen("userCount.txt", "r");
    fscanf(userCounter, "%d", &id);
    id++;
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    //xml = mxmlNewXML("1.0");
    node = mxmlNewElement(xml, itoa(id));
    mxmlElementSetAttr(node, "username", userAdd->username);
    data = mxmlNewElement(node, "username");
    mxmlNewText(data, 0, userAdd->username);
    data = mxmlNewElement(node, "password");
    mxmlNewText(data, 0, userAdd->password);
    data = mxmlNewElement(node, "healthCondition");
    mxmlNewText(data, 0, "1");
    data = mxmlNewElement(node, "firstName");
    mxmlNewText(data, 0, userAdd->firstName);
    data = mxmlNewElement(node, "lastName");
    mxmlNewText(data, 0, userAdd->lastName);
    data = mxmlNewElement(node, "nationalCode");
    mxmlNewText(data, 0, userAdd->nationalCode);
    data = mxmlNewElement(node, "email");
    mxmlNewText(data, 0, userAdd->email);
    data = mxmlNewElement(node, "businessOwner");
    mxmlNewText(data, 0, itoa(userAdd->businessOwner));
    data = mxmlNewElement(node, "alive");
    mxmlNewText(data, 0, "1");
    data = mxmlNewElement(node, "lastCodeChange");
    mxmlNewText(data, 0, itoa(time(NULL)));
    fclose(file);
    fclose(userCounter);
    file = fopen("user.xml", "w");
    userCounter = fopen("userCount.txt", "w");
    fprintf(userCounter, "%d", id);
    fclose(userCounter);
    mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
    fclose(file);
    return id;
}
void addbusinessOwner(int id, businessOwner *owner)
{
    FILE *file;
    mxml_node_t *xml = NULL;
    mxml_node_t *node = NULL;
    mxml_node_t *data = NULL;
    file = fopen("businessOwner.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    //xml = mxmlNewXML("1.0");
    node = mxmlNewElement(xml, itoa(id));
    data = mxmlNewElement(node, "storeName");
    mxmlNewText(data, 0, owner->storeName);
    data = mxmlNewElement(node, "storeStreet");
    mxmlNewText(data, 0, owner->storeStreet);
    data = mxmlNewElement(node, "storeCity");
    mxmlNewText(data, 0, owner->storeCity);
    fclose(file);
    file = fopen("businessOwner.xml", "w");
    mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
    fclose(file);
}
void addRegular(int id, regular *regularUser)
{
    FILE *file;
    mxml_node_t *xml = NULL;
    mxml_node_t *node = NULL;
    mxml_node_t *data = NULL;
    file = fopen("regular.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    //xml = mxmlNewXML("1.0");
    node = mxmlNewElement(xml, itoa(id));
    data = mxmlNewElement(node, "bloodType");
    mxmlNewText(data, 0, regularUser->bloodType);
    data = mxmlNewElement(node, "age");
    mxmlNewText(data, 0, regularUser->age);
    data = mxmlNewElement(node, "height");
    mxmlNewText(data, 0, regularUser->height);
    data = mxmlNewElement(node, "weight");
    mxmlNewText(data, 0, regularUser->weight);
    fclose(file);
    file = fopen("regular.xml", "w");
    mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
    fclose(file);
}
int editUser(int userId, const char *edit, const char *value)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    file = fopen("user.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlFindElement(xml, xml, itoa(userId), NULL, NULL, MXML_DESCEND);
    if (node != NULL)
    {
        mxml_node_t *child = mxmlGetFirstChild(node);
        while (child != NULL && strcmp(mxmlGetElement(child), edit))
        {
            child = mxmlGetNextSibling(child);
        }
        if (child == NULL)
        {
            return -1;
        }
        else
        {
            mxmlSetOpaque(child, value);
            fclose(file);
            file = fopen("user.xml", "w");
            mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
            fclose(file);
            return 1;
        }
    }
    else
    {
        return -1;
    }
}
int editbusinessOwner(int id, const char *edit, const char *value)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    file = fopen("businessOwner.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlFindElement(xml, xml, itoa(id), NULL, NULL, MXML_DESCEND);
    if (node != NULL)
    {
        mxml_node_t *child = mxmlGetFirstChild(node);
        while (child != NULL && strcmp(mxmlGetElement(child), edit))
        {
            child = mxmlGetNextSibling(child);
        }
        if (child == NULL)
        {
            return -1;
        }
        else
        {
            mxmlSetOpaque(child, value);
            fclose(file);
            file = fopen("businessOwner.xml", "w");
            mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
            fclose(file);
            return 1;
        }
    }
    else
    {
        return -1;
    }
}
int editRegular(int id, const char *edit, const char *value)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    file = fopen("regular.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlFindElement(xml, xml, itoa(id), NULL, NULL, MXML_DESCEND);
    if (node != NULL)
    {
        mxml_node_t *child = mxmlGetFirstChild(node);
        while (child != NULL && strcmp(mxmlGetElement(child), edit))
        {
            child = mxmlGetNextSibling(child);
        }
        if (child == NULL)
        {
            return -1;
        }
        else
        {
            mxmlSetOpaque(child, value);
            fclose(file);
            file = fopen("regular.xml", "w");
            mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
            fclose(file);
            return 1;
        }
    }
    else
    {
        return -1;
    }
}
codeChange *codeChangeRetrieve(int userId, int *numberOfItems)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    mxml_node_t *data;
    file = fopen("codeChange.xml", "r");
    codeChange *codeChanges = NULL;
    int count = 0;
    //xml = mxmlNewXML("1.0");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlGetFirstChild(xml);
    while (node != NULL)
    {
        if (atoi(mxmlElementGetAttr(node, "id")) == userId)
        {
            codeChanges = (codeChange *)realloc(codeChanges, (++count) * sizeof(codeChange));
            data = mxmlGetFirstChild(node);
            codeChanges[count - 1].from = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            codeChanges[count - 1].to = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            codeChanges[count - 1].date = atoi(mxmlGetOpaque(data));
            data = NULL;
        }
        node = mxmlGetNextSibling(node);
    }
    *numberOfItems = count;
    return codeChanges;
}
void regularVisitEdit(int regularId1, int regularId2)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    mxml_node_t *data;
    file = fopen("regularVisit.xml", "r");
    //xml = mxmlNewXML("1.0");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    if (xml != NULL)
    {
        node = mxmlNewElement(xml, "visit");
        data = mxmlNewElement(node, "p1");
        mxmlNewText(data, 0, itoa(regularId1));
        data = mxmlNewElement(node, "p2");
        mxmlNewText(data, 0, itoa(regularId2));
        data = mxmlNewElement(node, "p1Code");
        mxmlNewText(data, 0, itoa(getCodeById(regularId1)));
        data = mxmlNewElement(node, "p2Code");
        mxmlNewText(data, 0, itoa(getCodeById(regularId2)));
        data = mxmlNewElement(node, "date");
        mxmlNewText(data, 0, itoa(time(NULL)));
        fclose(file);
        file = fopen("regularVisit.xml", "w");
        mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
        fclose(file);
    }
}
regularVisit *regularVisitRetrieve(int userId, int *numberOfItems)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    mxml_node_t *p1;
    mxml_node_t *p2;
    file = fopen("regularVisit.xml", "r");
    regularVisit *visits = NULL;
    int count = 0;
    //xml = mxmlNewXML("1.0");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlGetFirstChild(xml);
    while (node != NULL)
    {
        p1 = mxmlGetFirstChild(node);
        p2 = mxmlGetNextSibling(p1);
        if (atoi(mxmlGetOpaque(p1)) == userId || atoi(mxmlGetOpaque(p2)) == userId)
        {
            if (atoi(mxmlGetOpaque(p1)) == userId)
            {
                visits = (regularVisit *)realloc(visits, (++count) * sizeof(regularVisit));
                visits[count - 1].p1 = atoi(mxmlGetOpaque(p1));
                visits[count - 1].p2 = atoi(mxmlGetOpaque(p2));
                p2 = mxmlGetNextSibling(p2);
                visits[count - 1].p1Code = atoi(mxmlGetOpaque(p2));
                p2 = mxmlGetNextSibling(p2);
                visits[count - 1].p2Code = atoi(mxmlGetOpaque(p2));
                p2 = mxmlGetNextSibling(p2);
                visits[count - 1].date = atoi(mxmlGetOpaque(p2));
            }
            else
            {
                visits = (regularVisit *)realloc(visits, (++count) * sizeof(regularVisit));
                visits[count - 1].p1 = atoi(mxmlGetOpaque(p2));
                visits[count - 1].p2 = atoi(mxmlGetOpaque(p1));
                p2 = mxmlGetNextSibling(p2);
                visits[count - 1].p2Code = atoi(mxmlGetOpaque(p2));
                p2 = mxmlGetNextSibling(p2);
                visits[count - 1].p1Code = atoi(mxmlGetOpaque(p2));
                p2 = mxmlGetNextSibling(p2);
                visits[count - 1].date = atoi(mxmlGetOpaque(p2));
            }
        }
        p1 = NULL;
        p2 = NULL;
        node = mxmlGetNextSibling(node);
    }
    *numberOfItems = count;
    return visits;
}
void businessVisitEdit(int regularId, int businessId)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    mxml_node_t *data;
    file = fopen("businessVisit.xml", "r");
    //xml = mxmlNewXML("1.0");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    if (xml != NULL)
    {
        node = mxmlNewElement(xml, "visit");
        data = mxmlNewElement(node, "regular");
        mxmlNewText(data, 0, itoa(regularId));
        data = mxmlNewElement(node, "business");
        mxmlNewText(data, 0, itoa(businessId));
        data = mxmlNewElement(node, "regularCode");
        mxmlNewText(data, 0, itoa(getCodeById(regularId)));
        data = mxmlNewElement(node, "businessCode");
        mxmlNewText(data, 0, itoa(getCodeById(businessId)));
        data = mxmlNewElement(node, "date");
        mxmlNewText(data, 0, itoa(time(NULL)));
        fclose(file);
        file = fopen("businessVisit.xml", "w");
        mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
        fclose(file);
    }
}
businessVisit *businessVisitRetrieveForUser(int userId, int *numberOfItems)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    mxml_node_t *data;
    file = fopen("businessVisit.xml", "r");
    businessVisit *visits = NULL;
    int count = 0;
    //xml = mxmlNewXML("1.0");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlGetFirstChild(xml);
    while (node != NULL)
    {
        data = mxmlGetFirstChild(node);
        if (atoi(mxmlGetOpaque(data)) == userId)
        {
            visits = (businessVisit *)realloc(visits, (++count) * sizeof(businessVisit));
            visits[count - 1].regularId = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            visits[count - 1].businessId = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            visits[count - 1].regularCode = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            visits[count - 1].businessCode = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            visits[count - 1].date = atoi(mxmlGetOpaque(data));
        }
        data = NULL;
        node = mxmlGetNextSibling(node);
    }
    *numberOfItems = count;
    return visits;
}
businessVisit *businessVisitRetrieveForbusiness(int businessId, int *numberOfItems)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    mxml_node_t *data;
    file = fopen("businessVisit.xml", "r");
    businessVisit *visits = NULL;
    int count = 0;
    //xml = mxmlNewXML("1.0");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlGetFirstChild(xml);
    while (node != NULL)
    {
        data = mxmlGetFirstChild(node);
        if (atoi(mxmlGetOpaque(mxmlGetNextSibling(data))) == businessId)
        {
            visits = (businessVisit *)realloc(visits, (++count) * sizeof(businessVisit));
            visits[count - 1].regularId = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            visits[count - 1].businessId = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            visits[count - 1].regularCode = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            visits[count - 1].businessCode = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            visits[count - 1].date = atoi(mxmlGetOpaque(data));
        }
        data = NULL;
        node = mxmlGetNextSibling(node);
    }
    *numberOfItems = count;
    return visits;
}
int getCodeById(int userId)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    mxml_node_t *child = NULL;
    file = fopen("user.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlFindElement(xml, xml, itoa(userId), NULL, NULL, MXML_DESCEND);
    child = mxmlGetFirstChild(node);
    if (child != NULL)
    {
        for (int i = 0; i < 2; i++)
        {
            child = mxmlGetNextSibling(child);
        }
        return atoi(mxmlGetOpaque(child));
    }
    else
    {
        return -1;
    }
}
void userCodeChangeById(int userId, int change)
{
    int from = getCodeById(userId);
    editUser(userId, "healthCondition", itoa(change));
    editUser(userId, "lastCodeChange", itoa(time(NULL)));
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node = NULL;
    mxml_node_t *data;
    file = fopen("codeChange.xml", "r");
    //xml = mxmlNewXML("1.0");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    if (xml != NULL)
    {
        node = mxmlNewElement(xml, "codeChange");
        mxmlElementSetAttr(node, "id", itoa(userId));
        data = mxmlNewElement(node, "from");
        mxmlNewText(data, 0, itoa(from));
        data = mxmlNewElement(node, "to");
        mxmlNewText(data, 0, itoa(change));
        data = mxmlNewElement(node, "date");
        mxmlNewText(data, 0, itoa(time(NULL)));
        fclose(file);
        file = fopen("codeChange.xml", "w");
        mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
        fclose(file);
    }
}
user **getRegularList(int *numberOfItems)
{
    user **users = NULL;
    user *tempuser;
    FILE *file;
    file = fopen("userCount.txt", "r");
    int userCount;
    fscanf(file, "%d", &userCount);
    for (int i = 1; i <= userCount; i++)
    {
        tempuser = retrieveUserById(i);
        if (tempuser->businessOwner == 0)
        {
            users = (user **)realloc(users, ++(*numberOfItems) * sizeof(user *));
            users[*numberOfItems - 1] = tempuser;
        }
    }
    return users;
}
user **getbusinessOwnerList(int *numberOfItems)
{
    user **users = NULL;
    user *tempuser;
    FILE *file;
    file = fopen("userCount.txt", "r");
    int userCount;
    fscanf(file, "%d", &userCount);
    for (int i = 1; i <= userCount; i++)
    {
        tempuser = retrieveUserById(i);
        if (tempuser->businessOwner == 1)
        {
            users = (user **)realloc(users, ++(*numberOfItems) * sizeof(user *));
            users[*numberOfItems - 1] = tempuser;
        }
    }
    return users;
}
int getUserCount(void)
{
    FILE *file;
    int count;
    file = fopen("userCount.txt", "r");
    fscanf(file, "%d", &count);
    fclose(file);
    return count;
}
void weeklyCodeChange(void)
{
    int count = getUserCount();
    time_t now = time(NULL);
    user *tempUser;
    for (int i = 1; i <= count; i++)
    {
        tempUser = retrieveUserById(i);
        if (tempUser->businessOwner == 0)
        {
            if ((tempUser->healthCondition == 3 || tempUser->healthCondition == 2) && (now - tempUser->lastCodeChange) > week)
            {
                userCodeChangeById(i, (tempUser->healthCondition - 1));
            }
        }
        else
        {
            if (tempUser->healthCondition == 2 && (now - tempUser->lastCodeChange) > 2 * week)
            {
                userCodeChangeById(i, 1);
            }
        }
    }
}
void codeChangeRecursive(int user, int to, int timeSpan)
{
    userCodeChangeById(user, to);
    if (to == 3 || to == 4)
    {
        time_t now = time(NULL);
        int num = 0;
        regularVisit *visits = regularVisitRetrieve(user, &num);
        for (int i = 0; i < num; i++)
        {
            if (getCodeById(visits[i].p2) < to - 1 && now - visits[i].date < timeSpan)
            {
                codeChangeRecursive(visits[i].p2, to - 1, now - visits[i].date);
            }
        }
    }
    if (to == 4)
    {
        time_t now = time(NULL);
        int num = 0;
        businessVisit *businessVisits = businessVisitRetrieveForUser(user, &num);
        for (int i = 0; i < num; i++)
        {
            if (getCodeById(businessVisits[i].businessId) == 1 && now - businessVisits[i].date < timeSpan)
            {
                userCodeChangeById(businessVisits[i].businessId, 2);
            }
        }
    }
}
int checkHealth(void)
{
    int ch, maxY, maxX, boxY, boxX, status = 0;
    initscr();
    clear();
    noecho();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    curs_set(0);
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 4) / 2;
    boxX = (maxX - 52) / 2;
    wbkgd(stdscr, COLOR_PAIR(1));
    refresh();
    WINDOW *box = newwin(4, 52, boxY, boxX);
    WINDOW *shadow = newwin(4, 52, boxY + 1, boxX + 2);
    wbkgd(shadow, COLOR_PAIR(3));
    wbkgd(box, COLOR_PAIR(2));
    box(box, 0, 0);
    wrefresh(shadow);
    wrefresh(box);
    keypad(box, true);
    mvwprintw(box, 1, 1, "Do you have dry coughs?");
    char *choices[2] = {"Yes", "No"};
    int highlight = 0, choice;
    while (1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, 2, 21 + 5 * i, "%s", choices[i]);
            wattroff(box, A_REVERSE);
        }
        wrefresh(box);
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_RIGHT:
        case KEY_LEFT:
            highlight = !highlight;
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            if (highlight == 0)
            {
                status += 30;
            }
            break;
        }
    }
    mvwprintw(box, 1, 1, "Is your body temprature above 37 degrees?");
    highlight = 0;
    while (1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, 2, 21 + 5 * i, "%s", choices[i]);
            wattroff(box, A_REVERSE);
        }
        wrefresh(box);
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_RIGHT:
        case KEY_LEFT:
            highlight = !highlight;
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            if (highlight == 0)
            {
                status += 20;
            }
            break;
        }
    }
    mvwprintw(box, 1, 1, "Do you have any shortness of breath?            ");
    highlight = 0;
    while (1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, 2, 21 + 5 * i, "%s", choices[i]);
            wattroff(box, A_REVERSE);
        }
        wrefresh(box);
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_RIGHT:
        case KEY_LEFT:
            highlight = !highlight;
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            if (highlight == 0)
            {
                status += 20;
            }
            break;
        }
    }
    mvwprintw(box, 1, 1, "Do you have any body aches?           ");
    highlight = 0;
    while (1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, 2, 21 + 5 * i, "%s", choices[i]);
            wattroff(box, A_REVERSE);
        }
        wrefresh(box);
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_RIGHT:
        case KEY_LEFT:
            highlight = !highlight;
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            if (highlight == 0)
            {
                status += 15;
            }
            break;
        }
    }
    mvwprintw(box, 1, 1, "Does your throat hurts?          ");
    highlight = 0;
    while (1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_REVERSE);
            }
            mvwprintw(box, 2, 21 + 5 * i, "%s", choices[i]);
            wattroff(box, A_REVERSE);
        }
        wrefresh(box);
        choice = wgetch(box);
        switch (choice)
        {
        case KEY_RIGHT:
        case KEY_LEFT:
            highlight = !highlight;
            break;
        default:
            break;
        }
        if (choice == '\n')
        {
            if (highlight == 0)
            {
                status += 15;
            }
            break;
        }
    }
    if (status <= 15)
    {
        status = 1;
    }
    else if (status <= 50)
    {
        status = 2;
    }
    else if (status <= 75)
    {
        status = 3;
    }
    else
    {
        status = 4;
    }
    wclear(box);
    box(box, 0, 0);
    switch (status)
    {
    case 1:
        mvwprintw(box, 1, 1, "You are healthy.");
        break;
    case 2:
        mvwprintw(box, 1, 1, "You are suspicous.");
        break;
    case 3:
        mvwprintw(box, 1, 1, "You are dangerous.");
        break;
    case 4:
        mvwprintw(box, 1, 1, "You are ill.");
        break;
    default:
        break;
    }
    wrefresh(box);
    getch();
    return status;
}
int getTotalCases(void)
{
    int num, cases = 0;
    user **users = getRegularList(&num);
    for (int i = 0; i < num; i++)
    {
        if (users[i]->healthCondition == 4)
        {
            cases++;
        }
    }
    return cases;
}
hospital *getHospitalsListForUser(int *numberOfItems, int locationId)
{
    hospital *hospitals = NULL;
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node;
    mxml_node_t *data;
    file = fopen("hospitals.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlGetFirstChild(xml);
    while (node != NULL)
    {
        data = mxmlGetFirstChild(node);
        if (atoi(mxmlGetOpaque(data)) == locationId)
        {
            hospitals = (hospital *)realloc(hospitals, ++(*numberOfItems) * sizeof(hospital));
            hospitals[*numberOfItems - 1].hospitalLocationId = atoi(mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            strcpy(hospitals[*numberOfItems - 1].hospitalName, mxmlGetOpaque(data));
            data = mxmlGetNextSibling(data);
            strcpy(hospitals[*numberOfItems - 1].hospitalCity, mxmlGetOpaque(data));
        }
        data = NULL;
        node = mxmlGetNextSibling(node);
    }
    return hospitals;
}
hospital *getHospitalsListForAdmin(int *numberOfItems)
{
    hospital *hospitals = NULL;
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node;
    mxml_node_t *data;
    file = fopen("hospitals.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlGetFirstChild(xml);
    while (node != NULL)
    {
        hospitals = (hospital *)realloc(hospitals, ++(*numberOfItems) * sizeof(hospital));
        data = mxmlGetFirstChild(node);
        hospitals[*numberOfItems - 1].hospitalLocationId = atoi(mxmlGetOpaque(data));
        data = mxmlGetNextSibling(data);
        strcpy(hospitals[*numberOfItems - 1].hospitalName, mxmlGetOpaque(data));
        data = mxmlGetNextSibling(data);
        strcpy(hospitals[*numberOfItems - 1].hospitalCity, mxmlGetOpaque(data));
        data = NULL;
        node = mxmlGetNextSibling(node);
    }
    return hospitals;
}
void addHospital(hospital *add)
{
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node;
    mxml_node_t *data;
    file = fopen("hospitals.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlNewElement(xml, "hospital");
    data = mxmlNewElement(node, "hospitalId");
    mxmlNewText(data, 0, itoa(add->hospitalLocationId));
    data = mxmlNewElement(node, "hospitalName");
    mxmlNewText(data, 0, add->hospitalName);
    data = mxmlNewElement(node, "hospitalCity");
    mxmlNewText(data, 0, add->hospitalCity);
    fclose(file);
    file = fopen("hospitals.xml", "w");
    mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
    fclose(file);
}
void showHospitalsForAdmin(void)
{
    int maxY, maxX, boxY, boxX, usersCount = 0;
    hospital *hospitals = getHospitalsListForAdmin(&usersCount);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(usersCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d hospitals:", usersCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "Location ID");
        mvwprintw(box, 1, 15, "Name");
        mvwprintw(box, 1, 35, "City Name");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            wattron(box, COLOR_PAIR(2));
            mvwprintw(box, i + 2, 1, "%d", hospitals[i + j].hospitalLocationId);
            mvwprintw(box, i + 2, 15, "%s", hospitals[i + j].hospitalName);
            mvwprintw(box, i + 2, 35, "%s", hospitals[i + j].hospitalCity);
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < usersCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < usersCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;
        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
    adminMenu();
}
int getLocationID(int x, int y)
{
    int ch;
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    curs_set(1);
    noecho();
    WINDOW *popUp;
    FIELD *fields[2];
    FORM *form;
    for (int i = 0; i < 2; i++)
    {
        fields[i] = NULL;
    }
    popUp = newwin(4, 50, y + 3, x);
    wclear(popUp);
    wbkgd(popUp, COLOR_PAIR(2));
    box(popUp, 0, 0);
    mvwprintw(popUp, 1, 1, "Please enter your area code:");
    fields[0] = new_field(1, 46, 0, 0, 0, 0);
    set_field_back(fields[0], A_UNDERLINE | COLOR_PAIR(2));
    set_field_fore(fields[0], COLOR_PAIR(2));
    field_opts_on(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
    form = new_form(fields);
    set_form_win(form, popUp);
    set_form_sub(form, derwin(popUp, 1, 48, 2, 1));
    post_form(form);
    wrefresh(popUp);
    keypad(popUp, true);
    while (ch != '\n')
    {
        ch = wgetch(popUp);
        switch (ch)
        {
        case KEY_DOWN:
        case KEY_UP:
            break;
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case '\n':
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_NEXT_FIELD);
            if (atoi(trimWhitespace(field_buffer(fields[0], 0))) != 0)
            {
                unpost_form(form);
                free_form(form);
                return atoi(trimWhitespace(field_buffer(fields[0], 0)));
            }
            else
            {
                wclear(popUp);
                box(popUp, 0, 0);
                mvwprintw(popUp, 1, 1, "Invalid Input.");
                mvwprintw(popUp, 2, 1, "Press any key to continue.");
                curs_set(0);
                wrefresh(popUp);
                getch();
                unpost_form(form);
                free_form(form);
                free_field(fields[0]);
                return 0;
            }
            break;
        case KEY_END:
            ch = '\n';
            break;
        default:
            form_driver(form, ch);
            break;
        }
    }
}
void showHospitalsForUser(int x, int y)
{
    int maxY, maxX, boxY, boxX, usersCount = 0, locationId = getLocationID(x, y);
    if (locationId != 0)
    {
        hospital *hospitals = getHospitalsListForUser(&usersCount, locationId);
        initscr();
        clear();
        start_color();
        init_pair(1, 0, 240);
        init_pair(2, 0, 12);
        init_pair(3, 17, 17);
        wbkgd(stdscr, COLOR_PAIR(1));
        getmaxyx(stdscr, maxY, maxX);
        int size = min(usersCount, 15);
        boxY = (maxY - size) / 2 - 3;
        boxX = (maxX - 50) / 2;
        attron(A_BOLD);
        mvprintw(boxY - 1, boxX, "There are currently %d hospitals:", usersCount);
        attroff(A_BOLD);
        refresh();
        WINDOW *box = newwin(size + 3, 50, boxY, boxX);
        WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
        box(box, 0, 0);
        wbkgd(shadow, COLOR_PAIR(3));
        wrefresh(shadow);
        curs_set(0);
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
        attroff(COLOR_PAIR(1) | A_BOLD);
        refresh();
        keypad(box, TRUE);
        int j = 0, choice, highlight = 0;
        noecho();
        while (1)
        {
            wclear(box);
            wbkgd(box, COLOR_PAIR(2));
            wattron(box, A_BOLD);
            mvwprintw(box, 1, 1, "Location ID");
            mvwprintw(box, 1, 15, "Name");
            mvwprintw(box, 1, 35, "City Name");
            wattroff(box, A_BOLD);
            for (int i = 0; i < size; i++)
            {
                if (i == highlight)
                {
                    wattron(box, A_STANDOUT);
                }
                wattron(box, COLOR_PAIR(2));
                mvwprintw(box, i + 2, 1, "%d", hospitals[i + j].hospitalLocationId);
                mvwprintw(box, i + 2, 15, "%s", hospitals[i + j].hospitalName);
                mvwprintw(box, i + 2, 35, "%s", hospitals[i + j].hospitalCity);
                wattroff(box, A_STANDOUT);
            }
            wrefresh(box);
            choice = wgetch(box);
            char option;
            switch (choice)
            {
            case KEY_DOWN:
                if (highlight < usersCount)
                {
                    highlight++;
                }
                if (highlight == size)
                {
                    highlight = size - 1;
                    if (j + size < usersCount)
                    {
                        j++;
                    }
                }
                break;
            case KEY_UP:
                if (highlight > 0)
                {
                    highlight--;
                }
                if (highlight == 0)
                {
                    if (j > 0)
                    {
                        j--;
                    }
                }
                break;
            default:
                break;
            }
            if (choice == KEY_END)
            {
                break;
            }
        }
        endwin();
    }
}
int validate(char *pin)
{
    CURL *curl;
    CURLcode res;
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *data;
    file = fopen("result.xml", "w");
    curl = curl_easy_init();
    char url[80];
    sprintf(url, "authenticatorapi.com/api.asmx/ValidatePin?pin=%s&secretCode=DANNY1273649974", pin);
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
    fclose(file);
    file = fopen("result.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    data = mxmlGetNextSibling(mxmlGetFirstChild(xml));
    if (strcmp(mxmlGetOpaque(data), "true") == 0)
    {
        fclose(file);
        remove("result.xml");
        return 1;
    }
    else
    {
        fclose(file);
        remove("result.xml");
        return 0;
    }
}
void addHospitalMenu(void)
{
    int ch, maxY, maxX, boxY, boxX;
    bool allFilled = true, complete = false;
    hospital tempHospital;
    initscr();
    clear();
    noecho();
    start_color();
    curs_set(1);
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 11) / 2;
    boxX = (maxX - 52) / 2;
    wbkgd(stdscr, COLOR_PAIR(1));
    refresh();
    WINDOW *box, *shadow, *insideBox, *infoPopup;
    box = newwin(11, 52, boxY, boxX);
    shadow = newwin(11, 52, boxY + 1, boxX + 2);
    insideBox = derwin(box, 8, 50, 2, 1);
    wbkgd(box, COLOR_PAIR(2));
    wbkgd(shadow, COLOR_PAIR(3));
    wbkgd(insideBox, COLOR_PAIR(2));
    box(box, 0, 0);
    box(insideBox, 0, 0);
    wattron(box, A_BOLD);
    mvwprintw(box, 1, 2, "Add Hospital");
    wattroff(box, A_BOLD);
    wattron(stdscr, A_BOLD);
    mvwprintw(stdscr, boxY + 12, boxX, "Press End Key to exit.");
    wattroff(stdscr, A_BOLD);
    wrefresh(stdscr);
    wrefresh(shadow);
    wrefresh(box);
    wrefresh(insideBox);
    FIELD *fields[4];
    FORM *form;
    fields[0] = new_field(1, 46, 1, 0, 0, 0);
    fields[1] = new_field(1, 46, 3, 0, 0, 0);
    fields[2] = new_field(1, 46, 5, 0, 0, 0);
    fields[3] = NULL;
    for (int i = 0; i < 3; i++)
    {
        set_field_back(fields[i], A_UNDERLINE | COLOR_PAIR(2));
        set_field_fore(fields[i], COLOR_PAIR(2));
        field_opts_on(fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
    }
    form = new_form(fields);
    set_form_win(form, insideBox);
    set_form_sub(form, derwin(insideBox, 6, 48, 1, 1));
    post_form(form);
    mvwprintw(insideBox, 1, 1, "Hospital Name:");
    mvwprintw(insideBox, 3, 1, "Hospital City:");
    mvwprintw(insideBox, 5, 1, "Hospital Area Code:");
    wmove(insideBox, 2, 1);
    wrefresh(insideBox);
    keypad(stdscr, TRUE);
    while (ch != '\n')
    {
        ch = getch();
        switch (ch)
        {
        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case '\n':
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_NEXT_FIELD);
            for (int i = 0; i < 3; i++)
            {
                if (strlen(trimWhitespace(field_buffer(fields[i], 0))) == 0)
                {
                    allFilled = false;
                }
            }
            if (atoi(trimWhitespace(field_buffer(fields[2], 0))) == 0)
            {
                allFilled = false;
            }
            if (allFilled)
            {
                strcpy(tempHospital.hospitalName, trimWhitespace(field_buffer(fields[0], 0)));
                strcpy(tempHospital.hospitalCity, trimWhitespace(field_buffer(fields[1], 0)));
                tempHospital.hospitalLocationId = atoi(trimWhitespace(field_buffer(fields[2], 0)));
                complete = true;
                addHospital(&tempHospital);
            }
            else
            {
                infoPopup = newwin(4, 52, boxY + 3, boxX);
                wclear(infoPopup);
                wbkgd(infoPopup, COLOR_PAIR(2));
                box(infoPopup, 0, 0);
                mvwprintw(infoPopup, 1, 1, "Invalid Input");
                mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
                wrefresh(infoPopup);
                ch = getch();
                for (int i = 0; i < 7; i++)
                {
                    set_field_buffer(fields[i], 0, "");
                }
                form_driver(form, REQ_FIRST_FIELD);
                ch = 0;
            }
            box(box, 0, 0);
            box(insideBox, 0, 0);
            wrefresh(box);
            wrefresh(insideBox);
            delwin(infoPopup);
            break;
        case KEY_END:
            ch = '\n';
        default:
            form_driver(form, ch);
            break;
        }
        wrefresh(insideBox);
    }
    unpost_form(form);
    free_form(form);
    for (int i = 0; i < 3; i++)
    {
        free_field(fields[i]);
        fields[i] = NULL;
    }
    if (complete)
    {
        curs_set(0);
        infoPopup = newwin(4, 52, boxY + 3, boxX);
        wclear(infoPopup);
        wbkgd(infoPopup, COLOR_PAIR(2));
        box(infoPopup, 0, 0);
        mvwprintw(infoPopup, 1, 1, "Hospital Added Successfully");
        mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
        wrefresh(infoPopup);
        getch();
    }
    else
    {
        curs_set(0);
        infoPopup = newwin(4, 52, boxY + 3, boxX);
        wclear(infoPopup);
        wbkgd(infoPopup, COLOR_PAIR(2));
        box(infoPopup, 0, 0);
        mvwprintw(infoPopup, 1, 1, "Nothing Changed");
        mvwprintw(infoPopup, 2, 1, "Prees any key to continue.");
        wrefresh(infoPopup);
        getch();
    }
}
void showCodeChangesHistory(int userId)
{
    int maxY, maxX, boxY, boxX, recordCount = 0;
    codeChange *records = codeChangeRetrieve(userId, &recordCount);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(recordCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d records:", recordCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "From");
        mvwprintw(box, 1, 7, "To");
        mvwprintw(box, 1, 15, "Date");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            wattron(box, COLOR_PAIR(2));
            mvwprintw(box, i + 2, 1, "%d", records[i + j].from);
            mvwprintw(box, i + 2, 7, "%d", records[i + j].to);
            mvwprintw(box, i + 2, 15, "%s", asctime(localtime(&records[i + j].date)));
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < recordCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < recordCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;

        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
}
void addRegularVisit(int userId)
{
    int maxY, maxX, boxY, boxX, usersCount = 0;
    user **users = getRegularList(&usersCount);
    user *loggedInUser = retrieveUserById(userId);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(8, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(usersCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d users:", usersCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(8));
    wrefresh(shadow);
    init_pair(3, 88, 12);
    init_pair(4, 208, 12);
    init_pair(5, 220, 12);
    init_pair(6, 34, 12);
    init_pair(7, 232, 12);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        usersCount = 0;
        users = getRegularList(&usersCount);
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "ID");
        mvwprintw(box, 1, 5, "Name");
        mvwprintw(box, 1, 15, "Status");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            if (users[i + j]->id == userId)
            {
                mvwprintw(box, i + 2, 1, "Current User");
            }
            else if (users[i + j]->alive)
            {
                if (users[i + j]->healthCondition == 4)
                {
                    wattron(box, COLOR_PAIR(3));
                    mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                    mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                    mvwprintw(box, i + 2, 15, "Ill");
                    wattroff(box, COLOR_PAIR(3));
                }
                else if (users[i + j]->healthCondition == 3)
                {
                    wattron(box, COLOR_PAIR(4));
                    mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                    mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                    mvwprintw(box, i + 2, 15, "Dangerous");
                    wattroff(box, COLOR_PAIR(4));
                }
                else if (users[i + j]->healthCondition == 2)
                {
                    wattron(box, COLOR_PAIR(5));
                    mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                    mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                    mvwprintw(box, i + 2, 15, "Suspicuos");
                    wattroff(box, COLOR_PAIR(5));
                }
                else if (users[i + j]->healthCondition == 1)
                {
                    wattron(box, COLOR_PAIR(6));
                    mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                    mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                    mvwprintw(box, i + 2, 15, "Healthy");
                    wattroff(box, COLOR_PAIR(6));
                }
            }
            else
            {
                wattron(box, COLOR_PAIR(7));
                mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                mvwprintw(box, i + 2, 5, "%s", users[i + j]->username);
                mvwprintw(box, i + 2, 15, "Dead");
                wattroff(box, COLOR_PAIR(7));
            }
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        WINDOW *infoPopup = newwin(4, 50, boxY + ((size - 1) / 2), boxX);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < usersCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < usersCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;
        case '\n':
            if (users[highlight + j]->alive && users[highlight + j]->id != userId)
            {
                if (users[highlight + j]->healthCondition > 1)
                {
                    box(infoPopup, 0, 0);
                    wbkgd(infoPopup, COLOR_PAIR(2));
                    mvwprintw(infoPopup, 1, 1, "This person is dangerous, are you sure?");
                    mvwprintw(infoPopup, 2, 1, "Press Y for Yes or N for No");
                    wrefresh(infoPopup);
                }
                else
                {
                    box(infoPopup, 0, 0);
                    wbkgd(infoPopup, COLOR_PAIR(2));
                    mvwprintw(infoPopup, 1, 1, "Are you sure?");
                    mvwprintw(infoPopup, 2, 1, "Press Y for Yes or N for No");
                    wrefresh(infoPopup);
                }
                option = getchar();
                switch (option)
                {
                case 'y':
                case 'Y':
                    if (users[highlight + j]->healthCondition - 1 > loggedInUser->healthCondition)
                    {
                        userCodeChangeById(userId, users[highlight + j]->healthCondition - 1);
                    }
                    else if (users[highlight + j]->healthCondition < loggedInUser->healthCondition - 1)
                    {
                        userCodeChangeById(users[highlight + j]->id, loggedInUser->healthCondition - 1);
                    }
                    regularVisitEdit(userId, users[highlight + j]->id);
                    wclear(infoPopup);
                    box(infoPopup, 0, 0);
                    mvwprintw(infoPopup, 1, 1, "Meet added");
                    mvwprintw(infoPopup, 2, 1, "Press any key to continue.");
                    wrefresh(infoPopup);
                    getch();
                    break;
                case 'n':
                case 'N':
                default:
                    break;
                }
                delwin(infoPopup);
                refresh();
                wrefresh(box);
                break;
            }
        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
}
void addBussinessVisit(int userId)
{
    int maxY, maxX, boxY, boxX, usersCount = 0;
    user **users = getbusinessOwnerList(&usersCount);
    businessOwner *tempOwner;
    user *loggedInUser = retrieveUserById(userId);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(8, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(usersCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d shops:", usersCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(8));
    wrefresh(shadow);
    init_pair(3, 88, 12);
    init_pair(6, 34, 12);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "ID");
        mvwprintw(box, 1, 5, "Name");
        mvwprintw(box, 1, 15, "Status");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            if (users[i + j]->healthCondition == 2)
            {
                tempOwner = retrieveBusinessOwner(users[i + j]->id);
                wattron(box, COLOR_PAIR(3));
                mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                mvwprintw(box, i + 2, 5, "%s", tempOwner->storeName);
                mvwprintw(box, i + 2, 15, "Red");
                wattroff(box, COLOR_PAIR(3));
            }
            else if (users[i + j]->healthCondition == 1)
            {
                tempOwner = retrieveBusinessOwner(users[i + j]->id);
                wattron(box, COLOR_PAIR(6));
                mvwprintw(box, i + 2, 1, "%d.", i + j + 1);
                mvwprintw(box, i + 2, 5, "%s", tempOwner->storeName);
                mvwprintw(box, i + 2, 15, "Green");
                wattroff(box, COLOR_PAIR(6));
            }
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        char option;
        WINDOW *infoPopup = newwin(4, 50, boxY + ((size - 1) / 2), boxX);
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < usersCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < usersCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;
        case '\n':
            if (users[highlight + j]->healthCondition == 2)
            {
                box(infoPopup, 0, 0);
                wbkgd(infoPopup, COLOR_PAIR(2));
                mvwprintw(infoPopup, 1, 1, "This place is dangerous, are you sure?");
                mvwprintw(infoPopup, 2, 1, "Press Y for Yes or N for No");
                wrefresh(infoPopup);
            }
            else
            {
                box(infoPopup, 0, 0);
                wbkgd(infoPopup, COLOR_PAIR(2));
                mvwprintw(infoPopup, 1, 1, "Are you sure?");
                mvwprintw(infoPopup, 2, 1, "Press Y for Yes or N for No");
                wrefresh(infoPopup);
            }
            option = getchar();
            switch (option)
            {
            case 'y':
            case 'Y':
                if (users[highlight + j]->healthCondition == 2 && loggedInUser->healthCondition == 1)
                {
                    userCodeChangeById(userId, 2);
                }
                else if (loggedInUser->healthCondition == 4 && users[highlight + j]->healthCondition == 1)
                {
                    userCodeChangeById(users[highlight + j]->id, 2);
                }
                businessVisitEdit(userId, users[highlight + j]->id);
                wclear(infoPopup);
                box(infoPopup, 0, 0);
                mvwprintw(infoPopup, 1, 1, "Meet added");
                mvwprintw(infoPopup, 2, 1, "Press any key to continue.");
                wrefresh(infoPopup);
                getch();
                break;
            case 'n':
            case 'N':
            default:
                break;
            }
            delwin(infoPopup);
            refresh();
            wrefresh(box);
            break;
        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
}
void showRegularMeetsHistory(int userId)
{
    int maxY, maxX, boxY, boxX, recordCount = 0;
    regularVisit *records = regularVisitRetrieve(userId, &recordCount);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(recordCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d records:", recordCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "Person");
        mvwprintw(box, 1, 15, "Date");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            wattron(box, COLOR_PAIR(2));
            mvwprintw(box, i + 2, 1, "%s", retrieveUserById(records[i + j].p2)->username);
            mvwprintw(box, i + 2, 15, "%s", asctime(localtime(&records[i + j].date)));
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < recordCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < recordCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;

        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
}
void showBussinessMeetsHistory(int userId)
{
    int maxY, maxX, boxY, boxX, recordCount = 0;
    businessVisit *records = businessVisitRetrieveForUser(userId, &recordCount);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(recordCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d records:", recordCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "Shop");
        mvwprintw(box, 1, 15, "Date");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            wattron(box, COLOR_PAIR(2));
            mvwprintw(box, i + 2, 1, "%s", retrieveBusinessOwner(records[i + j].businessId)->storeName);
            mvwprintw(box, i + 2, 15, "%s", asctime(localtime(&records[i + j].date)));
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < recordCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < recordCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;

        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
}
void showVisitors(int userId)
{
    int maxY, maxX, boxY, boxX, recordCount = 0;
    businessVisit *records = businessVisitRetrieveForbusiness(userId, &recordCount);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(recordCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d records:", recordCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "Person");
        mvwprintw(box, 1, 7, "Code");
        mvwprintw(box, 1, 15, "Date");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            wattron(box, COLOR_PAIR(2));
            mvwprintw(box, i + 2, 1, "%s", retrieveUserById(records[i + j].regularId)->username);
            mvwprintw(box, i + 2, 1, "%d", records[i + j].regularCode);
            mvwprintw(box, i + 2, 15, "%s", asctime(localtime(&records[i + j].date)));
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < recordCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < recordCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;
        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
}
void addMessage(message *newMessage)
{
    FILE *file;
    mxml_node_t *xml = NULL;
    mxml_node_t *node = NULL;
    mxml_node_t *data = NULL;
    file = fopen("message.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlNewElement(xml, "message");
    data = mxmlNewElement(node, "text");
    mxmlNewText(data, 0, newMessage->text);
    data = mxmlNewElement(node, "senderId");
    mxmlNewText(data, 0, itoa(newMessage->senderId));
    data = mxmlNewElement(node, "date");
    mxmlNewText(data, 0, itoa(newMessage->date));
    fclose(file);
    file = fopen("message.xml", "w");
    mxmlSaveFile(xml, file, MXML_NO_CALLBACK);
    fclose(file);
}
message *getMessageList(int *numberOfItems)
{
    message *messages = NULL;
    FILE *file;
    mxml_node_t *xml;
    mxml_node_t *node;
    mxml_node_t *data;
    *numberOfItems = 0;
    file = fopen("message.xml", "r");
    xml = mxmlLoadFile(NULL, file, MXML_OPAQUE_CALLBACK);
    node = mxmlGetFirstChild(xml);
    while (node != NULL)
    {
        messages = (message *)realloc(messages, ++(*numberOfItems) * sizeof(message));
        data = mxmlGetFirstChild(node);
        strcpy(messages[*numberOfItems - 1].text, mxmlGetOpaque(data));
        data = mxmlGetNextSibling(data);
        messages[*numberOfItems - 1].senderId = atoi(mxmlGetOpaque(data));
        data = mxmlGetNextSibling(data);
        messages[*numberOfItems - 1].date = atoi(mxmlGetOpaque(data));
        data = NULL;
        node = mxmlGetNextSibling(node);
    }
    return messages;
}
void messageMenu(int userId)
{
    int maxY, maxX, boxY, boxX, ch;
    message newMessage;
    newMessage.senderId = userId;
    newMessage.date = time(NULL);
    initscr();
    clear();
    noecho();
    curs_set(1);
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    boxY = (maxY - 8) / 2;
    boxX = (maxX - 50) / 2;
    WINDOW *box = newwin(8, 50, boxY, boxX);
    WINDOW *shadow = newwin(8, 50, boxY + 1, boxX + 2);
    WINDOW *insideBox = derwin(box, 5, 48, 2, 1);
    box(box, 0, 0);
    wbkgd(stdscr, COLOR_PAIR(1));
    wbkgd(shadow, COLOR_PAIR(3));
    wbkgd(insideBox, COLOR_PAIR(2));
    wbkgd(box, COLOR_PAIR(2));
    wattron(box, A_BOLD);
    mvwprintw(box, 1, 1, "Send message to admin");
    wattroff(box, A_BOLD);
    wattron(stdscr, A_BOLD);
    mvwprintw(stdscr, boxY + 9, boxX, "Press F2 to send the message.");
    mvwprintw(stdscr, boxY + 10, boxX, "Press End key to go back.");
    wattroff(stdscr, A_BOLD);
    wrefresh(stdscr);
    wrefresh(shadow);
    wrefresh(box);
    FIELD *fields[2];
    fields[1] = NULL;
    fields[0] = new_field(2, 44, 1, 2, 0, 0);
    set_field_back(fields[0], A_UNDERLINE | COLOR_PAIR(2));
    set_field_fore(fields[0], COLOR_PAIR(2));
    field_opts_on(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP | O_EDIT | O_ACTIVE);
    FORM *form = new_form(fields);
    set_form_win(form, insideBox);
    set_form_sub(form, derwin(insideBox, 3, 46, 0, 0));
    post_form(form);
    box(insideBox, 0, 0);
    wrefresh(insideBox);
    keypad(stdscr, TRUE);
    ch = 0;
    while (ch != KEY_F(2) && ch != KEY_END)
    {
        ch = getch();
        WINDOW *infoPopup = newwin(5, 52, boxY + 3, boxX);
        switch (ch)
        {
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;
        case KEY_F(2):
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_NEXT_FIELD);
            strcpy(newMessage.text, trimWhitespace(field_buffer(fields[0], 0)));
            addMessage(&newMessage);
            errorPopup("Message sent.", boxX, boxY);
            box(box, 0, 0);
            wrefresh(box);
            box(insideBox, 0, 0);
            wrefresh(insideBox);
            break;
        default:
            form_driver(form, ch);
            break;
        }
        wrefresh(insideBox);
    }
    curs_set(0);
    endwin();
}
void messagePopup(char *message, int x, int y)
{
    int ch;
    start_color();
    init_pair(2, 0, 12);
    curs_set(0);
    noecho();
    WINDOW *popUp, *inside;
    popUp = newwin(5, 50, y, x);
    inside = derwin(popUp, 2, 48, 1, 1);
    wclear(popUp);
    wattron(popUp, A_BOLD);
    mvwprintw(popUp, 3, 1, "Prees any key to exit.");
    wattroff(popUp, A_BOLD);
    wbkgd(popUp, COLOR_PAIR(2));
    wbkgd(inside, COLOR_PAIR(2));
    box(popUp, 0, 0);
    mvwprintw(inside, 0, 0, message);
    wrefresh(popUp);
    getch();
}
void messageList(void)
{
    int maxY, maxX, boxY, boxX, messageCount = 0;
    message *messages = getMessageList(&messageCount);
    initscr();
    clear();
    start_color();
    init_pair(1, 0, 240);
    init_pair(2, 0, 12);
    init_pair(3, 17, 17);
    wbkgd(stdscr, COLOR_PAIR(1));
    getmaxyx(stdscr, maxY, maxX);
    int size = min(messageCount, 15);
    boxY = (maxY - size) / 2 - 3;
    boxX = (maxX - 50) / 2;
    attron(A_BOLD);
    mvprintw(boxY - 1, boxX, "There are currently %d messages:", messageCount);
    attroff(A_BOLD);
    refresh();
    WINDOW *box = newwin(size + 3, 50, boxY, boxX);
    WINDOW *shadow = newwin(size + 3, 50, boxY + 1, boxX + 2);
    box(box, 0, 0);
    wbkgd(shadow, COLOR_PAIR(3));
    wrefresh(shadow);
    curs_set(0);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(boxY + size + 4, boxX, "Press end key to exit.");
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    keypad(box, TRUE);
    int j = 0, choice, highlight = 0;
    noecho();
    while (1)
    {
        wclear(box);
        wbkgd(box, COLOR_PAIR(2));
        wattron(box, A_BOLD);
        mvwprintw(box, 1, 1, "From");
        mvwprintw(box, 1, 15, "Date");
        wattroff(box, A_BOLD);
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(box, A_STANDOUT);
            }
            wattron(box, COLOR_PAIR(2));
            mvwprintw(box, i + 2, 1, "%s", retrieveUserById(messages[i + j].senderId)->username);
            mvwprintw(box, i + 2, 15, "%s", asctime(localtime(&messages[i + j].date)));
            wattroff(box, A_STANDOUT);
        }
        wrefresh(box);
        choice = wgetch(box);
        char option;
        switch (choice)
        {
        case KEY_DOWN:
            if (highlight < messageCount)
            {
                highlight++;
            }
            if (highlight == size)
            {
                highlight = size - 1;
                if (j + size < messageCount)
                {
                    j++;
                }
            }
            break;
        case KEY_UP:
            if (highlight > 0)
            {
                highlight--;
            }
            if (highlight == 0)
            {
                if (j > 0)
                {
                    j--;
                }
            }
            break;
        case '\n':
            messagePopup(messages[highlight + j].text, boxX, boxY);
            wbkgd(stdscr, COLOR_PAIR(1));
            wbkgd(shadow, COLOR_PAIR(3));
            wrefresh(stdscr);
            wrefresh(shadow);
            wrefresh(box);
            break;
        default:
            break;
        }
        if (choice == KEY_END)
        {
            break;
        }
    }
    endwin();
}