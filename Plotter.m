% Written by Ian Kampine and Fahir Nasir on Matlab R2017b
% Matlab Portion of CS1324 Final Project, Fall 2017

% Program will load in the data files. Each data type will be given its own
% array. Each array will contain one matrix per data file read in. 
% Row 1 will be the timestamps associated with each measurement.
% Rows 2 and 3, if applicable, will contain the measurements. Program will then
% calculate the mean values for a given dataset. All data will be plotted first.
% Then, four plots of the mean values for each dataset
% will be generated: Heart Rate, Body Temperature, Diastolic Blood
% Pressure, and Systolic Blood Pressure. Each data series will be plotted
% as separate lines on each graph. 


%%%%%%%%%%%%%%%%%%%%%%%%%% DATA INPUT BLOCK %%%%%%%%%%%%%%%%%%%%%%%%%%%

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
                
                % Same as above, but with one more column of data.
                inputBuffer = cell2mat( textscan(current_file, '%f\t%f\t%f') );
                BP_data{current_num}(:, :) = inputBuffer;
            
        end
                     
    end
    
end

% Close all the files we just opened.
fclose('all');

% Merge and average the data sets.
BT_combined = combineDataSet(BT_data, 0);
BP_combined = combineDataSet(BP_data, 1);
HR_combined = combineDataSet(HR_data, 0);
BT_averaged = averageData(BT_combined, 0);
BP_averaged = averageData(BP_combined, 1);
HR_averaged = averageData(HR_combined, 0);

%%%%%%%%%%%%%%%%%%%%%%%%% END DATA INPUT BLOCK %%%%%%%%%%%%%%%%%%%%%%%

% Combines the data sets passed to it. Sums recurring entries.
% Also notes the number of times each value occurs.
function q = combineDataSet(data_array, isbp)
    
    % Populate the combined dataset with the first data file.
    combined_dataset = data_array{1}(:, :);
    
    % Append an extra column. This will be used for keeping track of how
    % many times a given time-value repeats.
    size_combined = size(combined_dataset);
    combined_dataset = [combined_dataset, ones(size_combined(1), 1) ];
    
    % Load each subsequent data file into a temporary array and start
    % comparing its time values to those in the existing combined array.
    for cell_entry = [2 : 5]
        
        size_combined = size(combined_dataset);
        working_mat = data_array{cell_entry}(:, :);
        size_working = size(working_mat);
            
            for working_row_index = [1 : size_working(1)]
                
                working_row = working_mat(working_row_index, :);
                
                % If a time-value recurs, add the value of the measurement
                % to the existing value in the combined dataset and then
                % increment the counter column for that entry. If it
                % doesn't, then append a zero entry to the row and add it
                % to the combined dataset.
                [repeated_row, ~] = find(combined_dataset == working_row(1));
                
                if ~isempty(repeated_row)
                    
                    combined_dataset(repeated_row, 2) = combined_dataset(repeated_row, 2) + working_row(2);
                    
                    % Need to account for the slightly different structure
                    % that blood pressure data will have. Namely, two
                    % measurements. This also changes the index of the
                    % count column.
                    if isbp ~= 0
                        
                        combined_dataset(repeated_row, 3) = combined_dataset(repeated_row, 3) + working_row(3);
                        combined_dataset(repeated_row, 4) = combined_dataset(repeated_row, 4) + 1;
                        
                    else
                        
                        combined_dataset(repeated_row, 3) = combined_dataset(repeated_row, 3) + 1;
                        
                    end
                    
                else
                    
                    row_to_add = [working_row, 1];
                    combined_dataset = [combined_dataset; row_to_add];
                
                end                   
            end
    end
    
    q = combined_dataset;
    
end




% Takes an aggregate data set and replaces the measured values with the
% calculated averages of the measurements at a given time.
function q = averageData(input_set, isbp)
    
    input_dim = size(input_set);
    input_rows = input_dim(1);
    input_cols = input_dim(2);
    for working_row = [1 : input_rows]
        
        input_set(working_row, input_cols - 1) = input_set(working_row, input_cols - 1) / input_set(working_row, input_cols);
        
        % As usual, we have a special case for blood pressure data.
        if isbp ~= 0
            input_set(working_row, input_cols - 2) = input_set(working_row, input_cols - 2) / input_set(working_row, input_cols);
        end

    end
       
    q = input_set;
    
end

