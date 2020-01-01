function [X] = multiKatz(A, alpha, mth, pcg_params)
% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
    n = length(A);
    e = ones(n, 1);
    
    if mth == "direct"
       tic;
       for i = 1:length(alpha)
          X(:, i) = (eye(n) - alpha(i) * A')\e;
       end
       toc;
    end
    
    if mth == "pcg"        
        tol = pcg_params{1};
        iter_limit = pcg_params{2};
        if length(pcg_params < 3)
           tic;
           for i = 1:length(alpha)
              X(:, i) = pcg(eye(n) - alpha(i) * A', e, tol, iter_limit);
           end
           toc;
        else
            prec = pcg_params{3};
            if prec == "ichol"
                M = [];
              tic;
              for i = 1:length(alpha)
                X(:, i) = pcg(eye(n) - alpha(i) * A', e, tol, iter_limit, M);
              end
              toc;
            end
        end
    end
    
    xhat = X(:, 1);
    dist = norm(e - A * xhat, 2);
end