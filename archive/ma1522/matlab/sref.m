function [R, E] = ref_symbolic(A)
    % Ensure A is a symbolic matrix
    A = sym(A);
    
    [m, n] = size(A);
    R = A;
    E = eye(m, 'sym');  % Initialize E as a symbolic identity matrix
    row = 1;  % Start from the first row
    
    for col = 1:n
        if row > m
            break;  % Exit if we've processed all rows
        end
        
        % Check if the pivot element is non-zero (simplify to ensure symbolic comparison)
        if simplify(R(row, col)) ~= 0
            % Eliminate below the pivot element in column `col`
            for i = row+1:m
                % Calculate the factor to eliminate the entry
                factor = R(i, col) / R(row, col);  
                
                % Perform row operation: R(i,:) = R(i,:) - factor * R(row,:)
                R(i, :) = R(i, :) - factor * R(row, :);  
                
                % Simplify the current row after the row operation
                R(i, :) = simplify(R(i, :));  % Simplify each row of R
                
                % Update E for the row operation (subtract factor times row)
                E(i, :) = E(i, :) - factor * E(row, :);  % No need for rowOp matrix
            end
            row = row + 1;  % Move to next row only if a non-zero pivot is found
        end
    end
    % Simplify the entire row-reduced matrix R after all operations
    R = simplify(R);
end
