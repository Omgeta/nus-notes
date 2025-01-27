function out = ref_btr(M, rref_flag)
    % Perform Gauss-Jordan elimination on matrix M
    % If rref_flag is true, perform Gauss-Jordan elimination.
    % Otherwise, only perform Gaussian Elimination.
    % To prevent numerical imprecision, we cast M into a symbolic.
    M = sym(M);
    
    [r, c] = size(M); % Get the size of the matrix
    curr_row = 1; % Start with the first row

    % Gaussian Elimination
    for col = 1:c
        % Step 1: Locate the leftmost column that does not consist entirely of zeros
        if all(M(curr_row:r, col) == 0)
            continue;
        end
        
        % Step 2: Interchange the top row with another row, if necessary
        % Retrieve the first non-zero row.
        [nonzero_row, ~] = find(M(curr_row:r, c));
        % Get absolute row from relative row of submatrix.
        nonzero_row = nonzero_row + curr_row - 1; 
        if nonzero_row ~= curr_row
            % Perform swap and print the swap that occurred.
            M([curr_row, nonzero_row], :) = M([nonzero_row, curr_row], :);
            fprintf("R%s <-> R%s\n", nonzero_row, curr_row);
            disp(M);
        end
        
        % Step 3: Eliminate entries below the leading entry
        % We... SHOULD NOT need to check for epsilons yet...
        for i = curr_row+1:r
            if M(i, col) ~= 0
                % Kill the row if it is non-zero and print the linear
                % combi.
                coeff = -(M(i, col) / M(curr_row, col));
                M(i, :) = M(i, :) + coeff * M(curr_row, :);
                fprintf("R%s += %sR%d\n", i,  coeff, curr_row);
                disp(M);
            end
        end
        
        % Step 4: Move to the next row and column
        curr_row = curr_row + 1;
        if curr_row > r
            break;
        end
    end

    % If performStepB is false, return the matrix in row-echelon form
    if ~rref_flag
        return;
    end

    % Gauss-Jordan Elimination
    % Step 5: Normalize the leading entries to 1
    for i = 1:r
        leadingIndex = find(M(i, :) ~= 0, 1);
        if ~isempty(leadingIndex) && M(i, leadingIndex) ~= 1
            factor = 1 / M(i, leadingIndex);
            M(i, :) = M(i, :) * factor;
            fprintf("*%sR%d\n", factor, i);
            disp(M);
        end
    end

    % Step 6: Eliminate entries above the leading entries
    for i = r:-1:1
        leadingIndex = find(M(i, :) ~= 0, 1);
        if ~isempty(leadingIndex)
            for j = 1:i-1
                factor = -M(j, leadingIndex);
                M(j, :) = M(j, :) + factor * M(i, :);
                fprintf("R%d += %sR%d\n", j,  factor, i);
                disp(M);
            end
        end
    end
end