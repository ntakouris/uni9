function [dflag, discrC, discrR] = dd_check(A)
% Author: ?. ?????????, ?? 1054332, Date : 10/12/2019
    dflag = 1;
    discrC = 0;
    discrR = 0;
    
    n = size(A, 1);
    for i = 1:n
        row_sum = sum(abs(A(i, 1:n)));
        col_sum = sum(abs(A(1:1, i)));
        
        central_e = abs(A(i, i));
        r = row_sum - central_e;
        c = col_sum - central_e;
        if r > central_e
           dflag = 0;
           return
        end
        
        discrR = min(central_e - r, discrR);
        discrC = min(central_e - c, discrC);
    end
    
    discrR = max(discrR, 0);
    discrC = max(discrC, 0);
end