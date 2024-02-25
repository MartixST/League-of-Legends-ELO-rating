#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define SIZE 100

// Counting lines in files
void numrows (int *rowsm, int *rowsp, char *playerslog, char *matcheslog){
    char str[150];

    FILE* f_matches = fopen(matcheslog, "r");

    if(f_matches == NULL){
        printf("Unable to read the 'matches.txt' file\n");
        exit(1);
    }

    while(!feof(f_matches)){
        fgets(str, 150, f_matches);
        *rowsm = *rowsm + 1;
    }

    fclose (f_matches);

    FILE* f_players = fopen(playerslog, "r");

    if(f_matches == NULL){
        printf("Unable to read the 'players.txt' file\n");
        exit(1);
    }

    while(!feof(f_players)){
        fgets(str, 150, f_players);
        *rowsp = *rowsp + 1;
    }

    fclose (f_players);
}

// Adding the player's number in the array to a separate array
void playersint(int *players, char *str, int checkteam){
    for(int arrplayers = 0, arrstr = 0, repeat = 3; arrplayers < repeat; arrplayers++){
        if(checkteam == 1){
            repeat = 6;
            arrplayers = 3;
            checkteam = -1;
        }
        players[arrplayers] = atoi(str);
        for(int check = 0, count = 0; check < 2; arrstr++){
            if(str[arrstr] == ',' || str[arrstr] == ';' || str[arrstr] == '\n' || str[arrstr] == '\0'){
                if(check == 1){
                    str[count] = '.';
                    arrstr--;
                }
            check++;
            }
            else if(check == 1){
                str[count] = str[arrstr];
                count++;
            }
        }
    }
}

// Adding a player's ID and adding their default rating (1000) to the general array
void playersinstat(int checkteam, int rowsp, float *playersstat, int *players, int *arrplayers){
    for(int arrplayer = 0, arrstat = 0, arrmax = 3; arrplayer < arrmax; arrplayer++){
        if(checkteam == 1 && arrplayer == 0){
            arrplayer = 3;
            arrmax = 6;
        }

        while(arrstat <= rowsp * 7){
            if(playersstat[arrstat] == players[arrplayer]){
                players[arrplayer] = arrstat;
                arrstat = rowsp * 7;
            }
            else if(arrstat == rowsp * 7){
                playersstat[*arrplayers] = players[arrplayer];
                playersstat[*arrplayers + 5] = 1000;
                players[arrplayer] = *arrplayers;
                *arrplayers += 7;
            }
            arrstat += 7;
        }
        arrstat = 0;
    }
}

// Entering information about players into the array
void parameters(int checkteam, float *playersstat, int *players, char *str){
    float kills, assists, deaths;

    for(int count = 0, arrp = 0, parameter = 1, arrstr = 0; count <= 9; count++, parameter++){
        if(checkteam == 1 && count == 0)
            arrp = 3;

        if(parameter == 4){
            playersstat[players[arrp] + 6] += 1;
                    
            if(deaths != 0){
                playersstat[players[arrp] + 4] += (kills + assists) / deaths;
            }
            else
                playersstat[players[arrp] + 4] += kills + assists;

            if(!(parameter == 3 && arrp == 2)){
                parameter = 1;
                arrp++;
            }
        }
        if(count == 9){
            count++;
        }
        else{
            playersstat[players[arrp] + parameter] += atoi(str);
            if(parameter == 1)
                kills = atoi(str);
            else if(parameter == 2)
                assists = atoi(str);
            else if(parameter == 3)
                deaths = atoi(str);
                   
            for(int check = 0, arrs = 0; check < 2; arrstr++){
                if(str[arrstr] == ',' || str[arrstr] == ';' || str[arrstr] == '\n'){
                    if(check == 1){
                        str[arrs] = '.';
                        arrstr--;
                    }
                    check++;
                }
                else if(check == 1){
                    str[arrs] = str[arrstr];
                    arrs++;
                }
            }
        }
    }
}


// Calculation of the player's rating
void calcrating(float *playersstat, int *players, int checkwin){
    int rbblue, rbred, redstatus, bluestatus;
    float ea;

    if(checkwin == 0){
        redstatus = 1;
        bluestatus = 0;
    }
    else{
        redstatus = 0;
        bluestatus = 1;
    }

    rbblue = (playersstat[players[3] + 5] + playersstat[players[4] + 5] + playersstat[players[5] + 5]) / 3;
    rbred = (playersstat[players[0] + 5] + playersstat[players[1] + 5] + playersstat[players[2] + 5]) / 3;

    for(int red = 0, blue = 3, rared, rablue; red < 3; red++, blue++){
        rared = playersstat[players[red] + 5];
        rablue = playersstat[players[blue] + 5];

        ea = 1 / (1 + powf(10, ((rbblue - rared) / 400)));
        playersstat[players[red] + 5] = rared + 30 * (redstatus - ea);

        ea = 1 / (1 + powf(10, ((rbred - rablue) / 400)));
        playersstat[players[blue] + 5] = rablue + 30 * (bluestatus - ea);

        if(playersstat[players[blue] + 5] < 0)
            playersstat[players[blue] + 5] = 0;
        else if(playersstat[players[red] + 5] < 0)
            playersstat[players[red] + 5] = 0;
    }
}

// Calculation of the average value of the parameters
void average(float *playersstat, int rowsp){
    for(int i = 0, arr = 1; i < rowsp; i++){
        for(int q = 0; q < 4; q++)
            playersstat[arr + q] = playersstat[arr + q] / playersstat[arr + 5];
        arr += 7;
    }    
}

// Distribution of players by rating in the table
void placeplayer(int *place, float *playersstat, int rowsp, char *nicknames, char *playerslog){
    memset(place, -1, (sizeof(int) * rowsp));

    for(int count = 0, arrplace = 0; count < rowsp; count++){
        for(int i = 0, arrp = 0, rating = 0; i < rowsp; i++){
            if(playersstat[arrp + 5] > rating){
                for(int check = 0, arr = 0; check < 1; arr++){
                    if(place[arr] == -1){
                        place[arrplace] = playersstat[arrp];
                        rating = playersstat[arrp + 5];
                        check++;
                    }
                    else{
                        for(int q = 0; q <= arr && check == 0; q++){
                            if(place[q] == playersstat[arrp])
                                check++;
                        }
                    }
                }
            }
            arrp += 7;
        }
        arrplace++;
    }

    FILE* f_players = fopen(playerslog, "r");

    char str[SIZE];

    for(int count = 0, arrplace = 0, arrnick = 0; count < rowsp; count++){
        for(int check = 0, numplayer; 1 > check;){
            fgets(str, SIZE, f_players);
            numplayer = atoi(str);
            if(numplayer == place[arrplace]){
                for(int arrstr = 1, checknick = 0; check < 1; arrstr++){
                    if(str[arrstr] == '\0' || str[arrstr] == '\n'){
                        check++;
                        nicknames[arrnick] = '.';
                        arrnick++;
                        checknick--;
                    }
                    else if(str[arrstr] == ',' && checknick == 0){
                        checknick++;
                        arrstr++;
                    }
                    if(checknick == 1){
                        nicknames[arrnick] = str[arrstr];
                        arrnick++;
                    }
                }
                arrplace++;
                rewind(f_players);
            }
        }
    }

    fclose(f_players);
}

// HTML code output
void HTML(float *playersstat, char *output, char *playerslog, int rowsp){
    int place[1000];
    char nicknames[10000];
    int arrnick = 0;

    placeplayer(place, playersstat, rowsp, nicknames, playerslog);

    FILE* f_output = fopen(output, "w");

    fputs("<html>\n", f_output);
    fputs("  <head>\n", f_output);
    fputs("    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n", f_output);
    fputs("    <title>ELO rating</title>\n", f_output);
    fputs("    <style type=\"text/css\">\n", f_output);
    fputs("     TABLE {\n", f_output);
    fputs("      width: 300px;\n", f_output);
    fputs("      border-collapse: collapse;\n", f_output);
    fputs("     }\n", f_output);
    fputs("     TD, TH {\n", f_output);
    fputs("      padding: 3px;\n", f_output);
    fputs("      border: 1px solid black;\n", f_output);
    fputs("     }\n", f_output);
    fputs("     TH {\n", f_output);
    fputs("      background: #b0e0e6;\n", f_output);
    fputs("     }\n", f_output);
    fputs("    </style>\n", f_output);
    fputs("  </head>\n", f_output);
    fputs("  <body bgcolor = #c0c0c0>\n", f_output);
    fputs("    <table>\n", f_output);
    fputs("      <tbody>\n", f_output);
    fputs("        <tr style= \"background-color: rgb(155, 155, 155);\">\n", f_output);
    fputs("          <th>№</th>\n", f_output);
    fputs("          <th>Nickname</th>\n", f_output);
    fputs("          <th>Avg. Kills</th>\n", f_output);
    fputs("          <th>Avg. Assists</th>\n", f_output);
    fputs("          <th>Avg. Deaths</th>\n", f_output);
    fputs("          <th>Avg. KDA</th>\n", f_output);
    fputs("          <th>Rating</th>\n", f_output);
    fputs("          <th>Matches</th>\n", f_output);
    fputs("          <th>Division</th>\n", f_output);
    fputs("        </tr>\n", f_output);

    char nickname[SIZE];
    
    for(int countplayer = 1, crownarr = 0, sizetext = 4, arrstat = 0, arrplace = 0; countplayer <= rowsp; countplayer++, crownarr++){
        for(int check = 0, arr = 0; check < 1; arr++, arrnick++){
            if(nicknames[arrnick] == '.'){
                nickname[arr] = '\0';
                check++;
            }
            else
                nickname[arr] = nicknames[arrnick];
        }

        if(sizetext == 4){
            fputs("          <tr style=\"background-color: rgb(212, 175, 55);\">\n", f_output);
            fprintf(f_output, "            <td ><font size=\"+%d\">♛%d.</font></td>\n", sizetext, countplayer);
        }
        else if(sizetext == 3){
            fputs("          <tr style=\"background-color: rgb(192,192,192);\">\n", f_output);
            fprintf(f_output, "            <td ><font size=\"+%d\">♜%d.</font></td>\n", sizetext, countplayer);
        }
        else if(sizetext == 2){
            fputs("          <tr style=\"background-color: rgb(205, 127, 50);\">\n", f_output);
            fprintf(f_output, "            <td ><font size=\"+%d\">♞%d.</font></td>\n", sizetext, countplayer);
        }
        else{
            fputs("          <tr style=\"background-color: rgb(219, 218, 173);\">\n", f_output);
            fprintf(f_output, "            <td ><font size=\"+%d\">%d.</font></td>\n", sizetext, countplayer);
        }

        for(int check = 0, arr = 0; check < 1;){
            if(playersstat[arr] == place[arrplace]){
                arrstat = arr;
                check++;
                arrplace++;
            }
            arr += 7;
        }

        fprintf(f_output, "            <td><font size=\"+%d\">%s</font></td>\n", sizetext, nickname);
        for(int parameter = 1; parameter <= 4; parameter++){
                fprintf(f_output, "            <td><font size=\"+%d\">%.2f</font></td>\n", sizetext, playersstat[arrstat + parameter]);
        }
        fprintf(f_output, "            <td><font size=\"+%d\">%.0f</font></td>\n", sizetext, playersstat[arrstat + 5]);
        fprintf(f_output, "            <td><font size=\"+%d\">%.0f</font></td>\n", sizetext, playersstat[arrstat + 6]);

        if(playersstat[arrstat + 5] <= 1149)          
                fprintf(f_output, "            <td><font size=\"+%d\">Bronze</font></td>\n", sizetext);
        else if(playersstat[arrstat + 5] >= 1150 && playersstat[arrstat + 5] <= 1499)        
                fprintf(f_output, "            <td><font size=\"+%d\">Silver</font></td>\n", sizetext);
        else if(playersstat[arrstat + 5] >= 1500 && playersstat[arrstat + 5] <= 1849)         
                fprintf(f_output, "            <td><font size=\"+%d\">Gold</font></td>\n", sizetext);
        else if(playersstat[arrstat + 5] >= 1850 && playersstat[arrstat + 5] <= 2199)
            fprintf(f_output, "            <td><font size=\"+%d\">Platinum</font></td>\n", sizetext);
        else if(playersstat[arrstat + 5] >= 2200) 
            fprintf(f_output, "            <td><font size=\"+%d\">Diamond</font></td>\n", sizetext);
        
        if(sizetext > 1)            
            sizetext--;

        fputs("          </tr>\n", f_output);
    }
    
    fputs("      <tbody>\n", f_output);
    fputs("    <table>\n", f_output);
    fputs("  <body>\n", f_output);
    fputs("<html>", f_output);

    fclose(f_output);
}

int main(int argc, char** argv){
    if(argc < 4){
        printf("Wrong parameters");
        return 1;
    }

    char matcheslog[SIZE];
    char playerslog[SIZE];
    char output[SIZE];


    strcpy(matcheslog, argv[1]);
    strcpy(playerslog, argv[2]);
    strcpy(output, argv[3]);

    int rowsm = 0;
    int rowsp = 0;

    numrows(&rowsm, &rowsp, playerslog, matcheslog);
    
    char str[SIZE];
    char example[SIZE] = "match\n";
    int players[6];
    int arrplayers = 0;
    float playersstat[10000];
    int error = 0;

    memset(playersstat, 0, (sizeof(float) * (rowsp * 7)));

    FILE* f_matches = fopen(matcheslog, "r");

    if(f_matches == NULL){
        printf("Unable to read the 'matches.txt' file\n");
        return 1;
    }

    for(int i = 0, countrows = 1, checkteam = 0; i < rowsm; i++, countrows++){
        if(countrows == 4 && checkteam == 0){
            countrows = 2;
            checkteam++;
        }
        else if(countrows == 5 && checkteam == 1){
            countrows = 1;
            checkteam--;
        }

        if(countrows >= 2 || checkteam == 0)
            fgets(str, SIZE, f_matches);

        if(countrows == 1 && checkteam == 0){
            if(strcmp(example, str) != 0){
                printf("Incorrectly entered data\n");
                return 1;
            }
        }
        else if(countrows == 2){
            playersint(players, str, checkteam);
            playersinstat(checkteam, rowsp, playersstat, players, &arrplayers);
        }
        else if(countrows == 3)
            parameters(checkteam, playersstat, players, str);
        else if(countrows == 4){
            if(str[0] == 'r')
                calcrating(playersstat, players, 0);
            else if(str[0] == 'b')
                calcrating(playersstat, players, 1);
            else{
                printf("Incorrectly entered data\n");
                return 1;
            }
        }
    }

    fclose(f_matches);

    average(playersstat, rowsp);

    HTML(playersstat, output, playerslog, rowsp);

    return 0;
}