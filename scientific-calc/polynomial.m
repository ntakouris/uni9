% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
% Code for benching serial + explicit (part 3.2)

p = [1 14 71 154 120]';

% ensure sparsity
target_matrix = P_100_10;
% target_matrix = sparse(target_matrix); if not already sparse

e = ones(size(target_matrix, 1), 1);
b = polyvalm_MV(p, target_matrix, e);
% above line same as b = polyval(p, target_matrix) * e;

fprintf("Explicit: \n");
C = polyvalm(p, A);

tic
x = C \ b; % explicit
toc 

error = norm(e - x, 2);
fprintf("Explicit Error -> %f\n", error);

% as specified on the excercise pdf;
tolerance = 1e-7;
maxreps = 50;

x = zeros(size(b, 1), size(p, 1));
z = roots(p);
I = eye(size(target_matrix));

tic
for i = 1:size(z,1)
   Alpha = (target_matrix - z(i)*I);
   beta = b;
   if i > 1
       beta = x(:, i-1);
   end
   
   % solve Alpha x(i) = x(i-1)
   sol = Alpha \ beta;
   
   % pcg - no precondition
   %[sol ,flag,relres,iter] = pcg(Alpha, beta, tolerance, maxreps);
   
   % pcg - block jacobi precondition
   %[sol ,flag,relres,iter] = pcg(Alpha, beta, tolerance, maxreps);
   
   % block cholesky matlab
   
   % block cholesky mex
   
   x(:, i) = sol;
   error = norm(e - x(:, i), 2);
   fprintf("Iteration %d: Error -> %f\n", i, error);
end
toc

fprintf("pcg: \n");

tic;[x ,~,~,~] = pcg(target_matrix, b, tolerance, maxreps);toc

error = norm(e - x, 2);
fprintf("pcg Error -> %f\n", error);

return;
fprintf("pcg (block jacobi): \n");

M = [];
tic;[x ,~,~,~] = pcg(target_matrix, b, tolerance, maxreps, M);toc

error = norm(e - x, 2);
fprintf("pcg (block jacobi) Error -> %f\n", error);

return;
fprintf("chol_btr: \n");

tic;toc;

error = norm(e - x, 2);
fprintf("chol_btr Error -> %f\n", error);

return;
fprintf("chol_btr_mex: \n");

tic;toc;

error = norm(e - x, 2);
fprintf("chol_btr_mex Error -> %f\n", error);