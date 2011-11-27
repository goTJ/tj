function [J grad] = nnCostFunction(nn_params, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, ...
                                   X, y, lambda)
%NNCOSTFUNCTION Implements the neural network cost function for a two layer
%neural network which performs classification
%   [J grad] = NNCOSTFUNCTON(nn_params, hidden_layer_size, num_labels, ...
%   X, y, lambda) computes the cost and gradient of the neural network. The
%   parameters for the neural network are "unrolled" into the vector
%   nn_params and need to be converted back into the weight matrices. 
% 
%   The returned parameter grad should be a "unrolled" vector of the
%   partial derivatives of the neural network.
%

% Reshape nn_params back into the parameters Theta1 and Theta2, the weight matrices
% for our 2 layer neural network
Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params((1 + (hidden_layer_size * (input_layer_size + 1))):end), ...
                 num_labels, (hidden_layer_size + 1));

% Setup some useful variables
m = size(X, 1);
         
% You need to return the following variables correctly 

X = [ones(m, 1) X];
Y = zeros(size(Theta2, 1), m);
for i = 1:m,
    Y(y(i), i) = 1;
end

z2 = Theta1 * X';
a2 = [ones(1, m); sigmoid(z2)];
a3 = sigmoid(Theta2 * a2);

regTheta2 = [zeros(size(Theta2, 1), 1) Theta2(:, 2:end)];
regTheta1 = [zeros(size(Theta1, 1), 1) Theta1(:, 2:end)];

J = (sum(sum((-Y .* log(a3)) - ((1 - Y) .* log(1 - a3)))) / m) ...
    + ((sum(sum(regTheta1 .^ 2)) + sum(sum(regTheta2 .^ 2))) * lambda / (2 * m));

delta3 = a3 - Y;
delta2 = (Theta2' * delta3)(2:end, :) .* sigmoidGradient(z2);

Theta2_grad = ((delta3 * a2') + lambda .* regTheta2) ./ m;
Theta1_grad = ((delta2 * X) + lambda .* regTheta1) ./ m;

% Unroll gradients
grad = [Theta1_grad(:) ; Theta2_grad(:)];

end
