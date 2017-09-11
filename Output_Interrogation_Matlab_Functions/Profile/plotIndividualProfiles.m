function plotIndividualProfiles
close all

% plots the velocity profiles from the CVM individually or as a group

% taylored to suit either one grid point location and different models or
% different grid points for the same model
dirName{1} = 'Multiple_Profiles';
% dirName{2} = 'GeneratedProfile1D';

% dirName{2} = 'GeneratedProfileV0.2';
% dirName{3} = 'GeneratedProfileV0.3';
% dirName{4} = 'GeneratedProfileV0.4';
% dirName{5} = 'GeneratedProfileV0.5';



% dirName{1} = 'GeneratedProfileV0.3';

lineWidth = 2;
fontSize = 18;
legLocation = 'BestOutside';
minMult = 1.1;
maxMult = 1.1;
colorArray = [163 8 12;95 18 12; 160 161 165;255 51 255;204 0 0;0 0 153;0 0 0; ]/255;





F1 = figure(1);
hold on
figureHandle = gcf;
set(findall(gcf,'type','axes'),'fontsize',fontSize)
set(findall(gcf,'type','text'),'fontsize',fontSize)
F2 = figure(2);
hold on 
figureHandle = gcf;
set(findall(gcf,'type','axes'),'fontsize',fontSize)
set(findall(gcf,'type','text'),'fontsize',fontSize)
F3 = figure(3);
hold on
figureHandle = gcf;
set(findall(gcf,'type','axes'),'fontsize',fontSize)
set(findall(gcf,'type','text'),'fontsize',fontSize)


for i = 1 : length(dirName)
    profile = loadIndividualProfile(dirName{i});
    
    figure(1);
    plot(profile.vp,profile.dep,'LineWidth',lineWidth,'Color',colorArray(i,:));
    
    figure(2);
    plot(profile.vs,profile.dep,'LineWidth',lineWidth,'Color',colorArray(i,:));
    
    figure(3);
    plot(profile.rho,profile.dep,'LineWidth',lineWidth,'Color',colorArray(i,:));
    
%     ver(i) = profile.version;
%     lat(i) = profile.lat;
%     lon(i) = profile.lon;
%     minVp(i) = min(profile.vp);
%     maxVp(i) = max(profile.vp);
%     minVs(i) = min(profile.vs);
%     maxVs(i) = max(profile.vs);
%     minRho(i) = min(profile.rho);
%     maxRho(i) = max(profile.rho);
 
end

if length(dirName) ~= 1
    if all(ver == ver(1)) % if all model versions are the same
        titleVp = sprintf('Vp profiles for model version %.1f',ver(1));
        titleVs = sprintf('Vs profiles for model version %.1f',ver(1));
        titleRho = sprintf('Rho profiles for model version %.1f',ver(1));
        
        for i = 1 : length(dirName)
            leg{i} = sprintf('%.2f, %2.f',lat(i),lon(i));
        end
    end
    
    if all(lat == lat(1)) && all(lon == lon(1)) % if all lat and lon points are the same
            titleVp = sprintf('Vp profiles for lat. %.2f, lon. %.2f',lat(1),lon(1));
            titleVs = sprintf('Vs profiles for lat. %.2f, lon. %.2f',lat(1),lon(1));
            titleRho = sprintf('Rho profiles for lat. %.2f, lon. %.2f',lat(1),lon(1));
            
            for i = 1 : length(dirName)
                leg{i} = sprintf('Ver. %.1f',ver(i));
            end
    end
    figure(1);
    vpLeg = legend(leg);
    set(vpLeg, 'location', legLocation);
    figure(2);
    vsLeg = legend(leg);
    set(vsLeg, 'location', legLocation);
    figure(3);
    rhoLeg = legend(leg);
    set(rhoLeg, 'location', legLocation);
   
elseif length(dirName) == 1
    titleVp = sprintf('Vp profile');
    titleVs = sprintf('Vs profile');
    titleRho = sprintf('Rho profile');
    %         for i = 1 : profile.
end
zMin = -0.51;
zMax = 0;
axisVecVp = ([1.6 3.6 zMin zMax]);
axisVecVs = ([0.3 2.0 zMin zMax]);
axisVecRho = ([1.8 2.6 zMin zMax]);


    
figure(1);
title(titleVp);
xlabel('Vp (km/s)');
ylabel('Depth (km)');
axis(axisVecVp);

figure(2);
title(titleVs);
xlabel('Vs (km/s)');
ylabel('Depth (km)');
axis(axisVecVs);

figure(3);
title(titleRho);
xlabel('Rho (T/m^3)');
ylabel('Depth (km)');
axis(axisVecRho);



figure(1);

figure(2);

figure(3);

saveTitleVp = strcat(titleVp);
saveTitleVs = strcat(titleVs);
saveTitleRho = strcat(titleRho);


figure(1);
hold off
saveas(F1,sprintf('%s.png',saveTitleVp),'png');
figure(2);
hold off
saveas(F2,sprintf('%s.png',saveTitleVs),'png');
figure(3);
hold off
saveas(F3,sprintf('%s.png',saveTitleRho),'png');

close all





end