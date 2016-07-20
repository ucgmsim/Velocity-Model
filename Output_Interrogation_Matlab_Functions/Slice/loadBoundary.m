
function [lat lon] = loadBoundary(fileName)

fid = fopen(fileName);
count = 1;


while ~feof(fid)
    
    line = fgetl(fid);
    tabLocations =strfind(line,sprintf('\t'));
    lon(count) = str2double(line(1:tabLocations(1)));
    lat(count) = str2double(line(tabLocations(1):end));
    count = count +1;
end
fclose(fid);



end