apple = im2double(imread('photos/apple.jpg'));
applelap = genPyr(apple,'lap',2);
applegauss = genPyr(apple,'gauss',2);

orange = im2double(imread('photos/orange.jpg'));
orangelap = genPyr(orange,'lap',2);
orangegauss = genPyr(orange,'gauss',2);

figure, imshow(applegauss{1,1})
figure, imshow(applegauss{1,2})
figure, imshow(orangegauss{1,1})
figure, imshow(orangegauss{1,2})

figure, imshow(applelap{1,1})
figure, imshow(applelap{1,2})
figure, imshow(orangelap{1,1})
figure, imshow(orangelap{1,2})

