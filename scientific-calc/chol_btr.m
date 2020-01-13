function [L] = chol_btr(A, block_size)
    n = length(A);
    block = @(index) (index-1)*block_size+1:index*block_size;
    L = zeros(n);
    
    % assume tridiagonal
    for i=1:n/block_size
        L(block(i), block(i)) = chol(A(block(i), block(i)));
        
        % for full: L(block(i), i*block_size+1:end) = L(block(i), block(i))'\A(block(i), i*block_size+1:end);
        % for tridiag:
        L(block(i), block(i+1)) = L(block(i), block(i))'\A(block(i), block(i+1));
        
        %A(i*block_size+1:end, i*block_size+1:end) = ...
        %    A(i*block_size+1:end, i*block_size+1:end) - L(block(i), i*block_size+1:end)'*L(block(i), i*block_size+1:end);
    end
end