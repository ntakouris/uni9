% Author: T. Ntakouris - AM 1054332

email = zeros(1133);
fid = fopen('out.arenas-email');
tline = fgetl(fid);
while ischar(tline)
    elems = split(tline, " ");
    from = str2num(elems{1});
    to = str2num(elems{2});
    
    email(from, to) = 1;
    tline = fgetl(fid);
end
fclose(fid);

% 
% % Production of the matrices required at part 3.1
% N = 1000;
% theta = 1/2;
% 
% A = zeros(N);
% 
% for i = 1:N
%     for j = 1:N
%        A(i, j) = (i - j)^2;
%     end
%     A(i,i) = 1 + i^theta;
% end
% 
% [dflag, discrC, discrR]= dd_check(A);
% fprintf("cond(A) = %d, condest(A) = %d, dflag = %d, discrC = %d, discrR = %d\n", cond(A), condest(A), dflag, discrC, discrR);
% 
% % NM = [[100 10]; [10 100]; [100 100]];
% 
% P_100_10 = gen_P(100, 10);
% P_10_100 = gen_P(10, 100);
% P_100_100 = gen_P(100, 100);
% 
% [dflag, discrC, discrR]= dd_check(P_100_10);
% fprintf("cond(P_100_10) = %d, condest(P_100_10) = %d, dflag = %d, discrC = %d, discrR = %d\n", cond(P_100_10), condest(P_100_10), dflag, discrC, discrR);
% 
% [dflag, discrC, discrR]= dd_check(P_10_100);
% fprintf("cond(P_10_100) = %d, condest(P_10_100) = %d, dflag = %d, discrC = %d, discrR = %d\n", cond(P_10_100), condest(P_10_100), dflag, discrC, discrR);

% takes some half-minute
%[dflag, discrC, discrR]= dd_check(P_100_100);
%fprintf("cond(P_100_100) = %d, condest(P_100_100) = %d, dflag = %d, discrC = %d, discrR = %d\n", cond(P_100_100), condest(P_100_100), dflag, discrC, discrR);

function P = gen_P(n, m)
    I_n = eye(n);
    I_m = eye(m);
    
    T_m = toeplitz([30 -16 1 zeros(1,m-3)]);
    S_n = toeplitz([24 -12 zeros(1,n-2)]);
    
    P = sparse(kron(I_n, T_m) + kron(S_n, I_m));
end