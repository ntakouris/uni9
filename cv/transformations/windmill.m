mill = imread('windmill.png');
bg = imread('windmill_back.jpeg');
mask = imread('windmill_mask.png');

frame_count = 100;

writerObj = VideoWriter('millspin.avi');
writerObj.FrameRate = 24;
open(writerObj);

root = [250 400];
mill_size = size(mill);
mill_size = mill_size(1);

method = 'nearest';
for f = 1:frame_count
    angle = (frame / frame_count) * 360;
    rot_mill = imrotate(mill, angle, method);
    rot_mask = imrotate(mask, angle, method);
    
    % paste image ontop of other based on mask
    % f = ..
    
    writeVideo(writerObj, im2frame(f));
end

close(writerObj);
implay('millspin.avi');