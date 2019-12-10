function P = mask_band(n, type, p, q)
% Author: ?. ????????? ,?? 1054332  ,Date : 10/12/2019    
    if type == 'band'
        P = ones(n);
        % q = upper bandw
        % p = lower bandw
        if ~exist('q','var')
            q = p;
        end
        for i = 1:n
            low_b = i - p;
            if (low_b >= 1)
                P(i, 1:low_b) = 0;
            end
            
            high_b = i + q;
            if (high_b <= n)
                P(i, high_b:n) = 0;
            end
        end
    end
    
    if type == 'btdr'
        Psize = n;
        r = 0;
        if n < p
            Psize = p;
        else
            r = mod(n, p);
        end
        
        P = zeros(Psize + r);
        
        for i = 1:p:Psize
            block_end = i + p - 1;
            P(i:block_end, i:block_end) = 1;
        end
        
        if r ~= 0
            P(Psize:end, Psize:end) = 1;
        end
    end
end