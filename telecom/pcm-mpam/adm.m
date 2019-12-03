K = 1.5;
DELTA_INIT = 1;

function [y] = adm_encoder(x, M)
    x = interp(x, M);
    y = zeros(size(x));
    delta = zeros(size(x));

    delay = zeros(size(x) - 1);
    
    delta(1) = DELTA_INIT;
    
    x_q = 1;
    if x(1) < DELTA_INIT
        x_q = -1;
    end
        
    y(1) = x_q;
    delay(1) = y(1);
        
    for n = 2:size(x)
        e = x(n) - delay(n-1);
                
        q_e = 1;
        if e - delay(n-1) < delta(n-1)
            q_e = -1;
        end
                
        y(n) = q_e;
        
        if (y(n) == y(n-1))
            delta(n) = delta(n-1) * K;
        else
            delta(n) = delta(n-1) / K;
        end
            
        delay(n) = q_e + delay(n-1);
    end
end

function [y] = adm_decoder(b, N)  
    y = zeros(size(b));
    b = [b 0];
    
    delay = zeros(size(b) + 1);
    
    delta = zeros(b);
    delta(1) = DELTA_INIT;
    
    for n = 1:(size(b) - 1)
        y(n) = b(n) * delta(n) + delay(n+1);
        
        if b(n) == b(n+1)
            delta(n) = delta(n-1) * K;
        else
            delta(n) = delta(n-1) / K;
        end
        
        delay(n+1) = y(n);
    end
    
    y = interp(y, N);
end