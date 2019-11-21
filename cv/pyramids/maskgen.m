x = imread('photos/spongebob.jpg');
imshow(x);

hFH = drawfreehand();

M = hFH.createMask();
imwrite(M, 'photos/spongebob_mask.jpg');