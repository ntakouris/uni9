% Author:  T. Ntakouris, AM 1054332, Date : 1/15/2019

figure;
semilogy(0:it1,r1/norm(e),'b-o');
hold on;
semilogy(0:it2,r2/norm(e),'r-x');
legend('no prec','ichol');

xlabel('iteration number');
ylabel('relative residual');
title("Katz (a = 0.03) for email")

hold off;