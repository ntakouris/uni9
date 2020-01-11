make_dir_grayscale('train_seg');
make_dir_grayscale('test_seg');

function [] = make_dir_grayscale(d)
    f = dir(d);
    for k = 3:length(f)
      file = fullfile(f(k).folder, f(k).name);
      img = imread(file);
      colors = size(img, 3);
      
      if colors ~= 1
          img = rgb2gray(img);
      end
      
      imwrite(img, file);
    end
end
