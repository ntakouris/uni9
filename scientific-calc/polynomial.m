
z = [1 14 71 154 120];

% ensure sparsity
target_matrix = P_100_10;

e = ones(size(target_matrix, 1), 1);
b = polyvalm_MV(z, target_matrix, e);

fprintf("Explicit: \n");
% x = [];
% for i = 1:20
%     tic
%     x = p(target_matrix)\b; % explicit
%     toc
% end
% 
% err_explicit = sum(abs(e - x));

I = eye(size(target_matrix));

tolerance = 1e-7;
maxreps = 50;

fprintf("Serial + %s\n", "backslash");
err_serial = 0;
for k = 1:1
    x = zeros(size(z, 2) - 1, size(target_matrix, 1));
    sol = 0;
    for i = 1:(size(z, 2) - 1)
        if i == 1
            right = b;
        else
            right = x(i-1, :)';
        end
        
        L_A = (target_matrix - p(i) * I);
        L_b = right;
        
        %fprintf("iter %d : x(%d, %d) = L_A(%d, %d) \\ L_b(%d, %d)\n", i, size(x), size(L_A), size(L_b));
        % backslash
        %sol =  L_A \ L_b;

        % pcg - no precondition
         [ans, flag, relres, iter] = pcg(L_A, L_b, tolerance, maxreps);
         sol = ans;
        
        % pcg - jacobi precondition
        % normal jacobi: diag(diag(A))
%         M = blkdiag(A);
%         [ans, flag, relres, iter] = pcg(L_A, L_b, tolerance, maxreps, M);
        
        % cholesky matlab
        
        % cholesky mex
        
        x(i, :) = sol(:);
        err_serial = sum(abs(e - sol(:)));
        fprintf("iter: %d error: %f\n", i, err_serial)
    end
end


function M = p(z)
    M = z ^ 4 + 14 * z ^ 3 + 71 * z ^ 2 + 154 * z + 120;
end