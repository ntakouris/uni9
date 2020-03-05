function P = polyvalm_MV(p, A, b)
% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
    P = p(1) * A * b + p(2) * b; % BLAS-2
    
    for i = 3:size(p, 2)
        P = A*P + p(i)*b;  % Also BLAS-2
    end   
end