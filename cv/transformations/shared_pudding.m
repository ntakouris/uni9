pudding = imread('pudding.png');
frame_count = 100;

writerObj = VideoWriter('pudding_shear.avi');
writerObj.FrameRate = 24;
open(writerObj);

for f = 1:frame_count
    shear = sin((f / frame_count) * (2*pi));
    transform = maketform('affine2d',[1 0 0; shear 1 0; 0 0 1]);
    frame = im2frame(imwarp(pudding, transform));
    
    writeVideo(writerObj, frame);
end
transforms = arrayfun(shear_horiz, sin(0:0.1:pi));

close(writerObj);
implay('pudding_shear.avi');