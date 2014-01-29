% This m-file implements the approximate median algorithm for background
% subtraction.  It may be used free of charge for any purpose (commercial
% or otherwise), as long as the author (Seth Benton) is acknowledged.

% Adapted to use VideoReader class instead of aviread by Ajmal Thanikkal

input_file = 'Shopping Mall Wing People (2).mp4'
obj = VideoReader(input_file);

nFrames = obj.NumberOfFrames % Total number of frames

thresh = 5; % The higher the threshold, greater difference needed to classify two pixels as different



bg = read(obj,1);                 % read in 1st frame as background frame
bg_bw = rgb2gray(bg);             % convert background to greyscale


% ----------------------- set frame size variables -----------------------
fr_size = size(bg);             
width = fr_size(2);
height = fr_size(1);
% --------------------- process frames -----------------------------------

fg = zeros(height, width); % Initialize foreground variable
bw_mask = zeros(height, width);  % Binary mask where pixel =1 if part of foreground, 0 else

for i = 2:nFrames

    fr = read(obj, i);
    fr_bw = rgb2gray(fr);       % convert frame to grayscale
    
    fr_diff = abs(double(fr_bw) - double(bg_bw));  % cast operands as double to avoid negative overflow

    for j=1:width                 % if fr_diff > thresh pixel in foreground
         for k=1:height

             if ((fr_diff(k,j) > thresh))
                 fg(k,j) = fr_bw(k,j);
                 bw_mask(k,j) =1;
             else
                 fg(k,j) = 0;
                 bw_mask(k,j) =0;
             end

             if (fr_bw(k,j) > bg_bw(k,j))          
                 bg_bw(k,j) = bg_bw(k,j) + 1;           
             elseif (fr_bw(k,j) < bg_bw(k,j))
                 bg_bw(k,j) = bg_bw(k,j) - 1;     
             end

         end    
    end


    % You can Comment out these 4 lines and write to video instead 
    figure(1),subplot(4,1,1),imshow(fr);   
    subplot(4,1,2),imshow(uint8(bg_bw))
    subplot(4,1,3),imshow(uint8(fg))
    subplot(4,1,4),imshow(bw_mask)

   

end