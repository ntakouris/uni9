pudding = imread('pudding.png');
frame_count = 100;

writerObj = VideoWriter('pudding_shear.avi');
writerObj.FrameRate = 24;
open(writerObj);

speed = 4;
ampl = 0.2;

for f = 1:frame_count
    fprintf('Proccessing frame %d of %d\n', f, frame_count)
    shear = ampl * sin((f / frame_count) * (2 * speed *pi));
    
    transform = affine2d([1 0 0; 2 * shear 1 0; 0 0 1]);
    i = imwarp(pudding, transform, 'OutputView', imref2d( size(pudding) ));
    size(i)
    frame = im2frame(i);
    
    writeVideo(writerObj, frame);
end

close(writerObj);
implay('pudding_shear.avi');
% save to shared_pudding.mat