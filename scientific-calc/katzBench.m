% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
% load email

a = 0.01:0.01:1;
I = eye(size(email));

a = a(det(I - a * email') <= 0);

X = multiKatz(email, a, "direct", {});

reps = size(X, 2);

% top 5 nodes
min_x = X(:, 1);
max_x = X(:, end);

maxk_min = maxk(min_x, 5);
maxk_max = maxk(min_x, 5);

top5_min = find(min_x >= maxk_min(end));
top5_max = find(max_x >= maxk_max(end));