eps = 0.1;

% for audio
% PCM for 2 - 4 - 8 bits + entropy
% ADM

% SQNR ~ Iterations

% for picture
% PCM for 2 - 4 bits
% SQNR ~ Iterations


function [x_q, centers, D, K_max] = lloyd_max(x, N, x_min, x_max)
    range = x_max - x_min;
    step = int32(range / N);
    
    centers = (x_min + step):step:(x_max - step);
    
    t = zeros(N-1);
    
    d = 0;
    d_prev = 100000;
    iteration = 0;
    
    while 1
        % t in the middle of quant centers
        for i = 1:(size(centers) - 1)
            t(i) = (centers(i) + centers(i+1)) / 2;
        end
        
        % mse
        x_q = arrayfun(@(e) map_to(e, centers, t), x);
        d = mean((x_q - quantized).^2);
        
        if iteration == 1
            D = [d];
        else
            D = [D d];
        end
        
        old_centers = centers;
        
        % expected value of between t
        for i = 2:size(old_centers)
            idx = old_centers >= t(i) && old_centers < t(i+1);
            avg = average(old_centers(idx));
            
            centers(i) = avg;
        end
        
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
       if (s >= t(i))
            val = centers(i - 1);
            break
       end
    end
end