function [X] = multiKatz(A, alpha, mth, pcg_params)
% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
    n = length(A);
    e = ones(n, 1);
    
    it_n = 0;
    it_r = [];
    
    if mth == "direct"
       tic
       for i = 1:length(alpha)
         A_ = eye(n) - alpha(i) * A';
         sol = A_ \ e;
         
         fprintf("%e error\n", norm(e - A_ * sol, 2))
         
         X(i, :) = sol;
       end
       toc
    end
    
    if mth == "pcg"        
        tol = pcg_params{1};
        iter_limit = pcg_params{2};
        if length(pcg_params) < 3
           tic
           for i = 1:length(alpha)
              A_ = eye(n) - alpha(i) * A';
              [sol ,flag,~,iter, resvec] = pcg(A_, e, tol, iter_limit);
              X(i, :) = sol;
              
              assignin('base','it1',iter);
              assignin('base','r1',resvec);
              
              if flag == 0
                fprintf("Converged for a = %e \n", alpha(i))
                residuals = resvec;
                fprintf("%d iterations\n", iter);
                  fprintf("%e error\n", norm(e - A_ * sol, 2))
                else
                  fprintf("NOT Converged for a = %e\n", alpha(i))
                  fprintf("%e error\n", norm(e - A_ * sol, 2))

                end
           end
           toc
        else
            prec = pcg_params{3};
            if prec == "ichol"

              tic
              for i = 1:length(alpha)
                A_ = eye(n) - alpha(i) * A';
                A_ = sparse(A_);
                L = ichol(A_);

                [sol ,flag,~,iter, resvec] = pcg(A_, e, tol, iter_limit, L, L');
                X(i, :) = sol;
                
                  assignin('base','it2',iter);
                    assignin('base','r2',resvec);
                
                if flag == 0
                  fprintf("Converged for a = %e\n", alpha(i))
                  residuals = resvec;
                  fprintf("%d iterations\n", iter);
                  
                  fprintf("%e error\n", norm(e - A_ * sol, 2))
                else
                  fprintf("NOT Converged for a = %e\n", alpha(i))
                    fprintf("%e error\n", norm(e - A_ * sol, 2))
                end
              end

              toc
            end
        end
    end
end