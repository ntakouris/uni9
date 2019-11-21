ball_mask = imread('ball_mask.jpg');
ball = imread('ball.jpg');
orig_ball_size = size(ball);
orig_ball_size = orig_ball_size(1);

ball_multiscale = zeros(size(ball));
ball_multiscale(:,:,:) = ball;

for scale = 9:1
    ball_scaled = imresize(ball, scale / 10, 'nearest');
    ball_mask_scaled = imresize(ball_mask, scale / 10, 'nearest');
    
    ball_scaled_size = size(ball_scaled);
    ball_scaled_size = ball_scaled_size(1);
    
    start = (orig_ball_size / 2) - (ball_scaled_size / 2);
    
    for x = 0:ball_scaled_size
       for y = 0:ball_scaled_size
          if ball_mask_scaled(x, y) > 0
             ball_multiscale(start+ x, start + y, :) = ball_scaled(x,y,:);
          end
       end
    end
    
end

imshow(ball_multiscale);
imwrite(ball_multiscale, 'ball_multiscale');