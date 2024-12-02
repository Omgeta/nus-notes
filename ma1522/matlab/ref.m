function [R, E] = ref(A)
    [m, n] = size(A);
    R = A;
    E = eye(m);  % Initialize E as the identity matrix (no row operations yet)
    row = 1;  % Start from the first row
    for col = 1:n
        if row > m
            break;  % Exit if we've processed all rows
        end

        if R(row, col) ~= 0
            % Eliminate below the pivot element in column `col`
            for i = row+1:m
                factor = R(i, col) / R(row, col);  % Calculate the factor to eliminate the entry
                R(i, :) = R(i, :) - factor * R(row, :);  % Subtract the factor multiplied by the pivot row

                % Update E for the row operation (subtract factor times row)
                rowOp = eye(m);
                rowOp(i, row) = -factor;  % This is the elementary row operation matrix
                E = E * rowOp;  % Apply the row operation to the total transformation matrix
            end
            row = row + 1;  % Move to next row only if a non-zero pivot is found
        end
    end
end
