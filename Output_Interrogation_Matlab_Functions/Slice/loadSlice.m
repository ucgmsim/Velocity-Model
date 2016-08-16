function slice = loadSlice(fileName,type)
% reads the files within the profile directory and places the data within a
% struct 
fid = fopen(fileName);

temp = fgetl(fid); % disregard first line

line = fgetl(fid);
tabLocation = strfind(line,sprintf('\t'));
slice.res = str2double(line(tabLocation(1):end));

if strcmp(type,'Generated') == 1
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    slice.resZ = str2double(line(tabLocation(1):end));
end

% lat
line = fgetl(fid);
tabLocation = strfind(line,sprintf('\t'));
slice.latA = str2double(line(tabLocation(1):end));
line = fgetl(fid);
tabLocation = strfind(line,sprintf('\t'));
slice.latB = str2double(line(tabLocation(1):end));

% lon
line = fgetl(fid);
tabLocation = strfind(line,sprintf('\t'));
slice.lonA = str2double(line(tabLocation(1):end));
line = fgetl(fid);
tabLocation = strfind(line,sprintf('\t'));
slice.lonB = str2double(line(tabLocation(1):end));



line = fgetl(fid); % header line

% read primary data within file
count = 0;
while ~feof(fid)
    line = fgetl(fid);
    count = count + 1;
end

% pre allocation
tenPercent = round(count/10);
lon = zeros(1,count);
lat = zeros(1,count);
dep = zeros(1,count);
vp = zeros(1,count);
vs = zeros(1,count);
rho = zeros(1,count);

fclose(fid);

fid = fopen(fileName);
for i = 1:7
temp = fgetl(fid); % disregard first 6 lines
end
if strcmp(type,'Generated') == 1
    temp = fgetl(fid); % disregard first 6 lines
end

for i = 1 : count
    line = fgetl(fid);
    lineDouble = sscanf(line,'%lf\t%lf\t%lf\t%lf\t%lf\t%lf');
    lat(i) = lineDouble(1);
    lon(i) = lineDouble(2);
    dep(i) = lineDouble(3)/1000;
    vp(i) = lineDouble(4);
    vs(i) = lineDouble(5);
    rho(i) = lineDouble(6);
    if rem(i,tenPercent) == 0
        fprintf('File read %i percent complete.\n',10*i/tenPercent);
    end
end

nDepPts = length(unique(dep));

slice.lon = zeros(slice.res,1);
slice.lat = zeros(slice.res,1);
slice.dep = zeros(nDepPts,1);
slice.vp = zeros(slice.res,nDepPts);
slice.vs = zeros(slice.res,nDepPts);
slice.rho = zeros(slice.res,nDepPts);

fclose(fid);
fprintf('File read 100 percent complete.\n');

for i = 1 : slice.res
    slice.lat(i) = lat((i-1)*nDepPts+1);
    slice.lon(i) = lon((i-1)*nDepPts+1);
end
for j = 1 : nDepPts
    slice.dep(j) = dep(j);
end

for i = 1 : slice.res
    for j = 1 : nDepPts
        ind = (i-1)*nDepPts+j;
        slice.vp(i,j) = vp(ind);
        slice.vs(i,j) = vs(ind);
        slice.rho(i,j) = rho(ind);
    end
end

slice.zmin = min(slice.dep); % in km
slice.zmax = max(slice.dep);


end