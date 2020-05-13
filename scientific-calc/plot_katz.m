figure;
semilogy(0:length(r1)-1,r1/norm(e),'b-o');
hold on;
semilogy(0:length(r2)-1,r2/norm(e),'r-x');
legend('no prec','ichol');

xlabel('iteration number');
ylabel('relative residual');
title("Katz (a = 0.03) for email")

hold off;