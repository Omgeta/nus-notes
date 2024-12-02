function [U, S, V] = autosvd(A, skipNormalization)
% AUTOSVD Symbolic Singular Value Decomposition (SVD) of a matrix A.
% [U, S, V] = AUTOSVD(A, skipNormalization) computes the symbolic SVD of A
% such that A = U * S * V', where:
%   U - Orthogonal matrix of left singular vectors (m x m)
%   S - Diagonal matrix of singular values (m x n)
%   V - Orthogonal matrix of right singular vectors (n x n)
%
% Parameters:
%   A - Input matrix (symbolic or numeric)
%   skipNormalization - Boolean flag to skip normalization in Gram-Schmidt (default: false)
%
% Output:
%   U - Symbolic orthogonal matrix of left singular vectors
%   S - Symbolic diagonal matrix of singular values (matching A's size)
%   V - Symbolic orthogonal matrix of right singular vectors

    % Ensure input is symbolic
    if ~isa(A, 'sym')
        A = sym(A);
    end

    % Set default for skipNormalization if not provided
    if nargin < 2
        skipNormalization = false;
    end

    % Get the size of A
    [m, n] = size(A);

    % Compute A' * A (for V) and A * A' (for U)
    AtA = A.' * A; % A transpose times A
    AAt = A * A.'; % A times A transpose

    % Compute eigenvalues and eigenvectors for A' * A (right singular vectors V)
    [V, sigma2] = eig(AtA);
    sigma = sqrt(diag(sigma2)); % Singular values (sqrt of eigenvalues)

    % Sort singular values and reorder V accordingly
    [sigma, sortIdx] = sort(sigma, 'descend');
    V = V(:, sortIdx);

    % Compute left singular vectors U from V and A
    U = sym(zeros(m, m)); % Initialize U
    S = sym(zeros(m, n)); % Initialize S with correct dimensions

    for i = 1:length(sigma)
        % Compute the i-th left singular vector as A * v_i / sigma_i
        if sigma(i) ~= 0
            U(:, i) = A * V(:, i) / sigma(i);
            S(i, i) = sigma(i); % Place singular value in S
        else
            U(:, i) = sym(zeros(m, 1)); % Handle zero singular value
        end
    end

    % Handle zero columns in U by adding orthogonal vectors
    for i = 1:m
        if all(U(:, i) == 0) % Check if the column is all zeros
            % Generate a new orthogonal vector
            new_vec = sym(rand(m, 1)); % Start with a random symbolic vector
            for j = 1:i-1
                % Subtract projection onto existing vectors in U
                new_vec = new_vec - (dot(U(:, j), new_vec) / dot(U(:, j), U(:, j))) * U(:, j);
            end
            % Normalize the new vector or scale to integers
            norm_new_vec = sqrt(dot(new_vec, new_vec));
            if skipNormalization
                new_vec = simplify(new_vec); % Simplify entries
                denom = arrayfun(@(x) feval(symengine, 'denom', x), new_vec);
                lcm_denom = lcm(denom);
                new_vec = lcm_denom * new_vec; % Scale to integers
            elseif norm_new_vec ~= 0
                new_vec = new_vec / norm_new_vec; % Normalize
            end
            U(:, i) = new_vec; % Assign the new orthogonal vector
        end
    end

    % Orthonormalize U and V if skipNormalization is false
    U = gram(U, skipNormalization);
    V = gram(V, skipNormalization);

    % If skipping normalization, simplify and scale columns to integers
    if skipNormalization
        % Process U
        for j = 1:size(U, 2)
            col = U(:, j);
            col = simplify(col);
            denom = arrayfun(@(x) feval(symengine, 'denom', x), col); % Extract denominators
            lcm_denom = lcm(denom); % Compute LCM of denominators
            U(:, j) = lcm_denom * col; % Scale column to make entries integers
        end

        % Process V
        for j = 1:size(V, 2)
            col = V(:, j);
            col = simplify(col);
            denom = arrayfun(@(x) feval(symengine, 'denom', x), col); % Extract denominators
            lcm_denom = lcm(denom); % Compute LCM of denominators
            V(:, j) = lcm_denom * col; % Scale column to make entries integers
        end
    end
end
