function idx = findClosestCentroids(X, centroids)
%FINDCLOSESTCENTROIDS computes the centroid memberships for every example
%   idx = FINDCLOSESTCENTROIDS (X, centroids) returns the closest centroids
%   in idx for a dataset X where each row is a single example. idx = m x 1 
%   vector of centroid assignments (i.e. each entry in range [1..K])
%

% Set K
K = size(centroids, 1);

% You need to return the following variables correctly.
idx = zeros(size(X,1), 1);

for x_idx = 1:size(X, 1)
  x = X(x_idx, :);
  min_idx = 1;
  d = x - centroids(1, :);
  min_value = d * d';
  for c_idx = 2:K
    d = x - centroids(c_idx, :);
    value = d * d';
    if value < min_value
      min_idx = c_idx;
      min_value = value;
    end
  end
  idx(x_idx) = min_idx;
end

end

