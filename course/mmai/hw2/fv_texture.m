function fv = fv_texture(imagename)
% Read an image and return its texture features
im = rgb2gray(imread(imagename));
[nrow ncol] = size(im);
nPixel = nrow*ncol;
nscale = 4;
norient = 6;
EO = gaborconvolve(im, nscale, norient, 3, 2, 0.65, 1.5, 0);
count = 1;
for o = 1:norient
    for s = 1:nscale
        m = mean(abs(reshape(EO{s,o}, 1, nPixel)));
        v = sqrt(var(abs(reshape(EO{s,o}, 1, nPixel))));
        fv(count) = m/40.0;
        fv((count+1)) = v/40.0;
        count = count+2;
    end
end
