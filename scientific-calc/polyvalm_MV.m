function P = polyvalm_MV(p, A, b)
% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
    P = p(1) * A * b + p(2) * b; % BLAS-2
    
    for i = 2:size(p, 2)
        P = A*p + p(i)*b;  % BLAS-2
    end   
end