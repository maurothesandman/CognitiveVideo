% Original author = Seth Benton, http://www.eetimes.com/document.asp?doc_id=1275604
% Adapted to use VideoReader class instead of aviread by Ajmal Thanik
% 2014-01-30 Changed variables names and separated core tasks in functions, Mauricio Contreras

% The higher the threshold, greater difference needed to classify two pixels as different
function [] = frameDifferenceSave(fileName, threshold, numberOfFiles)

videoReader = VideoReader(fileName); % open file as a video for processing

nFrames = videoReader.NumberOfFrames; % Total number of frames

background = read(videoReader,1); % read in 1st frame as background

for i = 2:nFrames
    currentFrame = read(videoReader, i);
    
    % gets a mask with the pixels that have moved, using the frame
    % difference method
    mask = frameDifferenceMask(currentFrame, background, threshold);
    
    % Set current frame as background model for next iteration
    background = currentFrame;
    
    currentFrameGrayscale = rgb2gray(currentFrame);       % Read and convert current currentFrame to grayscale
    
    blobs = currentFrameGrayscale.*uint8(mask);
    
    if mod(i, nFrames/numberOfFiles) == 0
        disp('saving...')
        imwrite(blobs, srtcat(pwd,num2str(i)),'jpg');
    end
end

disp('frameDifferenceDisplay finished')
end