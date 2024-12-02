function Q = gram(A, skipnormalization)
    % Function to compute the Gram-Schmidt process symbolically
    % Handles linear dependence by producing zero columns if necessary.
    % Optionally skips normalization if `skipnormalization` is true.
    % Input:
    %   A - m x n symbolic matrix
    %   skipnormalization - Boolean flag to skip normalization (default: false)
    % Output:
    %   Q - m x n symbolic orthogonal matrix (or zero columns for dependent vectors)
    
    % Ensure input is symbolic
    if ~isa(A, 'sym')
        A = sym(A);
    end
    
    % Set default value for skipnormalization if not provided
    if nargin < 2
        skipnormalization = false;
    end
    
    [m, n] = size(A); % Get dimensions of A
    Q = sym(zeros(m, n)); % Initialize Q symbolically with zeros
    
    for j = 1:n
        % Start with the current column of A
        v = A(:, j);
        
        % Subtract projections onto previous vectors in Q
        for i = 1:j-1
            v = v - (dot(Q(:, i), A(:, j)) / dot(Q(:, i), Q(:, i))) * Q(:, i);
        end
        
        % Normalize v to make it a unit vector, or leave as zero if dependent
        norm_v = sqrt(dot(v, v));
        if norm_v ~= 0  % Check for linear dependence
            if skipnormalization
                Q(:, j) = v;  % Skip normalization
            else
                Q(:, j) = v / norm_v;  % Normalize
            end
        else
            Q(:, j) = sym(zeros(m, 1)); % Column is dependent, so set to zero
        end
    end
end