function P = polyvalm_MV(p, A, b)

    pol = zeros(size(A)) + p(1);
    
    for i = size(p, 2):2
        pol = pol + p(i) * pow(A, i);
    end
    
    P = pol * b;
end