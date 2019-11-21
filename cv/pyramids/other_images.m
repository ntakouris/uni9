close all
clear
p200 = im2double(imread('photos/P200.jpg'));
[M N ~] = size(p200);
bench = im2double(imread('photos/bench.jpg'));
bench_m = im2double(imread('photos/bench_mask.jpg'));

cat = im2double(imread('photos/cat.jpg'));
cat_m = im2double(imread('photos/cat_mask.jpg'));

dog1 = im2double(imread('photos/dog1.jpg'));
dog1_m = im2double(imread('photos/dog1_mask.jpg'));

dog2 = im2double(imread('photos/dog2.jpg'));
dog2_m = im2double(imread('photos/dog2_mask.jpg'));

spongebob = im2double(imread('photos/spongebob.jpg'));
spongebob_m = im2double(imread('photos/spongebob_mask.jpg'));

bench = imresize(bench, [M, N]);
cat = imresize(cat, [M, N]);
dog1 = imresize(dog1,[M, N]);
dog2 = imresize(dog2, [M, N]);
spongebob = imresize(spongebob, [M, N]);

bench_m = imresize(bench_m, [M, N]);
cat_m = imresize(cat_m, [M, N]);
dog1_m = imresize(dog1_m, [M, N]);
dog2_m = imresize(dog2_m, [M, N]);
spongebob_m = imresize(spongebob_m, [M, N]);

blurh = fspecial('gauss',30,15); % feather the borders
bench_m = imfilter(bench_m,blurh,'replicate');
cat_m = imfilter(cat_m,blurh,'replicate');
dog1_m = imfilter(dog1_m,blurh,'replicate');
dog2_m = imfilter(dog2_m,blurh,'replicate');
spongebob_m = imfilter(spongebob_m,blurh,'replicate');

v = 230;
level = 5;

p200_p = genPyr(p200,'lap',level);
bench_p = genPyr(bench,'lap',level);
cat_p = genPyr(cat,'lap',level);
dog_1_p = genPyr(dog1,'lap',level);
dog_2_p = genPyr(dog2,'lap',level);
spongebob_p = genPyr(spongebob,'lap',level);

% p200 - bench
limga=p200_p;
limgb=bench_p;

maskb = bench_m;
maska = 1-maskb;

limgo = cell(1,level);
for p = 1:level
	[Mp Np ~] = size(limga{p});
	maskap = imresize(maska,[Mp Np]);
	maskbp = imresize(maskb,[Mp Np]);
	limgo{p} = limga{p}.*maskap + limgb{p}.*maskbp;
end

imgo = pyrReconstruct(limgo);
imgo_p = genPyr(imgo,'lap',level);
figure,imshow(imgo)

% cat
limga=imgo_p;
limgb=cat_p;

maskb = cat_m;
maska = 1-maskb;

limgo = cell(1,level);
for p = 1:level
	[Mp Np ~] = size(limga{p});
	maskap = imresize(maska,[Mp Np]);
	maskbp = imresize(maskb,[Mp Np]);
	limgo{p} = limga{p}.*maskap + limgb{p}.*maskbp;
end

imgo = pyrReconstruct(limgo);
imgo_p = genPyr(imgo,'lap',level);
figure,imshow(imgo)

% dog1
limga=imgo_p;
limgb=dog_1_p;

maskb = dog1_m;
maska = 1-maskb;

limgo = cell(1,level);
for p = 1:level
	[Mp Np ~] = size(limga{p});
	maskap = imresize(maska,[Mp Np]);
	maskbp = imresize(maskb,[Mp Np]);
	limgo{p} = limga{p}.*maskap + limgb{p}.*maskbp;
end

imgo = pyrReconstruct(limgo);
imgo_p = genPyr(imgo,'lap',level);
figure,imshow(imgo)

% dog2
limga=imgo_p;
limgb=dog_2_p;

maskb = dog2_m;
maska = 1-maskb;

limgo = cell(1,level);
for p = 1:level
	[Mp Np ~] = size(limga{p});
	maskap = imresize(maska,[Mp Np]);
	maskbp = imresize(maskb,[Mp Np]);
	limgo{p} = limga{p}.*maskap + limgb{p}.*maskbp;
end

imgo = pyrReconstruct(limgo);
imgo_p = genPyr(imgo,'lap',level);

% spongebob
limga=imgo_p;
limgb=spongebob_p;

maskb = spongebob_m;
maska = 1-maskb;

limgo = cell(1,level);
for p = 1:level
	[Mp Np ~] = size(limga{p});
	maskap = imresize(maska,[Mp Np]);
	maskbp = imresize(maskb,[Mp Np]);
	limgo{p} = limga{p}.*maskap + limgb{p}.*maskbp;
end

imgo = pyrReconstruct(limgo);

figure,imshow(imgo)