eps = 0.001;

% for audio
[y,fs] = audioread('speech.wav');
%sound(y,fs)
audio_min = min(y);
audio_max = max(y);

[x_q, ~, ~, ~, ~] = lloyd_max(y, 8, audio_min, audio_max);
plot(x_q)
%entropy = sum(abs(x_q) .* log(abs(x_q)));
sound(x_q, fs)

% approximate speech by
m = -0.04;
s2 = 0.11;

function [] = show_pcm_image(pcm)
    pcm = 128*pcm + 128;
    y = reshape(pcm, 256, 256);
    imshow(uint8(y));
end

% function [x_q, centers, d] = simple_pcm(x, N, x_min, x_max)
%     range = x_max - x_min;
%     step = int32(range / N);
%     
%     centers = x_min:step:x_max;
%     
%     t = zeros(N);
% 
%     for i = 1:N
%         t(i) = (centers(i) + centers(i+1)) / 2;
%     end
%     
%     x_q = arrayfun(@(e) map_to(e, centers, t), x);
%     d = mean((x - x_q).^2);
% end


function [x_q, centers, D, K_max, S] = lloyd_max(x, N, x_min, x_max)
    range = x_max - x_min;
    step = range / N;
    
    centers = (x_min + (step / 2)):step:(x_max - (step / 2));
    
    d_prev = 100000;
    iteration = 0;
    S = [];
    D = [];
    
    while 1
        % t in the middle of quant centers
        t = zeros(size(centers)-1);
        for i = 1:(size(centers, 2) - 1)
            t(i) = (centers(i) + centers(i+1)) / 2;
        end
                
        % mse
        x_out = x;
        x_out(x_out <= t(1)) = centers(1);
        x_out(x_out >= t(end)) = centers(end);
        
        for i = 1:(size(centers, 2) - 2)
            idx = x_out > t(i) & x_out <= t(i+1);
            x_out(idx) = centers(i);
        end
        
        d = mean((x - x_out).^2);
        
        if iteration == 1
            D = [d];
        else
            D = [D d];
        end
                
        t = [x_min t];
        t = [t x_max];
                
        expected = zeros(size(centers));
        
        for i = 1:(size(centers, 2))
            low = t(i);
            
            high = t(i + 1);
            
            expected(i) = mean(x(x >= low & x < high));
        end
        
        centers = expected;
        
        % SQNR / iter
        p_signal = mean(x_out.^2);
        p_noise = mean((x - x_out).^2);
        sqnr = p_signal / p_noise;
        
        S = [S sqnr];
        
        if (iteration > 1 && d - d_prev > 0)
            break;
        end
        
        x_q = x_out;
        fprintf("Iteration: %d, D: %f\n", iteration, d);
        d_prev = d;
        
        iteration = iteration + 1;
    end
    
    K_max = iteration;
end