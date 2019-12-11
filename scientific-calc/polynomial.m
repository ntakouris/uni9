
z = [1 14 71 154 120];

target_matrix = A;

x = p(A)\b;

I = eye(size(A));

for i = 1:size(z, 2)
    if i == 1
        right = b;
    else
        right = x(i-1);
    end

    % backslash
    x(i) = (A - p(i) * I) \ right;
    
    % pcg - no precondition
    
    % pcg - jacobi precondition
    
    % cholesky matlab
    
    % cholesky mex
end

function M = p(z)
    M = z ^ 4 + 14 * z ^ 3 + 71 * z ^ 2 + 154 * z + 120;
end