function [J, grad] = costFunctionReg(theta, X, y, lambda)
%COSTFUNCTIONREG Compute cost and gradient for logistic regression with regularization
%   J = COSTFUNCTIONREG(theta, X, y, lambda) computes the cost of using
%   theta as the parameter for regularized logistic regression and the
%   gradient of the cost w.r.t. to the parameters. 

% Initialize some useful values
m = length(y); % number of training examples
n = length(theta); % number of features

new_theta = [0; theta(2:n)];
h = sigmoid(X * theta);
J = ((-y' * log(h) - (1 - y)' * log(1 - h)) + lambda * sum(new_theta .^ 2) / 2) / m;
grad = (sum(repmat((h - y), 1, n) .* X) + (lambda .* new_theta')) ./ m;


end
