function P = polyvalm_MV(p, A, b)

    P = zeros(size(A, 1), 1) + p(end);
    
    for i = size(p, 2):2
        P = P + (p(i) * pow(A, size(p, 2) - i)) * b; % blas-2
    end    
end