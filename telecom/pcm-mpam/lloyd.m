eps = 0.001;

% for audio
[y,fs, A_n] = audioread('speech.wav');

audio_min = min(y);
audio_max = max(y);

pcm_a = zeros(3);
entropy_a = zeros(3);
sqnr_a = zeros(3);

pcm_i = zeros(2);
entropy_i = zeros(2);
sqnr_i = zeros(2);

% PCM for 2 - 4 - 8 bits + entropy
[pcm, ~, ~] = simple_pcm(y, 2, audio_min , audio_max);
entropy = sum(pcm .* log(pcm));

p_signal = mean(pcm.^2);
p_noise = mean((y - pcm).^2);

pcm_a(1) = pcm;
entropy_a(1) = entropy;
sqnr_a(1) = p_signal / p_noise;

[pcm, ~, ~] = simple_pcm(y, 4, audio_min , audio_max);
entropy = sum(pcm .* log(pcm));

p_signal = mean(pcm.^2);
p_noise = mean((y - pcm).^2);

pcm_a(2) = pcm;
entropy_a(2) = entropy;
sqnr(2) = p_signal / p_noise;

[pcm, ~, ~] = simple_pcm(y, 8, audio_min , audio_max);
entropy = sum(pcm .* log(pcm));

p_signal = mean(pcm.^2);
p_noise = mean((y - pcm).^2);

pcm_a(3) = pcm;
entropy_a(3) = entropy;
sqnr(3) = p_signal / p_noise;

% ADM
adm = adm_encoder(y, 10000);
decoded_adm = adm_decode(adm, fs);

% SQNR ~ Iterations
load cameraman.mat
x = i(:); % turn to 1d
x = (x-128)/128; % map to [-1, 1]

% for picture
% PCM for 2 - 4 bits

[pcm, ~, ~] = simple_pcm(y, 2, -1 , 1);
entropy = sum(pcm .* log(pcm));

p_signal = mean(pcm.^2);
p_noise = mean((y - pcm).^2);

pcm_i(1) = pcm;
entropy_i(1) = entropy;
sqnr(1) = p_signal / p_noise;

[pcm, ~, ~] = simple_pcm(y, 4, -1 , 1);
entropy = sum(pcm .* log(pcm));

p_signal = mean(pcm.^2);
p_noise = mean((y - pcm).^2);

pcm_i(2) = pcm;
entropy_i(2) = entropy;
sqnr(2) = p_signal / p_noise;

% SQNR ~ Iterations
% ?? which first shape ?? 

% approximate speech by
m = -0.04;
s2 = 0.11;

function [] = show_pcm_image(pcm)
    pcm = 128*pcm + 128;
    y = reshape(pcm, 256, 256);
    imshow(uint8(y));
end

function [x_q, centers, d] = simple_pcm(x, N, x_min, x_max)
    range = x_max - x_min;
    step = int32(range / N);
    
    centers = x_min:step:x_max;
    
    t = zeros(N);

    for i = 1:N
        t(i) = (centers(i) + centers(i+1)) / 2;
    end
    
    x_q = arrayfun(@(e) map_to(e, centers, t), x);
    d = mean((x - x_q).^2);
end


function [x_q, centers, D, K_max, S] = lloyd_max(x, N, x_min, x_max)
    range = x_max - x_min;
    step = int32(range / N);
    
    centers = x_min:step:x_max;
    
    t = zeros(N);
    
    d_prev = 100000;
    iteration = 0;
    S = [];
    
    while 1
        % t in the middle of quant centers
        for i = 1:(size(centers) - 1)
            t(i) = (centers(i) + centers(i+1)) / 2;
        end
                
        % mse
        x_q = arrayfun(@(e) map_to(e, centers, t), x);
        d = mean((x - x_q).^2);
        
        if iteration == 1
            D = [d];
        else
            D = [D d];
        end
                
        t = [x_min t x_max];
        expected = zeros(size(t) - 2);

        for i = 1:size(expected)
            low = t(i);
            high = t(i + 1);
            
            idx = x >= low && x < high;
            expected(i) = average(x(idx));
        end
        
        new_centers = zeros(size(expected) - 1);
        for i = 1:size(new_centers)
            new_centers(i) = (expected(i) + expected(i+1)) / 2;
        end
        
        centers = [x_min new_centers x_max];
        
        % SQNR / iter
        p_signal = mean(x_q.^2);
        p_noise = mean((x - x_q).^2);
        sqnr = p_signal / p_noise;
        
        S = [S sqnr];
        
        if iteration > 1 && (abs(d-d_prev) < eps)
            break;
        end
        
        d_prev = d;
        
        iteration = iteration + 1;
        fprintf("Iteration: %d, D: %f", iteration, d);
    end
    
    K_max = iteration;
end

function val = map_to(s, centers, t) 
    val = centers(end);
    
    for i = 1:size(t)
       if (s <= t(i))
            val = centers(i);
            break
       end
    end
end