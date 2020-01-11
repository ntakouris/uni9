close all;

fprintf("Reading images\n");
train_im = load_from_dir('train', 1);
train_segm = load_from_dir('train_seg', 0);

test_im = load_from_dir('test', 1);
test_segm = load_from_dir('test_seg', 0);


X = zeros([1920*1080*numel(train_im) 3]);
classnames = {'skin', 'noskin'}; % skin = 1, noskin = 0;
Y = zeros([size(X, 1) 1]);

fprintf("Preparing X and Y train\n");
tic
for k = 1:numel(train_segm)
    seg = train_segm{k};
    seg = seg / 255; % 1 or 0
    t = train_im{k};
    
    seg = reshape(seg, [1080*1920 1]);
    t = reshape(t, [1080*1920 3]);
    
    idx = ((k-1) * (1080*1920))+1:(k*1080*1920);
    
    X(idx, :) = t;
    Y(idx) = seg;
end
toc

fprintf("Training\n");
tic
cf = fitcnb(X, Y);
%svm = fitcsvm(X, Y);
toc

% tests
X_test = zeros([1920*1080*numel(test_im) 3]);
pred_segm = {};

fprintf("Testing\n");

X_test = zeros([1920*1080*numel(test_im) 3]);
Y_test = zeros([size(X_test, 1) 1]);

for k = 1:numel(test_segm)
    seg = test_segm{k};
    t = test_im{k};

    seg = reshape(seg, [1080*1920 1]);
    t = reshape(t, [1080*1920 3]);

    idx = ((k-1) * (1080*1920) + 1):(k*1080*1920);

    X_test(idx, :) = t;
    Y_test(idx) = seg;

    %[labels, posterior, cost] = predict(cf, X(idx, :));
    %[labels, posterior, cost] = predict(svm, X(idx, :));

    % tresholding
    %class = posterior(:, 1) < r * posterior(: ,2);

    %Y_Pred(idx) = class;
    %pred_segm{k} = reshape(class, [1080 1920 1]);
    %visualize(reshape(t, [1080 1920 3]), pred_segm{k});
end

% roc
[Y_Pred, scores] = predict(cf, X_test);

[x_,y_,T,AUC] = perfcurve(Y_test ,scores(:,2), 1);

plot(x_,y_,'LineWidth',3)
xlabel('False positive rate')
ylabel('True positive rate')
title('ROC for Classification ')

function imgs = load_from_dir(d, t)    
    f = dir(d);
    imgs = cell([length(f)-2 1]);

    for k = 3:length(f)
      file = fullfile(f(k).folder, f(k).name);
      img = imread(file);
      
      if t == 1
        %img = rgb2hsv(img);
      end
      imgs{k-2} = img;
    end
end

function [] = visualize(img, seg)
    vis = im2double(img);
    %vis = hsv2rgb(img);
    figure
    imshow(vis .* seg)
end