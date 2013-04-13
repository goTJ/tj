function [C, sigma] = dataset3Params(X, y, Xval, yval)
%EX6PARAMS returns your choice of C and sigma for Part 3 of the exercise
%where you select the optimal (C, sigma) learning parameters to use for SVM
%with RBF kernel
%   [C, sigma] = EX6PARAMS(X, y, Xval, yval) returns your choice of C and 
%   sigma. You should complete this function to return the optimal C and 
%   sigma based on a cross-validation set.
%

C_vec = [0.01 0.03 0.1 0.3 1 3 10 30]';
sigma_vec = [0.01 0.03 0.1 0.3 1 3 10 30]';

C = -1;
min_cost = 0;

for C_idx = 1:length(C_vec)
  for sigma_idx = 1:length(sigma_vec)
    model= svmTrain(X, y, C_vec(C_idx), @(x1, x2) gaussianKernel(x1, x2, sigma_vec(sigma_idx)));
    predictions = svmPredict(model, Xval);
    cost = mean(double(predictions ~= yval));
    if C == -1 || cost < min_cost
      min_cost = cost;
      C = C_vec(C_idx);
      sigma = sigma_vec(sigma_idx);
    end
  end
end

end
