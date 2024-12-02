function colBasis = col(A)
    % Find the row echelon form (REF) of A and the pivot columns
    [R, ~] = sref(A);  % Use sref to get the row echelon form and elementary row operations
    
    [m, n] = size(R);  % Get the size of the matrix
    pivotCols = [];     % Initialize an empty array for pivot column indices
    
    % Loop through each row and find the first non-zero entry (the pivot)
    for i = 1:m
        for j = 1:n
            if R(i, j) ~= 0  % If the entry is non-zero
                pivotCols = [pivotCols, j];  % Add the column index to pivotCols
                break;  % Move to the next row after finding the pivot
            end
        end
    end
    
    % Ensure unique pivot columns
    pivotCols = unique(pivotCols);
    
    % Basis for the column space: the columns in the original matrix A
    % corresponding to the pivot columns in R
    colBasis = A(:, pivotCols);
end
