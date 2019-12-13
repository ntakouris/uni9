function [X] = multiKatz(A, alpha, mth, pcg_params)
% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
    n = length(A);
    e = ones(n, 1);
    
    if mth == "direct"
       for i = 1:length(alpha)
          X(:, i) = (eye(n) - alpha(i) * A')\e;
       end
    end
    
    if mth == "pcg"        
        tol = pcg_params{1};
        iter_limit = pcg_params{2};
        if length(pcg_params < 3)
           for i = 1:length(alpha)
              X(:, i) = pcg(eye(n) - alpha(i) * A', e, tol, iter_limit);
           end
        else
            prec = pcg_params{3};
            if length(pcg_params < 4)
               % solve with prec
            else
                prec2 = pcg_params{3};
                % solve with prec and prec2
            end
        end
    end
end