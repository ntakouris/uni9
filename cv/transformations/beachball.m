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
    perc = f / frame_count;
    angle = -mod((perc * 10 * bounce_times * 2), 360);
    
    scale = interp1([0 1], [0.3 0.02], perc);
    
    ball_size = scale * 800;
    amplitude = interp1([0,1], [200, 0], perc);
    bounce = abs(amplitude * cos((perc * bounce_times * pi)));
    
    offset_x = int32(interp1([0,1], [300, 400], perc) + bounce);
    offset_y = int32(interp1([0,1], [100, 1300], perc));
    
    scale_ball = imresize(ball, scale);
    scale_mask = imresize(mask, scale);
    
    rot_ball = imrotate(scale_ball, angle);
    rot_mask = imrotate(scale_mask, angle); % no need to rotate because it is a circle actually
    
    big_ball = zeros(size(bg));
    big_mask = zeros(size(bg));
    
    s = size(rot_ball, 1);
    
    fprintf("ball end dims: %d %d\n", offset_x + s, offset_y + s);
    
    big_mask(offset_x:(offset_x + s - 1), offset_y:(offset_y + s - 1), :) = rot_mask;
    big_ball(offset_x:(offset_x + s - 1), offset_y:(offset_y + s - 1), :) = rot_ball;
  
    i = (1-big_mask) .* bg + big_mask .* big_ball;
    i = rescale(i);
    imshow(i)
     
     writeVideo(writerObj, im2frame(i));
end

close(writerObj);
implay('transf_beach.avi');
