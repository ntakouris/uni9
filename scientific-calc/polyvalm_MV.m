function P = polyvalm_MV(p, A, b)
% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
    P = p(1) * A * b; % BLAS-2
    
    for i = 2:(size(p, 2)-1)
        P = A(P + p(i) * b);  % BLAS-2
    end   
    
    P = P + p(end) * b; % still BLAS-2
end