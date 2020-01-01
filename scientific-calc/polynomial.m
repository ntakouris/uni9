% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
% Code for benching serial + explicit (part 3.2)

p = [1 14 71 154 120]';
z = roots(p);

% ensure sparsity
target_matrix = P_100_10;

I = eye(size(target_matrix));
e = ones(size(target_matrix, 1), 1);
% b = polyvalm_MV(p, target_matrix, e);
b = polyval(p, target_matrix) * e;

fprintf("Explicit: \n");
x = [];
tic
x = polyvalm(p, target_matrix) \ b; % explicit
toc

error = norm(e - x, 2);
fprintf("Explicit Error -> %f\n", error);

% as specified on the excercise pdf;
tolerance = 1e-7;
maxreps = 50;

x = zeros(size(b, 1), size(p, 1));

for i = 1:size(z,1)
   Alpha = (target_matrix - z(i)*I);
   beta = b;
   if i > 1
       beta = x(:, i-1);
   end
   
   % solve Alpha x(i) = x(i-1)
   %sol = Alpha \ beta;
   
   % pcg - no precondition
   %[sol ,flag,relres,iter] = pcg(Alpha, beta, tolerance, maxreps);
   
   x(:, 1) = sol;
   error = norm(e - x(:, 1), 2);
   fprintf("Iteration %d: Error -> %f\n", i, error);
end


% fprintf("Serial + %s\n", "backslash");
% err_serial = 0;
% for k = 1:1
%     x = zeros(size(z, 2) - 1, size(target_matrix, 1));
%     for i = 1:(size(z, 2) - 1)
%         if i == 1
%             right = b;
%         else
%             right = x(i-1, :)';
%         end
%         
%         L_A = (target_matrix - p(i) * I);
%         L_b = right;
%         
%         % backslash
%         sol =  L_A \ L_b;
%         
%         % pcg - no precondition
% %         [ans,flag,relres,iter] = pcg(L_A, L_b, tolerance, maxreps);
% %         x(i, :) = ans;
%         
%         % pcg - jacobi precondition
%         % normal jacobi: diag(diag(A))
% %         M = blkdiag(A);
% %         [ans, flag, relres, iter] = pcg(L_A, L_b, tolerance, maxreps, M);
%         
%         % cholesky matlab
%         
%         % cholesky mex
%         
%         x(i, :) = sol';
%         err_serial = sum(abs(e - sol'));
%         fprintf("error: %f\n", err_serial)
%     end
% end