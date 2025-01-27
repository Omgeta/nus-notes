function [L, U] = luc(A)
    % LU: Computes the LU decomposition of a matrix A using row echelon form.
    % Inputs:
    %   A - A symbolic or numeric matrix
    % Outputs:
    %   L - Lower triangular matrix (product of inverse of row operations)
    %   U - Upper triangular matrix (row echelon form)
    
    % Ensure input is symbolic if required
    if ~isa(A, 'sym')
        A = sym(A);  % Convert numeric matrix to symbolic if necessary
    end

    % Run rowEchelonSymbolic (sref) to get the row echelon form and the elementary matrix
    [R, E] = sref(A);
    
    % U is the row echelon form (R)
    U = R;

    % L is the inverse of the elementary matrix (E)
    L = inv(sym(E));  % Compute the inverse of the elementary matrix
end