function T = b2t(A)
% Author: T. NTAKOURIS, AM 1054332, Date : 10/12/2019
    b = 10;
    if nargin == 2
        b = varargin{1};
    end
    m = length(A) / b;
    T = tenzeros([m m b b]);

    for i = 1:m % n x n blocks
        for j = 1:m
            %fprintf("A(%d:%d, %d:%d) -> T(:,:, %d, %d)\n", ((i-1) * m) + 1, i * m,(j-1) * m + 1, j * m , i, j);
            T(:, :, i, j) = A(((i-1) * m) + 1:(i * m), ((j-1) * m) + 1:(j * m));
        end
    end
end