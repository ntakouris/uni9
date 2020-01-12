a = 0.01;
% fill x1 x2 and a
x1 = [];
y1 = range(size(x1, 1));

x2 = [];
y2 = range(size(x2, 1));

% blue continuous line with o marker and red continuous line with x marker
figure
hold on
title("Katz (a = " + a + ") for email matrix")

xlabel("iteration")
ylabel("norm 2 of error")

plot(x1, y1, 'b-o')
plot(x2, y2, 'r-x')

legend('pcg','pcg+ichol')

hold off