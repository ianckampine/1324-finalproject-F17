% Written by Ian Kampine and Fahir Nasir on Matlab R2017b
% Matlab Portion of CS1324 Final Project, Fall 2017

% Program will load in the data files. Each data type will be given its own
% array. Each array will contain one matrix per data file read in. 
% Row 1 will be the timestamps associated with each measurement.
% Rows 2 and 3, if applicable, will contain the measurements. Four plots
% will be generated: Heart Rate, Body Temperature, Diastolic Blood
% Pressure, and Systolic Blood Pressure. Each data series will be plotted
% as separate lines on each graph. 


%%%%%%%%%%% DATA INPUT BLOCK %%%%%%%%%%%%%%

% Get the directory containing the files from the user.
% working_dir = input('Please enter the location of the directory containing the files to read in.', 's');
working_dir = "C:/Users/ickampine/Documents/GitHub/1324-finalproject-F17/DataFiles";

% Set up basic information for the read-in loop.
file_types = ["HR", "BT", "BP"];
file_nums = [1 : 5];


% Data for each sensor type is stored in a five-element cell array whose entries are
% themselves matrices of the data in each individual file. Initializing
% these arrays below.
BT_data = cell(1, 5);
BP_data = cell(1, 5);
HR_data = cell(1, 5);



% Read-in loop.
for current_type = file_types
    
    % Append the sensor data type to the path supplied by the user.
    file_dataType_to_open = strcat(working_dir, '/', current_type, '_');
    
    for current_num = file_nums
        
        % Append the file number to the sensor data file type.
        file_to_open = strcat(file_dataType_to_open, num2str(current_num), '.txt');
        
        % Then open it.
        current_file = fopen(file_to_open);
        
        % Determines where to write the file data based upon the current
        % type of sensor data being read in.
        switch current_type
            
            case 'BT'
                
                % Read in the first two columns. Ignore the units by matching
                % them directly with textscan.
                inputBuffer = cell2mat( textscan(current_file, '%f\t%f\tmW') );
            
                % Then convert the buffer to a matrix for ease of use and jam it in the
                % appropriate data set.
                BT_data{current_num}(:, :) = inputBuffer;
            
            case 'HR'
                
                % Works identically to the BT case.
                inputBuffer = cell2mat( textscan(current_file, '%f\t%f\tper min') );
                HR_data{current_num}(:, :) = inputBuffer;
                
            case 'BP'
                
                inputBuffer = cell2mat( textscan(current_file, '%f\t%f\t%f\t') );
                BP_data{current_num}(:, :) = inputBuffer;
            
        end
                     
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%% END DATA INPUT BLOCK %%%%%%%%%%%%%%%%%%%%%%%


