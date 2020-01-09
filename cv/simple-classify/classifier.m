train_im = load_from_dir('train');
train_segm = load_from_dir('train_seg');

test_im = load_from_dir('test');
test_segm = load_from_dir('test_seg');


X = zeros([1920*1080*numel(train_im) 3]);
classnames = {'skin', 'noskin'}; % skin = 1, noskin = 0;
Y = zeros([size(X, 1) 1]);

for k = 1:numel(train_segm)
    seg = train_segm{k};
    t = train_im{k};
    
    seg = reshape(seg, [1080*1920 3]);
    t = reshape(t, [1080*1920 3]);
    
    idx = ((k-1) * (1080*1920)):(k*1080*1920);
    
    X(idx) = t;
    Y(idx) = seg;
end

nb = fitcnb(X, Y);
% svm = fitcsvm(X, Y);

for k = 3:length(test_f)
  img = imread(test_f(k).name);
  % img = rgb2hsv(img);
  test_im{k-2} = img;
end

for k = 3:length(test_segm_f)
  img = imread(train_segm_f(k).name);
  test_segm{k-2} = img;
end

Y_Pred = zeros(size(Y));

pred_segm = {};

for k = 1:numel(test_segm)
    seg = test_segm{k};
    t = test_im{k};
    
    seg = reshape(seg, [1080*1920 3]);
    t = reshape(t, [1080*1920 3]);
    
    idx = ((k-1) * (1080*1920)):(k*1080*1920);
    
    X(idx) = t;
    Y(idx) = seg;
    % pred = ...
    % tresholding
    % 
    % t = 0.7 * p_ns / 0.3 * p_ns
    % pred(pred >= t) = 1
    % pred(pred < t) = 0
    % Y_Pred(idx) = pred;
    %pred_segm{k} = reshape(pred_seg, [1080 1920 1]);
    % visualize(reshape(t, [1080 1920 3]), pred_segm{k});
end
% metrics, on Y and Y_Pred
incorrect_px = length(find(Y_Pred ~= Y));
correct_px = length(find(Y_Pred == Y));
total_px = length(Y);

fprintf("% Precision %f", (correct_px - incorrect_px) / total_px);
fprintf("% Accuracy %f", correct_px / total_px);


function imgs = load_from_dir(dir, varargin)    
    convert = 0;
    if nargin == 1
        convert = varargin{1};
    end
    f = dir('train_seg');
    imgs = {};

    for k = 3:length(f)
      img = imread(f(k).name);
      
      if convert == 1
        img = rgb2hsv(img);
      end
      
      imgs{k-2} = img;
    end
end

function [] = visualize(img, seg)
    vis = img;
    %vis = hsv2rgb(img);
    
    vis = vis + vis .* seg * 0.5;
    figure
    imshow(vis)
end