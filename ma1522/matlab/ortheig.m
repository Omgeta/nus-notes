function [Q, D] = orthogonalDiagonalizationSymbolic(A, skipNormalization)
% ORTHOGONALDIAGONALIZATIONSYMBOLIC Symbolic orthogonal diagonalization of a symmetric matrix.
% [Q, D] = ORTHOGONALDIAGONALIZATIONSYMBOLIC(A, skipNormalization) computes the symbolic orthogonal matrix Q
% and diagonal matrix D such that A = Q * D * Q', where A is symmetric.
% 
% Parameters:
%   A - Symmetric matrix (symbolic or numeric)
%   skipNormalization - Boolean flag to skip normalization in Gram-Schmidt (default: false)
%
% Output:
%   Q - Symbolic orthogonal matrix
%   D - Symbolic diagonal matrix of eigenvalues

    % Ensure the input matrix is symbolic
    if ~isa(A, 'sym')
        A = sym(A);
    end

    % Set default for skipNormalization if not provided
    if nargin < 2
        skipNormalization = false;
    end

    % Check if the matrix is symmetric
    if ~isequal(A, A.')
        error('Matrix A must be symmetric.');
    end

    % Compute eigenvalues and eigenvectors symbolically
    [V, eigenvalues] = eig(A); % Eigenvalues as a diagonal matrix
    eigenvalues = diag(eigenvalues); % Convert diagonal matrix to vector

    % Sort eigenvalues and rearrange eigenvectors accordingly
    [sortedEigenvalues, sortIdx] = sort(eigenvalues, 'ascend');
    V = V(:, sortIdx); % Rearrange eigenvectors to match sorted eigenvalues

    % Initialize orthogonal matrix Q
    Q = sym([]);

    % Group eigenvectors by eigenvalues (handle eigenspaces)
    unique_eigenvalues = unique(sortedEigenvalues); % Unique eigenvalues

    for i = 1:length(unique_eigenvalues)
        % Find eigenvectors corresponding to the current eigenvalue
        eigval = unique_eigenvalues(i);
        eigspace_indices = find(sortedEigenvalues == eigval); % Indices of current eigenspace
        eigspace = V(:, eigspace_indices); % Eigenvectors for the current eigenspace

        % Orthonormalize (or only orthogonalize if skipNormalization is true)
        Q_eigspace = gram(eigspace, skipNormalization); % Use Gram-Schmidt

        % If skipping normalization, simplify and scale columns to integers
        if skipNormalization
            for j = 1:size(Q_eigspace, 2)
                % Simplify the column
                col = Q_eigspace(:, j);
                col = simplify(col);

                % Scale to integers by multiplying by the LCM of denominators
                denom = arrayfun(@(x) feval(symengine, 'denom', x), col); % Extract denominators
                lcm_denom = lcm(denom); % Compute LCM of denominators
                Q_eigspace(:, j) = lcm_denom * col; % Scale column to make entries integers
            end
        end

        % Append orthonormalized eigenvectors to Q
        Q = [Q, Q_eigspace];
    end

    % Reconstruct D as a diagonal matrix
    D = diag(sortedEigenvalues);
end
