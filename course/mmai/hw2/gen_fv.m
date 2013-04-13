clear

% build feature dataset
listFid = fopen('list.txt', 'r');
dataSize = 1;
while feof(listFid) == 0
	imagenames{dataSize} = fgetl(listFid);
	disp(imagenames{dataSize});
	color_fvs{dataSize} = fv_color(imagenames{dataSize});
	texture_fvs{dataSize} = fv_texture(imagenames{dataSize});
	dataSize = dataSize+1;
end
dataSize = dataSize-1;
fclose(listFid);

% optimize p for p*c+(1-p)*t by leave-one-out
maxv = -1;
for p = 0:0.02:1
	count = 0;
	for q = 1:dataSize
%		fprintf('query: %s\n', imagenames{q});

		% extract class name
		[token, remain] = strtok(imagenames{q}, '/');
		last = {'', ''};
		while token
			last = {last{2}, token};
			[token, remain] = strtok(remain, '/');
		end
		class_name = last{1};

		color_fv = color_fvs{q};
		texture_fv = texture_fvs{q};
		for i = 1:dataSize
			color_dis(i) = fv_sim(color_fv, color_fvs{i}); % color distance
			texture_dis(i) = fv_sim(texture_fv, texture_fvs{i}); % texture distance
		end
		[scores, ids] = sort([color_dis' texture_dis']*[p; 1-p]);
%		fprintf('%s\n', imagenames{ids(1:9)});
		result = {imagenames{ids(1:9)}};
		for i = 1:9
			if findstr(class_name, result{i})
				count = count+1;
			end
		end
	end
	fprintf('p: %.3f, count: %d\n', p, count);
	if count > maxv
		maxv = count;
		p_color = p;
	end
end

save ('fvs', 'dataSize', 'imagenames', 'color_fvs', 'texture_fvs', 'p_color');
