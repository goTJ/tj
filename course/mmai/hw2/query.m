typeName = {'Color only', 'Texture only', 'Hybrid'};
topN = 10;

listFid = fopen('list_query.txt', 'r');
htmlFout = fopen('result.html', 'w');
fprintf(htmlFout, '<html><head><title="MMAI HW2 Result"></head><body bgcolor="#ccffcc">\n');

count = 1;
while feof(listFid) == 0
	imagename = fgetl(listFid);
	fprintf('query %s\n', imagename);
	result = im_query(imagename);

	% extract class name
	[token, remain] = strtok(imagename, '/');
	while token
		last = token;
		[token, remain] = strtok(remain, '/');
	end
	query_class_name = strtok(last, '.');

	fprintf(htmlFout, '<hr><p><h1>Query %d: %s <img src="%s" width="160" align="top"></h1>', count, query_class_name, imagename);
	for type = 1:3
        [nrow, nResult] = size(result{type});
		if topN > nResult
			topN = nResult;
		end
		fprintf(htmlFout, '<h3> %s</h3>\n', typeName{type});
		fprintf(htmlFout, '<table border="1" cellspacing="0" cellpadding="5">\n');
		fprintf(htmlFout, '<tr>\n');
		for i = 1:topN
			fprintf(htmlFout, '<th align="center">%d</th>', i);
		end
		fprintf(htmlFout, '</tr>\n');
		fprintf(htmlFout, '<tr>\n');
		for i = 1:topN
			fprintf(htmlFout, '<td><img src="%s" width="80"></td>', result{type}{i});
		end
		fprintf(htmlFout, '</tr>\n');
		fprintf(htmlFout, '<tr>\n');
		for i = 1:topN
			if findstr(query_class_name, result{type}{i})
				fprintf(htmlFout, '<td align="center">O</td>');
			else
				fprintf(htmlFout, '<td align="center">X</td>');
			end
		end
		fprintf(htmlFout, '</tr>\n');
		fprintf(htmlFout, '</table>\n');
		hits = [];
		for i = 1:nResult
			if findstr(query_class_name, result{type}{i})
				hits = [hits i];
			end
		end
		fprintf(htmlFout, '<p>\n');
		fprintf(htmlFout, '<h5>PR Table</h5>\n');
		fprintf(htmlFout, '<table border="1" cellspacing="0" cellpadding="5">\n');
		[nrow, totalHit] = size(hits);

%		fprintf(htmlFout, '<tr>\n');
%		fprintf(htmlFout, '<th>Hits</th>');
%		fprintf(htmlFout, '<td>%d</td>', hits);
%		fprintf(htmlFout, '</tr>\n');
		
		recalls = [1:totalHit] ./ totalHit;
		fprintf(htmlFout, '<tr>\n');
		fprintf(htmlFout, '<th>Recall</th>');
		fprintf(htmlFout, '<td align="center">%.2g</td>', recalls);
		fprintf(htmlFout, '</tr>\n');

		precisions = [1:totalHit] ./ hits;
		fprintf(htmlFout, '<tr>\n');
		fprintf(htmlFout, '<th>Precision</th>');
		fprintf(htmlFout, '<td align="center">%.2g</td>', precisions);
		fprintf(htmlFout, '</tr>\n');

		fprintf(htmlFout, '</table>\n');

		switch type
			case 1
				lineStyle = 'r.-';
			case 2
				lineStyle = 'g.-';
			case 3
				lineStyle = 'b.-';
			otherwise
				lineStyle = 'k.-';
		end
		set(gcf, 'PaperUnits', 'points');
		set(gcf, 'PaperPosition', [0 0 300 240]);
		plot(recalls, precisions, lineStyle);
		hold on;
	end
	[filename, errmsg] = sprintf('%d.png', count);
	title('PR Curve');
	xlabel('Recall');
	ylabel('Precision');
	legend(typeName);
	print('-dpng', filename);
	hold off;
	fprintf(htmlFout, '<p><center><img src="%s"></center><br/>\n', filename);
	fprintf(htmlFout, '<p><br/><br/>\n');
	count = count+1;
end
fprintf(htmlFout, '</body></html>');
fclose(htmlFout);
fclose(listFid);
