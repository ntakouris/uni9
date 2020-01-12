function [X] = multiKatz(A, alpha, mth, pcg_params)
% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
    n = length(A);
    e = ones(n, 1);
    
    
    if mth == "direct"
       tic
       for i = 1:length(alpha)
         X(i, :) = A_ \ e;
       end
       toc
    end
    
    if mth == "pcg"        
        tol = pcg_params{1};
        iter_limit = pcg_params{2};
        if length(pcg_params < 3)
           tic
           for i = 1:length(alpha)
              A_ = eye(n) - alpha(i) * A';
              [sol ,flag,~,~, resvec] = pcg(A_, e, tol, iter_limit);
              X(i, :) = sol;
              
              if flag == 0
                fprintf("Converged for a = %d in", alpha(i))
                residuals = resvec;
                fprintf("%d iterations\n", iter);
              end
           end
           toc
        else
            prec = pcg_params{3};
            if prec == "ichol"

              tic
              for i = 1:length(alpha)
                A_ = eye(n) - alpha(i) * A';
                L = ichol(A_);

                [sol ,flag,~,iter, resvec] = pcg(A_, e, tol, iter_limit, L, L');
                X(i, :) = sol;
                
                if flag == 0
                  fprintf("Converged for a = %d", alpha(i))
                  residuals = resvec;
                  fprintf("%d iterations", iter);
                end
              end

              toc
            end
        end
    end
    
    %xhat = X(:, 1);
    %dist = norm(e - A * xhat, 2);
end