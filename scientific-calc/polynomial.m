% Code for benching serial + explicit (part 3.2)

p = [1 14 71 154 120]';

target_matrix = C_;

e = ones(size(target_matrix, 1), 1);
b = polyvalm_MV(p, target_matrix, e);
% above line same as b = polyval(p, target_matrix) * e;

fprintf("Explicit: \n");
C = polyvalm(p, target_matrix);

tic
x = C \ b; % explicit
toc 

error = norm(e - x, 2);
fprintf("Explicit Error -> %e\n", error);

% as specified on the excercise pdf;
tolerance = 1e-7;
maxreps = 50;

x = zeros(size(b, 1), size(p, 1));
z = roots(p);
I = eye(size(target_matrix));

fprintf("Serial backslash\n");
tic
for i = 1:size(z,1)
   Alpha = (target_matrix - z(i)*I);
   beta = b;
   if i > 1
       beta = x(:, i-1);
   end
   
   % solve Alpha x(i) = x(i-1)
   sol = Alpha \ beta;

   x(:, i) = sol;
   error = norm(e - x(:, i), 2);
   fprintf("Iteration %d: Error -> %e\n", i, error);
end
toc

fprintf("pcg no prec\n");
tic
for i = 1:size(z,1)
   Alpha = (target_matrix - z(i)*I);
   beta = b;
   if i > 1
       beta = x(:, i-1);
   end
   
   % solve Alpha x(i) = x(i-1)
   % pcg - no precondition
   [sol ,flag,relres,iter] = pcg(Alpha, beta, tolerance, maxreps);
   
   x(:, i) = sol;
   error = norm(e - x(:, i), 2);
   fprintf("Iteration %d: Error -> %e\n", i, error);
end
toc

fprintf("pcg block jacobi\n");
tic
for i = 1:size(z,1)
   Alpha = (target_matrix - z(i)*I);
   beta = b;
   if i > 1
       beta = x(:, i-1);
   end
   
   % pcg - block jacobi precondition
   M = diag(diag(Alpha));
   [sol ,flag,relres,iter] = pcg(Alpha, beta, tolerance, maxreps, M);

   x(:, i) = sol;
   error = norm(e - x(:, i), 2);
   fprintf("Iteration %d: Error -> %e\n", i, error);
end
toc
