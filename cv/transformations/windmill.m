mill = im2double(imread('windmill.png'));
bg = im2double(imread('windmill_back.jpeg'));
mask = 1 - im2double(imread('windmill_mask.png'));

frame_count = 10;

writerObj = VideoWriter('transf_windmill.avi');
writerObj.FrameRate = 24;
open(writerObj);

mill_size = size(mill);
mill_size = mill_size(1);

method = 'nearest';
for f = 1:frame_count
    fprintf('Proccessing frame %d of %d\n', f, frame_count)
    
    theta = (f / frame_count) * 20;
        
    rot_mill = imrotate(mill, theta, 'nearest', 'crop');
    rot_mask = 1 - imrotate(mask, theta, 'nearest', 'crop');
        
    big_mill = zeros(size(bg));
    big_mask = zeros(size(bg));
    s = size(rot_mill, 1);
    
    root_x = 450 - int32(s/2);
    root_y = 650 - int32(s/2);
    
    for x = 1:s
        for y = 1:s
            if rot_mask(x,y) == 0
                big_mill(root_x + x, root_y + y, :) = rot_mill(x,y,:);
                big_mask(root_x + x, root_y + y, :) = 1;
            end
        end
    end
    
    i = (1-big_mask) .* bg + big_mask .* big_mill;
    i = rescale(i); 
    
    writeVideo(writerObj, im2frame(i));
end

close(writerObj);
implay('transf_windmill.avi');
% save to transf_windmill.mat