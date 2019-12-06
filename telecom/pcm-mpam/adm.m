K = 1.5;
DELTA_INIT = 1;

in = audioread("speech.wav");
encoded = adm_encoder(in, 2);
decoded = adm_decoder(encoded, 2);

function [y] = adm_encoder(x, M)
    x = interp(x, M);
    y = zeros(size(x));
    delta = zeros(size(x));

    delay = zeros(size(x) - 1); % accumulator
    
    delta(1) = 0.1;
    
    b = 1;
    if x(1) < 0
        b = -1;
    end
        
    y(1) = b;
    delay(1) = y(1) * delta(1);
        
    for n = 2:size(x)
        e = x(n) - delay(n-1);
                
        b = 1;
        if e < 0
            b = -1;
        end
                
        y(n) = b;
        
        delta(n) = delta(n-1) * 1.5;

        if (b ~= y(n-1))
            delta(n) = delta(n-1) / 1.5;
        end
            
        delay(n) = b * delta(n) + delay(n-1);
    end
end

function [y] = adm_decoder(b, N)  
    y = zeros([size(b) 1]);
       
    delta = zeros(size(b));
    delta(1) = 0.1;
    
    y(1) = delta(1);
    if b(1) == -1
        y(1) = -delta(1);
    end
        
    for n = 2:(size(b))
        delta(n) = delta(n-1) * 1.5;

        if b(n) ~= b(n-1)
            delta(n) = delta(n-1) / 1.5;
        end
        
        y(n) = b(n) * delta(n) + y(n-1);
    end
    
    y = y(1:N:end);
end