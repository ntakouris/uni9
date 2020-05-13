function [dflag, discrC, discrR] = dd_check(A)
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
            curr_r = central_e - r;
            curr_c = central_e - c;
            
            if i == 1
                discrR = curr_r;
                discrC = curr_c;
            else
                discrR = min(curr_r, discrR);
                discrC = min(curr_c, discrC);
            end
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