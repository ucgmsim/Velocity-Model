function depth = loadAllSurfaceDepths(fileName)
fid = fopen(fileName);

line = fgetl(fid);
tabLocation = strfind(line,sprintf('\t'));

for i = 1 : length(tabLocation) - 3
    depth.surfName{i} = (line(tabLocation(i+1)+1:tabLocation(i+2)-1));
end
depth.nSurf = i;
count = 0;
while ~feof(fid)
    line = fgetl(fid);
    count = count + 1;
    tabLocation = strfind(line,sprintf('\t'));
    depth.lat(count) = str2double(line(1:tabLocation(1)));
    depth.lon(count) = str2double(line(tabLocation(1):tabLocation(2)));
    for i = 1 : length(tabLocation)-3
        depth.surfDep(i,count) = str2double((line(tabLocation(i+2):tabLocation(i+3))));
    end
    
end


end