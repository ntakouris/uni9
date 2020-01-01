function T = b2t(A, m, n)
% Author: T. NTAKOURIS, AM 1054332, Date : 10/12/2019
    T = tenzeros([m m n n]);

    for i = 1:m % n x n blocks
        for j = 1:m
            %fprintf("A(%d:%d, %d:%d) -> T(:,:, %d, %d)\n", ((i-1) * m) + 1, i * m,(j-1) * m + 1, j * m , i, j);
            T(:, :, i, j) = A(((i-1) * m) + 1:(i * m), ((j-1) * m) + 1:(j * m));
        end
    end
end