#include <stdio.h>
#include <stdlib.h>// system code
#include <string.h>
#include <stdbool.h>
#include <ctype.h> 
#include <math.h> // ceil function 
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#define MAX_SONG_POOL 120
#define MAX_STRING_SIZE 200
// Data Structure for an individual song
int count=0;
int random_temp=-1;
typedef struct Song *songptr;
typedef struct Song
{
    char title[MAX_STRING_SIZE];
    char album[MAX_STRING_SIZE];
    char uri[MAX_STRING_SIZE];
    short int year;
    int duration;
    int priority;
    int index;
} Song;

songptr song_pool[200],shuffle=NULL;
int pool_insert_index = 0;

typedef struct PlaylistNode *node_ptr;
typedef struct PlaylistNode
{
    songptr song;
    node_ptr prev_song;
    node_ptr next_song;
} PlaylistNode;

node_ptr header_node = NULL, now_playing = NULL,prior=NULL,shuffsong=NULL,shufftemp=NULL;

//Function to check if the song pool is empty
bool is_pool_empty()
{
    return song_pool[0] == NULL;
}

//Function to check if playlist exists
bool does_playlist_exist()
{
    return !(header_node == NULL);
}
void play_song(int n){
    char adress[200];
    char buffer[300];
    int durations;
    strcpy(adress,now_playing->song->uri);
    if(n==1)
        system("killall afplay");
    durations=now_playing->song->duration;
    sprintf(buffer, "afplay \"%s\" -t %d & disown",adress,durations);
    system(buffer);
}

void play_next_song()
{
    if (does_playlist_exist() && now_playing != NULL)
    {
        if (now_playing->next_song != NULL)
            {now_playing = now_playing->next_song; 
            play_song(1);}
        else
            printf("REACHED END OF PLAYLIST");
    
    }
    else
    {
        printf("NO SONG ADDED TO PLAYLIST");
    }
}

void play_prev_song()
{
    if (does_playlist_exist() && now_playing != NULL)
    {
        if (now_playing->prev_song != NULL)
            {
                now_playing = now_playing->prev_song;
                play_song(1);
            }
        else
            printf("REACHED START OF PLAYLIST");
    }
    else
    {
        printf("NO SONG ADDED TO PLAYLIST");
    }
}
void shuffle_song(){
       while(true){
        int num= rand() % 10 ;
        if(num!=random_temp){
        node_ptr new_node = (node_ptr)malloc(sizeof(PlaylistNode));
        node_ptr last = header_node;
        new_node->song = song_pool[num];
        new_node->next_song = NULL;
        /* 4. If the Linked List is empty, then make the new
          node as head */
        if (header_node == NULL)
        {
            new_node->prev_song = NULL;
            header_node = new_node;
            now_playing = new_node;
            play_song(0);
            count++;
        }
        else
        {
            while (last->next_song != NULL)
                last = last->next_song;

            last->next_song = new_node;
            new_node->prev_song = last;
            now_playing = new_node;
            play_song(1);
            }
                random_temp = num; 
                break;
             }
        }
     
   
   
}

void show_song_details()
{
    if (now_playing == NULL || now_playing->song == NULL)
    {
        printf("                       CREATE A PLAYLIST FIRST\n");
    }
    else
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("                          DETAILED OVERVIEW\n");
        printf("----------------------------------------------------------------------\n");
        printf("\t   TITLE    |   %s\n", now_playing->song->title);
        printf("\t   ALBUM    |   %s\n", now_playing->song->album);
        printf("\t   YEAR     |   %d\n", now_playing->song->year);
        printf("\t  DURATION  |   %02d:%02d minutes\n", now_playing->song->duration/60,now_playing->song->duration%60);
        printf("\t  LOCATION  |   %s\n", now_playing->song->uri);
        printf("----------------------------------------------------------------------\n");
    }
    char leave[MAX_STRING_SIZE];
    printf("<<<<< Enter any input to return to MAIN MENU\n");
    if (scanf("%s", leave))
    {
        system("clear");
        return;
    }
}

void main_menu()
{
    printf("\n----------------------------------------------------------------------\n");
    printf("                              MAIN MENU\n");
    printf("----------------------------------------------------------------------\n");

    if (now_playing == NULL || now_playing->song == NULL)
    {
        printf("                          NO TRACK SELECTED\n");
    }
    else
    {
        printf("    NOW    |   %s\n", now_playing->song->title);
        printf("  PLAYING  |   %02d:%02d minutes\n", now_playing->song->duration/60, now_playing->song->duration%60);
    }
    printf("----------------------------------------------------------------------\n");
    printf("   #   |  Action   \n");
    printf("----------------------------------------------------------------------\n");
    printf("   1   |  Display all available songs\n");
    if (does_playlist_exist())
        printf("   2   |  Delete playlist\n");
    else
        printf("   2   |  Create a new playlist\n");
    printf("   3   |  Add songs to the playlist\n");
    printf("   4   |  Show playlist\n");
    printf("   5   |  Play previous track\n");
    printf("   6   |  Play next track\n");
    printf("   7   |  Play Recommended track\n");
    if (now_playing != NULL && now_playing->song != NULL)
        printf("   8   |  Show more information about the song playing\n");
    printf("  -1   |  Exit music player\n");
    printf("----------------------------------------------------------------------\n");
    printf("                        Enter your choice below\n");
}

void show_all_songs_raw()
{
    for (int i = 0; i < MAX_SONG_POOL && song_pool[i] != NULL; i++)
        printf("%-1d %-3s %-3s %-3d %02d:%02d min\n", (i + 1), song_pool[i]->title, song_pool[i]->album, song_pool[i]->year, song_pool[i]->duration/60,song_pool[i]->duration%60);
}

void show_playlist()
{
    if (does_playlist_exist())
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("                          YOUR PLAYLIST\n");
        printf("----------------------------------------------------------------------\n");
        printf(" Title                                            | Duration\n");
        printf("----------------------------------------------------------------------\n");
        node_ptr current = header_node;
        while (current != NULL && current->song != NULL)
        {
            node_ptr next = current->next_song;
            printf(" %-48.48s | %02d:%02d min\n", current->song->title, current->song->duration/60,current->song->duration%60);
            current = next;
        }

        printf("----------------------------------------------------------------------");
    }
    else
    {
        printf("\nYou haven't created a playlist yet.\n");
    }
}

void pagewise_song_display(int step)
{
    printf("\n----------------------------------------------------------------------\n");
    printf("                             SONGS LIST\n");
    printf("----------------------------------------------------------------------\n");
    printf("   #   |  Title                                            | Duration\n");
    printf("----------------------------------------------------------------------\n");
    for (int i = step - 10; i < step && song_pool[i] != NULL; i++)
    {
        printf("  %2d   |  %-48.48s | %02d:%02d min\n", (i + 1), song_pool[i]->title, song_pool[i]->duration/60,song_pool[i]->duration%60);
    }
    printf("----------------------------------------------------------------------\n");
    printf("[Enter -2 to go to prev page] | Page %d | [Enter -1 to go to next page]\n", ((int)step / 10));
    printf("----------------------------------------------------------------------\n");
    printf(" <<<< Enter 0 to go back to main menu.\n");

    printf("----------------------------------------------------------------------\n");
    printf("                        Enter your choice below\n");
}

int song_selector()
{
    int song_number = -1;
    int step = 10;
    while (song_number < 0 || song_number > pool_insert_index)
    {
        pagewise_song_display(step);
        char input[MAX_STRING_SIZE];
        scanf("%s", input);
        if (!sscanf(input, "%d", &song_number))
        {
            system("clear");
            printf("You seem to have entered an invalid input.\nReturning to main menu.");
            return 0;
        }

        if (song_number == -2)
        {
            if (step > 10)
            {
                step -= 10;
                system("clear");
            }
            else
            {
                system("clear");
                printf("\n No Previous Page");
            }
        }
        else if (song_number == -1)
        {
            if (step < ceil(pool_insert_index / 10.0) * 10)
            {
                step += 10;
                system("clear");
            }
            else
            {
                system("clear");
                printf("\nThis is the last page");
            }
        }
        else if (song_number < -2 || song_number > pool_insert_index)
        {
            system("clear");
            printf("\nENTER A VALID CHOICE\n");
        }
    }

    return song_number;
}
void rearrange (songptr temp){
            temp->priority++;
            int l=0;int k;
            for(int i=temp->index;i<pool_insert_index-1;i++)
                        song_pool[i]=song_pool[i+1];

             while(l!=pool_insert_index){
                    if( temp->priority <=song_pool[l]->priority )
                    l++;
                    else
                    break;
                }
                    for(k= pool_insert_index-1;k>l;k--){
                         song_pool[k]=song_pool[k-1];
                         if(k<=temp->index)
                            song_pool[k]->index++;
                    }
        song_pool[l] = temp;
        temp->index=l;
}
Song *createSong(const char *title, const char *album, const short int year, const int duration, const char *uri,const int priority1,const int i)
{
    Song *temp = malloc(sizeof(Song));
    strcpy(temp->title, title);
    strcpy(temp->album, album);
    strcpy(temp->uri, uri);
    temp->year = year;
    temp->duration = duration;
    temp->priority=priority1;
    temp->index=i;
    pool_insert_index++;
    return temp;
}


void create_playlist()
{
    int song_number = 0, i = 0;

    node_ptr temp = (node_ptr)malloc(sizeof(PlaylistNode));
    printf("\nPick the song you want to add to your new playlist\n");
    song_number = song_selector();
    if (song_number > 0 && song_number <= pool_insert_index)
    {   
        temp->song = song_pool[song_number - 1];
        temp->prev_song = NULL;
        temp->next_song = NULL;
        header_node = temp;
        now_playing = temp;
        system("clear");
        printf("---\n%s has been added to your new playlist.\n---", song_pool[song_number-1 ]->title);
        
        rearrange(now_playing->song);
        play_song(0);
        count++;

    }
    else if (song_number == 0)
    {
        system("clear");
    }
    else
    {
        printf("\nThere was a problem while handling your request, try again.\n");
    }
}

void add_to_playlist()
{
    int song_number = 0;
    int step = 10;
    printf("\nPick the song you want to add\n");
    song_number = song_selector();
    if (song_number > 0 && song_number <= pool_insert_index)
    {
        node_ptr new_node = (node_ptr)malloc(sizeof(PlaylistNode));
        node_ptr last = header_node;
        new_node->song = song_pool[song_number - 1];
        new_node->next_song = NULL;
        /* 4. If the Linked List is empty, then make the new
          node as head */
        if (header_node == NULL)
        {
            new_node->prev_song = NULL;
            header_node = new_node;
            now_playing = new_node;
            system("clear");
            printf("%s has been added to your playlist.\n", song_pool[song_number - 1]->title);
            rearrange(now_playing->song);
            count++;
            play_song(0);
        }
        else
        {

            while (last->next_song != NULL)
                last = last->next_song;

            last->next_song = new_node;
            new_node->prev_song = last;
            system("clear");
            printf("%s has been added to your playlist.\n", song_pool[song_number - 1]->title);
            rearrange(new_node->song);
            
            count++;
        }

    }
    else if (song_number == 0)
    {
        system("clear");
    }
    else
    {
        printf("\nThere was a problem while handling your request.\n");
    }

    
}

/** CSV Parser that maps a data set of upto songs onto the song pool
    Songs must be present in the root directory at rawdata.csv
*/
void readFromCSV()
{
    FILE *file = fopen("rawdata.csv", "r");
    char line[1000];
    int i = 0;

    if (file == NULL)
    {
        perror("******\nUnable to load songs from data source");
        printf("Please check if rawdata.csv exists at the root directory of the application.\n");
        printf("You can still enter songs manually.\n******\n");
        printf("Error Message");
    }
    else
    {
        char songtitle[MAX_STRING_SIZE];
        char releaseDate[MAX_STRING_SIZE];
        int durationms;
        char id[MAX_STRING_SIZE];
        char album[MAX_STRING_SIZE];
        char uri[MAX_STRING_SIZE];
        int year;
        int priority1=0;
        int num;
        while (fgets(line, sizeof(line), file) && pool_insert_index < MAX_SONG_POOL)
        {
            char *token;
            token = strtok(line, ","); //tokenizes current line with delimiter ',' and returns the first substring
            if (token == NULL)
                continue;
            strcpy(album, token);
            token = strtok(NULL, ","); 
            strncpy(releaseDate, token + 1, 4);
            
            sscanf(releaseDate, "%d", &year);
            token = strtok(NULL, ",");
            char temp[MAX_STRING_SIZE];
            strncpy(temp, token + 1, 6);
            sscanf(temp, "%d", &durationms);
            token = strtok(NULL, ","); 
            strcpy(songtitle, token);
            token = strtok(NULL, ","); 
            strcpy(uri, token);
            token = strtok(NULL, ",");
          
            if (i == 0)
            {
                i++;
                continue;
            }
            song_pool[i - 1] = createSong(songtitle, album, year, durationms,uri,priority1,(i-1));
            i++;
            
        }
            
        }
    

    
    fclose(file);
   
}


void user_song_input()
{
    int add_another = 1;
    char songname[MAX_STRING_SIZE];
    int year;
    int priority;
    int durationms;
    char id[MAX_STRING_SIZE];
    char album[MAX_STRING_SIZE];
    char uri[MAX_STRING_SIZE] = "/Users/ushanandhini/Desktop/PDSProject/PDSProject/Music Library/";

    while (add_another && pool_insert_index < MAX_SONG_POOL)
    {
        system("clear");
        printf("\nEnter the details of the song you want to create.\n[ Don't give spaces between each word, use _ instead\n");
        printf("Title: ");
        scanf("%s", songname);
        printf("Album Name: ");
        scanf("%s", album);
        printf("Year of release: ");
        if (!(scanf("%d", &year)))
        {
            printf("INVALID INPUT ENTERED\n");
            sleep(1);
            break;
        }
        printf("Duration(in s): ");
        if (!(scanf("%d", &durationms)))
        {
            printf("INVALID INPUT ENTERED\n");
            sleep(1);
            break;
        }
        printf("Mp3 File Location: ");
        scanf("%s", id);
        strcat(uri, id);
        int n= pool_insert_index;
        song_pool[n] = createSong(songname, album, year, durationms, uri,0,pool_insert_index);
        printf("\nThe song %s has been created.\n\n", songname);
        printf("\nDo you want to create another song?\n(Enter 1 for yes and 0 for no) : ");
        scanf("%d", &add_another);
    }
    system("clear");
}

bool delete_playlist()
{
    if (header_node != NULL)
    {
        node_ptr current = header_node;
        while (current != NULL)
        {
            node_ptr next = current->next_song;
            free(current);
            current = next;
        }
        header_node = NULL;
        now_playing = NULL;
        system("killall afplay");
        return true;
    }
    return false;
}

void free_all_memory()
{
    delete_playlist();
    for (int i = 0; i < MAX_SONG_POOL && song_pool[i] != NULL; i++)
       {
        free(song_pool[i]);}
    printf("\n---END---");


 
}

int main()
{
    int userChoice = 0;
    int wrong_choice_count = 0;
    system("clear");
    readFromCSV();
    while (userChoice != -1)
    {
        switch (userChoice)
        {
        case 0:
        { //Show menu options
            main_menu();
            break;
        }
        case 1:
        {
            system("clear");
            //show_all_songs();
            int user_song_selection = song_selector();
            system("clear");
            break;
        }
        case 2:
        {
            system("clear");
            if (does_playlist_exist())
            {
                printf("---");
                if (delete_playlist())
                    printf("\nThe playlist was successfully deleted.\n");
                else
                    printf("\nYou haven't created a playlist yet. Nothing to delete\n");

                printf("---");
            }
            else
                create_playlist();
            break;
        }
        case 3:
        {
            system("clear");
            add_to_playlist();
            break;
        }
        case 4:
        {
            system("clear");
            show_playlist();
            break;
        }
        case 5:
        {
            system("clear");
            play_prev_song();
            break;
        }
        case 6:
        {
            system("clear");
            play_next_song();
            break;
        }
        case 7:
        {
            system("clear");
            shuffle_song();
            break;
            system("clear");
            show_song_details();
            break;
        }
        case 8:
        {
            system("clear");
            show_song_details();
            break;
        }
        default:
        {
            if (wrong_choice_count == 3)
            {
                system("clear");
                printf("Please enter a valid option or the program will terminate.\n");
            }
            if (wrong_choice_count > 3)
            {
                system("clear");
                printf("Sorry you have exceeded the maximum number of retries, terminating..");

                free_all_memory();
                exit(1);
            }
            else
            {
                system("clear");
                printf("Please enter a valid option from the main menu below\n");
            }
            wrong_choice_count++;
            break;
        }
        }
        if (userChoice != 0)
        {
            main_menu();
        }

        char input[MAX_STRING_SIZE];
        scanf("%s", input);

        if (!sscanf(input, "%d", &userChoice))
        {
            system("clear");
            printf("----\nINVALID INPUT\n");
            userChoice = 0;
            wrong_choice_count++;
            if (wrong_choice_count > 3)
            {
                printf("Sorry you have exceeded the maximum number of retries, terminating..");
                free_all_memory();
                exit(1);
            }
            continue;
        }
    }
    free_all_memory();
    return 0;
}

