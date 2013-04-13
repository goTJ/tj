function fv = fv_color(imagename)
% Read an image and return its color features
rgb = imread(imagename);
[nrow, ncolumn, nspace] = size(rgb);
hsv = rgb2hsv(rgb);
frame(:,:) = floor(hsv(:,:,1)*18)*9 + floor(hsv(:,:,2)*3)*3 + floor(hsv(:,:,3)*3);
fv = hist(reshape(frame, 1, nrow*ncolumn), 0:162) ./ (nrow*ncolumn);