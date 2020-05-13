function P = polyvalm_MV(p, A, b)
    P = p(1) * A * b + p(2) * b; % BLAS-2
    
    for i = 3:size(p, 2)
        P = A*P + p(i)*b;  % Also BLAS-2
    end   
end