% Author: T. NTAKOURIS, AM 1054332, Date : 12/12/2019
% load email
tolerance = 1e-7;
maxreps = 50;

a = 0.01:0.01:1;
I = eye(size(email));
e = ones(size(email, 1), 1);

% find min and max a that makes sense
fprintf("Finding which a result to invertible matrix\n");
tic
a = a(det(I - a * email') ~= 0 & (I - a * email') * e > 0);
toc

max_a = max(a);
min_a = min(a);

fprintf("max(a) = %f \nmin(a) = %f\n", max_a, min_a);

a = [min_a max_a];

method = "direct";
params = {};
X = multiKatz(email, a, method, params);
fprintf("explicit (mina) ");
show_top5(X(1, :));
fprintf("explicit (maxa) ");
show_top5(X(end, :));

method = "pcg";
params = {tolerance, maxreps};
X = multiKatz(email, a, method, params);
fprintf("pcg no prec (mina) ");
show_top5(X(1, :));
fprintf("pcg no prec (maxa) ");
show_top5(X(end, :));

method = "pcg";
params = {tolerance, maxreps, "ichol"};
X = multiKatz(email, a, method, params);
fprintf("pcg ichol prec (mina) ");
show_top5(X(1, :));
fprintf("pcg ichol prec (maxa) ");
show_top5(X(end, :));

function [] = show_top5(x)
    m = maxk(x, 5);
    top5 = find(x >= m(end));
    top5 = sort(top5, 'descend');
    
    fprintf("top5: %d \n", top5);
end
