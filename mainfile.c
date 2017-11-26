/// Written by Ian Kampine and Fahir Nasir using CodeBlocks 16.01
/// Main menu/execution control.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/// Function declarations.
void readDataIn(char path_to_folder[255]);



/// Struct declarations.
struct blood_pressure
{
    double timestamp;
    double diastolic;
    double systolic;
};

struct body_temperature
{
    double timestamp;
    double temp;
    char mW[2];
};

struct heart_rate
{
    double timestamp;
    double rate;
    char permin[6];
};





/// Handles all user input and output. This is effectively the main menu function.
void main() {

    /// Setting up basic variables for flow control
    /// as well as critical parameters.
    int running = 1;
    int folder_loc_set = 0;
    int time_interval_set = 0;
    int time_for_score_set = 0;
    double time_start;
    double time_stop;
    double time_for_score;
    char path_to_folder[255];
    char option;

    /// Declare a few arrays of data from the struct to hold everything.
    struct blood_pressure BP[45];
    struct body_temperature BT[43];
    struct heart_rate HR[25];


    /// Main menu loop/main logic loop.
    while(running) {


        /// Most functionality here should be pretty clear. Running is the variable used to ensure that the whole program continues looping
        /// until the user tells it to exit. Each input step also performs preliminary input validation.
        printf("Please select one of the following options: (case-sensitive) \n");
        printf("a) Set input folder location. \n");
        printf("b) Set time interval for calculations. \n");
        printf("c) Get health score for single time. \n");
        printf("d) Get statistical values for time intervals. \n");
        printf("e) Exit program. \n");
        scanf('%c', &option);


        switch(option) {



            case 'a' :
                printf("Please enter the location of the input files. Please use a relative path. \n");
                scanf('%s', &path_to_folder);
                /// ***PASS THIS RECORDED PATH TO THE DATA INPUT/FILE READING FUNCTION***
                folder_loc_set = 1;
                printf("Folder location set to %s \n", path_to_folder);
                break;



            case 'b' :
                /// First, make sure that the user has specified the location of the files. Some information from the files is needed for basic input validation.
               if(!folder_loc_set) {
                    printf("Please set the path to the folder before setting a time period for calculations. Use option a) in the previous menu. \n");
               } else {
                    printf("Please enter the start time (in seconds) for the desired time interval. \n");
                    scanf('%f', &time_start);
                    printf("Please enter the stop time (in seconds) for the desired time interval. \n");
                    scanf('%f', &time_stop);
                    time_interval_set = 1;
               }

               /// Next, make sure that the times entered make sense.
               if(time_start > time_stop) {
                    printf("Error. Stop time for the interval must be greater than the start time. \n");
                    time_interval_set = 0;
               }
               if(time_start < 0) {
                    printf("Error. Start time cannot be negative. Negative time is nonsense. \n");
                    time_interval_set = 0;
               }
               if(time_stop < 0) {
                    printf("Error. Stop time cannot be negative. Negative time is nonsense. \n");
                    time_interval_set = 0;
               }
               if(time_stop = time_start) {
                    printf("Error. Values may not be equal. This is not an interval. \n");
                    time_interval_set = 0;
               }

               /// TODO: INCLUDE SECTION TO CHECK THAT THE TIMES PROVIDED ARE WITHIN THE BOUNDS OF THE DATASET PROVIDED.

               if(time_interval_set) {
                    printf("Time interval set successfully! \n");
               } else {
                    printf("Time interval was not set successfully. Rectify errors and try again. \n");
               }
               break;



            case 'c' :
                /// First, check to see if the user has bothered to set the proper folder and read in data.
                if(!folder_loc_set) {
                    printf("Error. Please set the folder that contains the data to analyze before beginning analysis. \n");
                } else {
                    printf("Please enter the time you would like to calculate the health score at. \n");
                    scanf("%f", &time_for_score);
                    time_for_score_set = 1;
                }
                /// Basic input validation for this case.
                if(time_for_score < 0) {
                    printf("Error. Time cannot be negative. \n");
                    time_for_score_set = 0;
                }
                /// INSERT CHECK FOR WHETHER THE SUPPLIED TIME VALUE IS WITHIN THE TIMES SPECIFIED BY THE FILE
                break;



            case 'd' :
                /// Need data to be loaded and also need to know the time interval to calculate the desired statistics.
                if(!folder_loc_set || !time_interval_set) {
                    printf("Error. Must specify the time interval for analysis and/or specify the location of the input sensor data before calculating statistics. \n");
                } else {
                    /// PERFORM THE CALCULATIONS ON STATISTICAL PARAMETERS
                }
                break;



            case 'e' :
                printf("Exiting program. \n");
                running = 0;
                break;



            default :
                printf("Invalid input. Please select one of the options provided above. \n");
                break;

        }

    }

}




/// Definitions of functions. They appear in the order that they are used in main() above.



void readDataIn(char path_to_folder[255]) {


    char filename[255];
    FILE *infile;

    /// Added this to simplify interfacing your function with the main program loop.
    strcpy(path_to_folder, filename);

    //Blood Pressure Files
    int a;
    int i = 0;
    char filename2[255];
    strcat(filename,"\\BP_%d.txt");
    for (a = 1; a<=5; a++)
        {
            sprintf(filename2, filename, a);
            infile = fopen(filename2, "r");

            if(infile==NULL)
            {
                printf("File NOT FOUND!");
            }

            if (a==1)
                {
                    fscanf(infile, "%lf", &BP[i].timestamp);
                    fscanf(infile, "%lf", &BP[i].diastolic);
                    fscanf(infile, "%lf", &BP[i].systolic);

                    while (!feof(infile))
                    {
                        //pre-increment since i at zero was already counted for manually
                        ++i;
                        fscanf(infile, "%lf", &BP[i].timestamp);
                        fscanf(infile, "%lf", &BP[i].diastolic);
                        fscanf(infile, "%lf", &BP[i].systolic);
                    }
                }
                //after the first file is complete, moves onto files 2 through 5
                if (a!=1 && a<=5)
                {
                    while (!feof(infile))
                    {
                        //post increment to continue the counting and make sure that it is sequential
                        ++i;
                        fscanf(infile, "%lf", &BP[i].timestamp);
                        fscanf(infile, "%lf", &BP[i].diastolic);
                        fscanf(infile, "%lf", &BP[i].systolic);
                    }
                }

            fclose(infile);
        }

    //Switching to Body Temperature Files
    //removes the \BP_%d.txt
    char *temp;
    temp = strchr(filename, 'B');
    *temp = '\0';

    FILE *infile2;
    int j = 0;
    strcat(filename,"BT_%d.txt");

    for (a = 1; a<=5; a++)
        {
            sprintf(filename2, filename, a);
            infile2 = fopen(filename2, "r");

            if(infile2==NULL)
            {
                printf("File NOT FOUND!");
            }

             if (a==1)
                {
                    fscanf(infile2, "%lf", &BT[j].timestamp);
                    fscanf(infile2, "%lf%*s", &BT[j].temp);
                }

                    while (!feof(infile2))
                    {
                        //pre-increment since i at zero was already counted for manually
                        ++j;
                        fscanf(infile2, "%lf", &BT[j].timestamp);
                        fscanf(infile2, "%lf%*s", &BT[j].temp);
                    }


                if (a!=1 && a<=5)
                {

                    while (!feof(infile2))
                    {
                        ++j;
                        fscanf(infile2, "%lf", &BT[j].timestamp);
                        fscanf(infile2, "%lf%*s", &BT[j].temp);
                    }
                }

            fclose(infile2);
        }

    //Switching to Heart Rate Data

    temp = strchr(filename, 'B');
    *temp = '\0';

    FILE *infile3;
    int k = 0;
    strcat(filename,"HR_%d.txt");
    for (a = 1; a<=5; a++)
        {
            sprintf(filename2, filename, a);
            infile3 = fopen(filename2, "r");

            if(infile3==NULL)
            {
                printf("File NOT FOUND!");
            }

            fscanf(infile3, "%lf", &HR[k].timestamp);
            fscanf(infile3, "%lf", &HR[k].rate);
            fscanf(infile3, "%[^\n]", HR[k].permin);


            while (!feof(infile3))
                {
                    //pre-increment since i at zero was already counted for manually
                    ++k;
                    fscanf(infile3, "%lf", &HR[k].timestamp);
                    fscanf(infile3, "%lf", &HR[k].rate);
                    fscanf(infile3, "%[^\n]", HR[k].permin);

                }

            fclose(infile3);
        }
}



