% This m-file implements the currentFrameame difference algorithm for background
% subtraction.  It may be used currentFrameee of charge for any purpose (commercial
% or otherwise), as long as the author (Seth Benton) is acknowledged.

% Adapted to use VideoReader class instead of aviread by Ajmal Thanik
inputFile = 'Shopping Mall Wing People.mp4'
videoReader = VideoReader(inputFile);

nFrames = videoReader.NumberOfFrames % Total number of frames

threshold = 5; % The higher the thresholdold, greater difference needed to classify two pixels as different
fadeStep = 0.01;

background = read(videoReader,1);         % read in 1st currentFrameame as background currentFrameame
backgroundGrayscale = rgb2gray(background);             % convert background to greyscale


% ----------------------- set currentFrameame size variables -----------------------
frameSize = size(background);
width = frameSize(2);
height = frameSize(1);
% --------------------- process currentFrameames -----------------------------------

foreground = zeros(height, width); % Initialize foreground variable
mask = zeros(height, width);  % Binary mask where pixel =1 if part of foreground, 0 else
pastMask = zeros(height, width);

for i = 2:2%nFrames
    
    currentFrame = read(videoReader, i);
    currentFrameGrayscale = rgb2gray(currentFrame);       % Reada and convert current currentFrameame to grayscale
    
    frameDifference = abs(double(currentFrameGrayscale) - double(backgroundGrayscale));  % cast operands as double to avoid negative overflow
    
    for j=1:width                 % if frameDifference > threshold,  pixel in foreground
        for k=1:height
            if ((frameDifference(k,j) > threshold))
                mask(k,j) = 1;
            else
                if pastMask(k,j) > fadeStep
                    mask(k,j) = pastMask(k,j) - fadeStep;
                else
                    mask(k,j) = 0;
                end
            end
        end
    end
    
    % Set current frame as background model for next iteration
    backgroundGrayscale = currentFrameGrayscale;
    pastMask = mask;
    
    figure(1)
    subplot(2,1,1),imshow(currentFrame);colormap(jet);
    subplot(2,1,2),imshow(mask);%[0 1] based, unless one specifies otherwise
    
%     if mod(i, 50) == 0
%         disp('saving')%'%disp(srtcat('saving', num2str(i+10)))
%         imwrite(mask, 'hola', 'jpg');
%     end
    
end

disp('finished')