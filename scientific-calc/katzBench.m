% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
% load email

method = "direct";
params = {};

a = 0.01:0.01:1;
I = eye(size(email));
e = ones(size(email, 1), 1);

fprintf("Finding which a result to invertible matrix\n");
a = a(det(I - a * email') ~= 0 && (I - a * email') * e > 0);

max_a = max(a);
min_a = min(a);

fprintf("max(a) = %f \nmin(a) = %f\n", max_a, min_a);

% fprintf("multiKatz: a = %f", min_a);
% X_min = multiKatz(email, min_a, method, params);
% fprintf("multiKatz: a = %f", max_a);
% X_max = multiKatz(email, max_a, method, params);
a = [min_a max_a];
X = multiKatz(email, a, method, params);

reps = size(X, 2);

% top 5 nodes
min_x = X(:, 1);
max_x = X(:, end);

maxk_min = maxk(min_x, 5);
maxk_max = maxk(min_x, 5);

top5_min = find(min_x >= maxk_min(end));
top5_max = find(max_x >= maxk_max(end));

