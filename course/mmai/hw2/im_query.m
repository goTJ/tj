function ranklist = im_query(imagename)
% query image from dataset fvs.mat & reture the rank list
load fvs;

color_fv = fv_color(imagename);
texture_fv = fv_texture(imagename);

for i = 1:dataSize
	color_dis(i) = fv_sim(color_fv, color_fvs{i}); % color distance
	texture_dis(i) = fv_sim(texture_fv, texture_fvs{i}); % texture distance
end

[scores, ids] = sort(color_dis);
ranklist{1} = {imagenames{ids}};
[scores, ids] = sort(texture_dis);
ranklist{2} = {imagenames{ids}};
[scores, ids] = sort([color_dis' texture_dis']*[p_color; 1-p_color]);
ranklist{3} = {imagenames{ids}};
