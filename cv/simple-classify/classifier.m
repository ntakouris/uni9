fprintf("Reading images\n");
train_im = load_from_dir('train');
train_segm = load_from_dir('train_seg');

test_im = load_from_dir('test');
test_segm = load_from_dir('test_seg');


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
% svm = fitcsvm(X, Y);
toc

Y_Pred = zeros(size(Y));

pred_segm = {};

fprintf("Testing\n");
tic
for k = 1:numel(test_segm)
    seg = test_segm{k};
    t = test_im{k};
    
    seg = reshape(seg, [1080*1920 1]);
    t = reshape(t, [1080*1920 3]);
    
    idx = ((k-1) * (1080*1920) + 1):(k*1080*1920);
    
    X(idx, :) = t;
    Y(idx) = seg;
    labels = predict(cf, X(idx, :));
    
    % tresholding
    Y_Pred(idx) = labels;
    pred_segm{k} = reshape(labels, [1080 1920 1]);
    %visualize(reshape(t, [1080 1920 3]), pred_segm{k});
end
toc

fprintf("\n\nMetrics:\n\n");
truepos = length(find(Y_Pred == 1 & Y == 1));
falsepos = length(find(Y_Pred == 1 & Y == 0));
falseneg = length(find(Y_Pred == 0 & Y == 1));
trueneg = length(find(Y_Pred == 0 & Y == 0));

total = length(Y);

accuracy = (truepos + trueneg) / total;
precision = truepos / (truepos + falsepos);
recall = truepos / (truepos + falseneg);
f1 = 2 * (precision * recall) / (precision + recall);

fprintf("Precision %f\n", precision);
fprintf("Accuracy %f\n", accuracy);
fprintf("Recall %f\n", recall);
fprintf("F1 %f\n", f1);


function imgs = load_from_dir(d)    
    f = dir(d);
    imgs = cell([length(f)-2 1]);

    for k = 3:length(f)
      file = fullfile(f(k).folder, f(k).name);
      img = imread(file);
      
      % img = rgb2hsv(img);
      
      imgs{k-2} = img;
    end
end

function [] = visualize(img, seg)
    vis = im2double(img);
    % vis = hsv2rgb(img);
    figure
    imshow(vis .* seg)
end