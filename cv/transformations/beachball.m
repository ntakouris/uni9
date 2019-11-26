ball = im2double(imread('ball.jpg'));
bg = im2double(imread('beach.jpg'));
mask = 1 - im2double(imread('ball_mask.jpg'));


frame_count = 20;

writerObj = VideoWriter('transf_beach.avi');
writerObj.FrameRate = 24;
open(writerObj);

bounce_times = 5;
baseline_y = 100;
baseline_x = 50;

for f = 1:frame_count
    angle = -mod(((f / frame_count) * 10 * bounce_times * 2), 360);
    amplitude = (exp((-(f / frame_count) * 4))) * 100; % 100 pixel scale
    
    offset_y = abs(amplitude * cos(((f / frame_count) * bounce_times * pi)) );
    offset_x = exp(((f / frame_count) / 5)) * 300; 
    
    scale = interp1([0,1],[1, 0.8], (f / frame_count));
    %%%%%
    
    scale_ball = imresize(ball, scale);
    scale_mask = imresize(mask, scale);
    
    rot_ball = imrotate(scale_ball, angle, 'nearest', 'crop');
    rot_mask = 1 - imrotate(scale_mask, angle, 'nearest', 'crop'); % no need to rotate because it is a circle
    
    big_ball = zeros(size(bg));
    big_mask = zeros(size(bg));
    
    s = size(rot_ball, 1);
     
    root_x = 1000 - int32(s/2);    
    root_y = 750 - int32(s/2);
    
    for x = 1:s
        for y = 1:s
            if rot_mask(x,y) == 0
                big_ball(root_x + x, root_y + y, :) = rot_ball(x,y,:);
                big_mask(root_x + x, root_y + y, :) = 1;
            end
        end
    end
  
    i = (1-big_mask) .* bg + big_mask .* big_ball;
    i = rescale(i);
    imshow(i)
%     
%     writeVideo(writerObj, im2frame(i));
end

close(writerObj);
implay('transf_beach.avi');
% save to transf_beach.mat