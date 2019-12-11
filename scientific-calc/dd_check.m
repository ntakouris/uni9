function [dflag, discrC, discrR] = dd_check(A)
% Author: ?. ?????????, ?? 1054332, Date : 10/12/2019
    dflag = 1;
    dk = 1;
    discrC = 0;
    discrR = 0;
    
    n = size(A, 1);
    for i = 1:n
        row_sum = sum(abs(A(i, :)));
        col_sum = sum(abs(A(:, i)));
        
        central_e = abs(A(i, i));
        r = row_sum - central_e;
        c = col_sum - central_e;
        if r > central_e || c > central_e
           dflag = 0;
           return
        end
        
        if r == central_e || c == central_e
            dk = 1;
        end
        
        if dk ~= 1
            discrR = min(central_e - r, discrR);
            discrC = min(central_e - c, discrC);
        end
    end
    
    if dk == 1
        discrR = 0;
        discrC = 0;
        return
    end
    
    discrR = max(discrR, 0);
    discrC = max(discrC, 0);
end