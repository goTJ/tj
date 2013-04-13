function [J, grad] = lrCostFunction(theta, X, y, lambda)
%LRCOSTFUNCTION Compute cost and gradient for logistic regression with 
%regularization
%   J = LRCOSTFUNCTION(theta, X, y, lambda) computes the cost of using
%   theta as the parameter for regularized logistic regression and the
%   gradient of the cost w.r.t. to the parameters. 

% Initialize some useful values
m = length(y); % number of training examples

% You need to return the following variables correctly 
new_theta = [0; theta(2:end)];
h = sigmoid(X * theta);
J = ((-y' * log(h) - (1 - y)' * log(1 - h)) + lambda * sum(new_theta .^ 2) / 2) / m;
grad = ((X' * (h - y)) + (lambda .* new_theta)) ./ m;

end
