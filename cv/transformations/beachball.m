ball = imread('ball.jpg');
bg = imread('beach.jpg');
mask = imread('ball_mask.jpg');

frame_count = 100;

writerObj = VideoWriter('beach.avi');
writerObj.FrameRate = 24;
open(writerObj);

root = [250 400];
mill_size = size(mill);
mill_size = mill_size(1);

bounce_times = 5;
baseline_y = 100;
baseline_x = 50;

for f = 1:frame_count
    angle = mod(((frame / frame_count) * 360 * bounce_times * 2), 360);
    amplitude = (e^ (-(frame / frame_count) * 4)) * 100; % 100 pixel scale
    
    offset_y = abs(amplitude * cos(((frame / frame_count) * bounce_count * pi)) );
    offset_x = e^((frame / frame_count) / 5) * 300; 
    
    scale = interp1([0,1],[1, 0.1], frame / frame_count);
    
    % rotate by angle
    
    % paste image ontop of other based on mask and offsets
    % f = ..
    
    writeVideo(writerObj, im2frame(f));
end

close(writerObj);
implay('beach.avi');