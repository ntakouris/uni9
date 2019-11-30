K = 1.5;
DELTA_INIT = 1;
N = 8000;
M = 10000;

function [y] = adm_encoder(x)
    x = interp(x, M);
    y = zeros(size(x));
    delta = zeros(size(x));

    x = [x 0];
    delay = zeros(size(x));
    
    delta(1) = DELTA_INIT;
    
    x_q = 1;
    if x(1) < 0
        x_q = -1; % 1 bit quant
    end
    
    quantized_x = delta(1) * x_q;
    
    y(1) = quantized_x;
    delay(1) = y(1);
    
    % first pass out of loop to initialize
    
    for n = 1:size(x)
        e = x(n) - delay(n-1);
        
        % step size control (delta(n-1))
        
        q_e = 1;
        if e - delay(n-1) < 0
            q_e = -1;
        end
        
        quantized_x = delta(n-1) * q_e;
        
        y(n) = quantized_x;
        
        if (y(n) == y(n-1))
            delta(n) = delta(n-1) * K;
        else
            delta(n) = delta(n-1) / K;
        end
            
        delay(n) = quantized_x + delay(n-1);
    end
end

function [x_q] = adm_decoder(b)
    x_q = zeros(b);
    delta = zeros(b);
    
    for n = 2:size(b)
        if b(n) == b(n-1) % is this step control logic inverse of encoding?
            delta(n) = delta(n-1) * K;
        else
            delta(n) = delta(n-1) / K;
        end
        
        e = b(n) * delta(n);
        
        x_q(n) = x_q(n-1) + e;
    end
    
    x_q = interp(x_q, N);
end