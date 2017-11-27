/// Written by Ian Kampine and Fahir Nasir using CodeBlocks 16.01
/// Main menu/execution control.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define NUM_HR_MEASUREMENTS 25
#define NUM_BP_MEASUREMENTS 45
#define NUM_T_MEASUREMENTS 43



/// Function declarations, sorted in blocks by function (Input, Stats, Health Score, Output)
void readDataIn(char path_to_folder[255]);

double meanT(double timestart, double timestop);
double meanBP(double timestart, double timestop, int sys);
double meanHR(double timestart, double timestop);
double sdT(double timestart, double timestop);
double sdBP(double timestart, double timestop, int sys);
double sdHR(double timestart, double timestop);
double minT(double timestart, double timestop);
double minBP(double timestart, double timestop, int sys);
double minHR(double timestart, double timestop);
double maxT(double timestart, double timestop);
double maxBP(double timestart, double timestop, int sys);
double maxHR(double timestart, double timestop);


void makeStatTable(char measurement_type[3], char units[5], double mean, double stdev, double max, double min);
float roundToHundredths(double number);



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




/// Declare a few arrays of data from the structs to hold everything.
struct blood_pressure BP[NUM_BP_MEASUREMENTS];
struct body_temperature BT[NUM_T_MEASUREMENTS];
struct heart_rate HR[NUM_HR_MEASUREMENTS];




/// Handles all user input and output. This is effectively the main menu function.
void main() {



    /// Declare a few arrays of data from the struct to hold everything.
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


            /// Query user for path to folder.
            case 'a' :
                printf("Please enter the location of the input files. Please use a relative path. \n");
                scanf('%s', &path_to_folder);
                readDataIn(path_to_folder);
                folder_loc_set = 1;
                printf("Folder location set to %s \n", path_to_folder);
                break;


            /// Query the user for the time interval of interest.
            case 'b' :
                /// First, make sure that the user has specified the location of the files. Some information from the files is needed for basic input validation.
               if(!folder_loc_set) {
                    printf("Please set the path to the folder before setting a time period for calculations. Use option a) in the previous menu. \n");
               } else {
                    printf("This program will automatically handle any stop time values exceeding the size of the dataset. \n");
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
               if(time_stop == time_start) {
                    printf("Error. Values may not be equal. This is not an interval. \n");
                    time_interval_set = 0;
               }

               if(time_interval_set) {
                    printf("Time interval set successfully! \n");
               } else {
                    printf("Time interval was not set successfully. Rectify errors and try again. \n");
               }
               break;


            /// Query the user for the time and calculate health score.
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

                if(time_for_score_set) {
                    printf("Successfully set time for health score. Calculating health score... \n");
                    /// INSERT HEALTH SCORE CALCULATION HERE
                } else {
                    printf("Failed to set time for calculating health score. Please rectify errors and retry. \n");
                }

                break;



            case 'd' :
                /// Need data to be loaded and also need to know the time interval to calculate the desired statistics.
                if(!folder_loc_set || !time_interval_set) {
                    printf("Error. Must specify the time interval for analysis and specify the location of the input sensor data before calculating statistics. \n");
                } else {
                    printf("STATISTICAL PARAMETERS FOR SPECIFIED INTERVAL:\n");
                    makeStatTable("BP", "mmHg", meanBP(time_start, time_stop, 1), sdBP(time_start, time_stop, 1), maxBP(time_start, time_stop, 1), minBP(time_start, time_stop, 1));
                    printf("(SYSTOLIC MEASUREMENTS) \n"); /// Remember to communicate what KIND of blood pressure.
                    makeStatTable("BP", "mmHg", meanBP(time_start, time_stop, 0), sdBP(time_start, time_stop, 0), maxBP(time_start, time_stop, 0), minBP(time_start, time_stop, 0));
                    printf("(DIASTOLIC MEASUREMENTS)\n");
                    makeStatTable("HR", " BPM", meanHR(time_start, time_stop), sdHR(time_start, time_stop), maxHR(time_start, time_stop), minHR(time_start, time_stop));
                    makeStatTable("BT", "degF", meanT(time_start, time_stop), sdT(time_start, time_stop), maxT(time_start, time_stop), minT(time_start, time_stop));
                    printf("\n");
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


/// Reads the data in from the folder specified by the user.
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



/// Straightforward name: given two time values as a double, we pass these values to a function and have it
/// check all of the time values in the dataset. Function just counts the number of t-values in the range, sums them, then divides
/// by the number that it counts.
double meanT(double timestart, double timestop) {

    float sum = 0;
    float count = 0;  /// Preemptively typing as float. Less efficient, but it saves us from forgetting to cast it as a float later.
    int i;

    for(i = 0; i < NUM_T_MEASUREMENTS; i++ ) {
        /// Check if each t-value is within the specified window.
        if(BT[i].timestamp < timestop && BT[i].timestamp > timestart) {
            sum += BT[i].temp;
            count++;
        }
    }

    return (sum / count);

}



/// Works the same as the above, but with a selector parameter that determines whether it calculates systolic or diastolic pressure means.
/// When sys is nonzero, it calculates systolic. If it is zero, it calculates diastolic pressure mean.
double meanBP(double timestart, double timestop, int sys) {
    float sum = 0;
    float count = 0;
    int i;

    for(i = 0; i < NUM_BP_MEASUREMENTS; i++) {

        if(BP[i].timestamp < timestop && BP[i].timestamp > timestart) {
            if(sys) {
                sum += BP[i].systolic;
                count++;
            } else {
                sum += BP[i].diastolic;
                count++;
            }
        }
    }

    return (sum / count);

}



/// Works more or less identically to meanT.
double meanHR(double timestart, double timestop) {

    float sum = 0;
    float count = 0;
    int i;

    for(i = 0; i < NUM_HR_MEASUREMENTS; i++ ) {
        if(HR[i].timestamp < timestop && HR[i].timestamp > timestart) {
            sum += HR[i].rate;
            count++;
        }
    }

    return (sum / count);

}



/// Calculates and returns the standard deviation of an interval within a temperature data set.
/// Using the formula for corrected sample standard deviation.
double sdT(double timestart, double timestop) {

    double mean = meanT(timestart, timestop);
    double diff_sq = 0;
    float sum_diff_squares = 0;
    float count = 0;
    int i;

    for(i = 0; i < NUM_T_MEASUREMENTS; i++) {
        if(BT[i].timestamp < timestop && BT[i].timestamp > timestart) {
            diff_sq = (mean - BT[i].temp) * (mean - BT[i].temp);
            sum_diff_squares += diff_sq;
            count++;
        }
    }

    return sqrt( sum_diff_squares / (count - 1) );

}



/// Works same way as above, but with a selector.
double sdBP(double timestart, double timestop, int sys) {

    double mean = meanBP(timestart, timestop, sys);
    double diff_sq = 0;
    float sum_diff_squares = 0;
    float count = 0;
    int i;

    for(i = 0; i < NUM_BP_MEASUREMENTS; i++) {
        if(BP[i].timestamp < timestop && BP[i].timestamp > timestart) {

            /// Just add another selector for the blood pressure type.
            if(sys) {
                diff_sq = (mean - BP[i].systolic) * (mean - BP[i].systolic);
            } else {
                diff_sq = (mean - BP[i].diastolic) * (mean - BP[i].diastolic);
            }
            sum_diff_squares += diff_sq;
            count++;
        }
    }

    return sqrt( sum_diff_squares / (count - 1));

}



/// Largely the same as the body temp case.
double sdHR(double timestart, double timestop) {

    double mean = meanHR(timestart, timestop);
    double diff_sq = 0;
    float sum_diff_squares = 0;
    float count = 0;
    int i;

    for(i = 0; i < NUM_HR_MEASUREMENTS; i++) {
        if(HR[i].timestamp < timestop && HR[i].timestamp > timestart) {
            diff_sq = (mean - HR[i].rate) * (mean - HR[i].rate);
            sum_diff_squares += diff_sq;
            count++;
        }
    }

    return sqrt( sum_diff_squares / (count - 1) );

}



/// Finds minimum value in the temperature dataset. Starts by assuming that the first value is the lowest,
/// and then proceeds under this assumption until it finds otherwise.
double minT(double timestart, double timestop) {

    double workingValue = BT[0].temp;
    int i;

    for(i = 1; i < NUM_T_MEASUREMENTS; i++) {

        if(BT[i].temp < workingValue) {
            workingValue = BT[i].temp;
        }
    }

    return workingValue;

}



/// Same as above, but as with all of the other BP calculations, it has a check for systolic/diastolic pressure.
double minBP(double timestart, double timestop, int sys) {

    if(sys) {

        double workingValue = BP[0].systolic;
        int i;

        for(i = 1; i < NUM_BP_MEASUREMENTS; i++) {

            if(BP[i].systolic < workingValue) {
                workingValue = BP[i].systolic;
            }
        }

    } else {

        double workingValue = BP[0].diastolic;
        int i;

        for(i = 1; i < NUM_BP_MEASUREMENTS; i++) {

            if(BP[i].diastolic < workingValue) {
                workingValue = BP[i].diastolic;
            }
        }
    }
}



/// Same as above.
double minHR(double timestart, double timestop) {

    double workingValue = HR[0].rate;
    int i;

    for(i = 1; i < NUM_HR_MEASUREMENTS; i++) {

        if(HR[i].rate < workingValue) {
            workingValue = HR[i].rate;
        }
    }

    return workingValue;

}



/// Just like minT, but with a single changed inequality.
double maxT(double timestart, double timestop) {

    double workingValue = BT[0].temp;
    int i;

    for(i = 1; i < NUM_T_MEASUREMENTS; i++) {

        if(BT[i].temp > workingValue) {
            workingValue = BT[i].temp;
        }
    }

    return workingValue;

}



/// Same as above, but for BP, so it has the same systolic parameter as usual.
double maxBP(double timestart, double timestop, int sys) {

     if(sys) {

        double workingValue = BP[0].systolic;
        int i;

        for(i = 1; i < NUM_BP_MEASUREMENTS; i++) {

            if(BP[i].systolic > workingValue) {
                workingValue = BP[i].systolic;
            }
        }

    } else {

        double workingValue = BP[0].diastolic;
        int i;

        for(i = 1; i < NUM_BP_MEASUREMENTS; i++) {

            if(BP[i].diastolic > workingValue) {
                workingValue = BP[i].diastolic;
            }
        }
    }
}



/// Should be relatively clear.
double maxHR(double timestart, double timestop) {

    double workingValue = HR[0].rate;
    int i;

    for(i = 1; i < NUM_T_MEASUREMENTS; i++) {

        if(HR[i].rate > workingValue) {
            workingValue = BT[i].temp;
        }
    }

    return workingValue;

}



/// Helper function: Deals with creating small, formatted tables of statistical parameters.
/// ASSUMPTION: All values rounded to two decimal places.
void makeStatTable(char measurement_type[3], char units[5], double mean, double stdev, double max, double min) {

    /// First, print table header.
    printf("\n");
    printf("  %s", measurement_type);
    printf("  | (%s", units);
    printf(") \n");

    /// Now, a spacer.
    printf("-------+----- \n");

    /// Then another header and spacer.
    printf(" Param | Val \n");
    printf("-------+----- \n");

    /// Then an entry for mean. Remembering to round.
    float roundedmean = roundToHundredths(mean);
    printf("  Mean | %4.2f \n", roundedmean);

    /// Then stdev.
    float roundedstdev = roundToHundredths(stdev);
    printf("  STDV | %4.2f \n", roundedstdev);

    /// Then the max value.
    float roundedmax = roundToHundredths(max);
    printf("   Max | %4.2f \n", roundedmax);

    /// Then the min value.
    float roundedmin = roundToHundredths(min);
    printf("   Min | %4.2f \n", roundedmin);

    /// Finally, close off the bottom of the table.
    printf("-------+----- \n");


}



/// Helper function to round the statistical values to two decimal places. Calculations were performed as doubles to ensure accuracy
/// while performing the operations. Need this function for final conversion to float.
float roundToHundredths(double number) {
    double intermed = number * 1000;
    int thousandths = ((int) intermed) % 10;
    float rounded;

    if(thousandths >= 5) {
        rounded = ceil(number * 100) / 100;
    } else {
        rounded = floor(number * 100) / 100;
    }

    return rounded;
}


