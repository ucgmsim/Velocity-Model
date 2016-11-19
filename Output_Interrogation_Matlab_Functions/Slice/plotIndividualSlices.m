function plotIndividualSlices
% plots extracted slices from the CVM
close all


dirName{1} = 'Rapid_Model';
% dirName{2} = '1.11';

% dirName{2} = 'PreQFenceNZAllBasins';
% dirName{1} = 'PreQFenceCantTomoOnly';
% dirName{2} = 'PreQFenceCantAllBasins';
% dirName{2} = 'v1.01ModelRemote';

% dirName{2} = 'v1.22';


% type{1} = 'Generated';
% type{2} = 'Generated';

type{1} = 'Extracted';
type{1} = 'Extracted';


% type{3} = 'Generated';
% type{4} = 'Generated';
% type{5} = 'Generated';
% type{6} = 'Generated';


% dirName{2} = 'VelocityModel1.2';
% dirName{3} = 'VelocityModel1.3';





depthHorizonsVp = [];%[8 7 6 5 4 3 2 1]; %km/s
depthHorizonsVs = [];%[3 4]; %km/s
depthHorizonsRho = [];%[2 2.5 3]; %t/m3

plotElevations = 0;
plotVelocities = 1; % WRITE THIS CODE!!!
plotSurfaceLegend = 0;



fontSize = 18;
lineWidth = 0.5;
lineWidth2 = 2;
lineWidth3 = 1.25;
colorDepth = 1000;


sliceLetter = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'};
colorArray = [0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0; 0 0 0];
colorArrayB = [255 0 0; 255 128 0; 255 255 0; 0  100 0; 128 255 0; 0 255 255; 0 128 255; 0 0 155; 127 0 255; 255 0 255; 240 240 240; 192 192 192;112 112 112; 64 64 64; 0 0 0]/255;





for j = 1 : length(dirName)
    F4 = figure(4);
    hold on
    saveDir = sliceLetter{j};
    mkdir(saveDir);
    sliceParameters = readSliceLogFile(dirName{j},type{j});
    
    for k = 1 : sliceParameters.nSlices
        
        if strcmp(type{j},'Extracted') == 1
            sliceFileName = strcat(dirName{j},'/Extracted_Slices/ExtractedSlice',sprintf('%i',k),'.txt');
        elseif strcmp(type{j},'Generated') == 1
            sliceFileName = strcat(dirName{j},'/Generated_Slices/GeneratedSlice',sprintf('%i',k),'.txt');
        end
        slice = loadSlice(sliceFileName, type{j});

        
        
        F1 = figure(1);
        hold on
        
        F2 = figure(2);
        hold on
        
        F3 = figure(3);
        hold on
        
        %         globalDepFileName = strcat(dirName{j},'/Slices/SliceSurfaceDepthsGlobal',sprintf('%i',k),'.txt');
        %         globDeps = loadAllSurfaceDepths(globalDepFileName);
        %
        %         basinDepFileName = strcat(dirName{j},'/Slices/SliceSurfaceDepthsBasin',sprintf('%i',k),'.txt');
        %         basinDeps = loadAllSurfaceDepths(basinDepFileName);
        
        % decide on which dimension to remove (lat or lon)
        if abs(slice.lonA - slice.lonB) == 0 %zero difference in lon vals
            titleVp = sprintf(' Vp slice from lat. %.2f to %.2f. at lon %.2f. v%.2f',slice.latA,slice.latB,slice.lonA,sliceParameters.version);
            titleVs = sprintf(' Vs slice from lat. %.2f to %.2f. at lon %.2f. v%.2f',slice.latA,slice.latB,slice.lonA,sliceParameters.version);
            titleRho = sprintf(' Rho slice from lat. %.2f to %.2f. at lon %.2f. v%.2f',slice.latA,slice.latB,slice.lonA,sliceParameters.version);
            xLab = 'Longitude';
            
            if plotElevations == 1
                for m = 1 : 3
                    figure(m);
                    for i = 1 : globDeps.nSurf
                        %                     plot(globDeps.lat,globDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                    for i = 1 : basinDeps.nSurf
                        plot(basinDeps.lat,basinDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                end
            end
            
            
            figure(1)
            colormap(jet(colorDepth));
            %         contourf(slice.lat,slice.dep,slice.vp')
            pcolor(slice.lat,slice.dep,slice.vp')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            
            figure(2)
            colormap(jet(colorDepth));
            %         contourf(slice.lat,slice.dep,slice.vs')
            pcolor(slice.lat,slice.dep,slice.vs')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            colormap(jet)
            
            figure(3)
            colormap(jet(colorDepth));
            %         contourf(slice.lat,slice.dep,slice.rho')
            pcolor(slice.lat,slice.dep,slice.rho')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            
            
            nGridX = slice.res + 1;
            gridVecX = zeros (1,nGridX);
            delta = abs(slice.lat(1)-slice.lat(2))/2;
            gridVecX(1) = slice.lat(1) + delta;
            for i = 2 : nGridX
                gridVecX(i) = slice.lat(i-1) - delta;
            end
            
            for i = 1 : length(depthHorizonsVp)
                depthToHorizon = determineDepthToConstantValue(slice,'Vp',depthHorizonsVp(i));
                figure(1);
                plot(slice.lat,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            for i = 1 : length(depthHorizonsVs)
                depthToHorizon = determineDepthToConstantValue(slice,'Vs',depthHorizonsVs(i));
                figure(2);
                plot(slice.lat,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            for i = 1 : length(depthHorizonsRho)
                depthToHorizon = determineDepthToConstantValue(slice,'Rho',depthHorizonsRho(i));
                figure(3);
                plot(slice.lat,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            
            if plotElevations == 1
                for m = 1 : 3
                    figure(m);
                    for i = 1 : globDeps.nSurf
                        %                     plot(globDeps.lat,globDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                    for i = 1 : basinDeps.nSurf
                        plot(basinDeps.lat,basinDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                end
            end
            
            
            
            
        elseif abs(slice.latA - slice.latB) == 0 % zero difference in lat vals
            
            
            
            titleVp = sprintf(' Vp slice from lon. %.2f to %.2f. at lat %.2f v%.2f',slice.lonA,slice.lonB,slice.latA,sliceParameters.version);
            titleVs = sprintf(' Vs slice from lon. %.2f to %.2f. at lat %.2f v%.2f',slice.lonA,slice.lonB,slice.latA,sliceParameters.version);
            titleRho = sprintf(' Rho slice from lon. %.2f to %.2f. at lat %.2f v%.2f',slice.lonA,slice.lonB,slice.latA,sliceParameters.version);
            xLab = 'Longitude';
            if plotElevations == 1
                
                for m = 1 : 3
                    figure(m);
                    for i = 1 : globDeps.nSurf
                        %                     plot(globDeps.lon,globDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                    for i = 1 : basinDeps.nSurf
                        plot(basinDeps.lon,basinDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                end
            end
            figure(1)
            colormap(jet(colorDepth));
            %         contourf(slice.lon,slice.dep,slice.vp')
            pcolor(slice.lon,slice.dep,slice.vp')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            
            figure(2)
            colormap(jet(colorDepth));
            %         contourf(slice.lon,slice.dep,slice.vs')
            pcolor(slice.lon,slice.dep,slice.vs')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            
            figure(3)
            colormap(jet(colorDepth));
            %         contourf(slice.lon,slice.dep,slice.rho')
            pcolor(slice.lon,slice.dep,slice.rho')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            
            nGridX = slice.res + 1;
            gridVecX = zeros (1,nGridX);
            delta = abs(slice.lon(1)-slice.lon(2))/2;
            gridVecX(1) = slice.lon(1) - delta;
            for i = 2 : nGridX
                gridVecX(i) = slice.lon(i-1) + delta;
            end
            for i = 1 : length(depthHorizonsVp)
                depthToHorizon = determineDepthToConstantValue(slice,'Vp',depthHorizonsVp(i));
                figure(1);
                plot(slice.lon,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            for i = 1 : length(depthHorizonsVs)
                depthToHorizon = determineDepthToConstantValue(slice,'Vs',depthHorizonsVs(i));
                figure(2);
                plot(slice.lon,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            for i = 1 : length(depthHorizonsRho)
                depthToHorizon = determineDepthToConstantValue(slice,'Rho',depthHorizonsRho(i));
                figure(3);
                plot(slice.lon,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            
            if plotElevations == 1
                for m = 1 : 3
                    figure(m);
                    for i = 1 : globDeps.nSurf
                        %                     plot(globDeps.lon,globDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                    for i = 1 : basinDeps.nSurf
                        plot(basinDeps.lon,basinDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                end
            end
            
            
            
        elseif (abs(slice.latA - slice.latB) ~= 0) && (abs(slice.lonA - slice.lonB) ~= 0)
            % diagional slice case
            
            titleVp = sprintf(' Vp slice from lon. %.2f to %.2f. lat %.2f to %.2f. v%.2f',slice.lonA,slice.lonB,slice.latA,slice.latB,sliceParameters.version);
            titleVs = sprintf(' Vs slice from lon. %.2f to %.2f. lat %.2f to %.2f. v%.2f',slice.lonA,slice.lonB,slice.latA,slice.latB,sliceParameters.version);
            titleRho = sprintf(' Rho slice from lon. %.2f to %.2f. lat %.2f to %.2f. v%.2f',slice.lonA,slice.lonB,slice.latA,slice.latB,sliceParameters.version);
            xLab = 'Longitude';
            
            if plotElevations == 1
                
                for m = 1 : 3
                    figure(m);
                    for i = 1 : globDeps.nSurf
                        %                     plot(globDeps.lon,globDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                    for i = 1 : basinDeps.nSurf
                        plot(basinDeps.lon,basinDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                end
                
            end
            
            figure(1)
            colormap(jet(colorDepth));
            pcolor(slice.lon,slice.dep,slice.vp')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            
            figure(2)
            colormap(jet(colorDepth));
            pcolor(slice.lon,slice.dep,slice.vs')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            
            figure(3)
            colormap(jet(colorDepth));
            pcolor(slice.lon,slice.dep,slice.rho')%,'CDataMapping','scaled')
            shading flat;
            set(gca,'YDir','normal')
            
            nGridX = slice.res + 1;
            gridVecX = zeros (1,nGridX);
            delta = abs(slice.lon(1)-slice.lon(2))/2;
            gridVecX(1) = slice.lon(1) - delta;
            for i = 2 : nGridX
                gridVecX(i) = slice.lon(i-1) + delta;
            end
            for i = 1 : length(depthHorizonsVp)
                depthToHorizon = determineDepthToConstantValue(slice,'Vp',depthHorizonsVp(i));
                figure(1);
                plot(slice.lon,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            for i = 1 : length(depthHorizonsVs)
                depthToHorizon = determineDepthToConstantValue(slice,'Vs',depthHorizonsVs(i));
                figure(2);
                plot(slice.lon,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            for i = 1 : length(depthHorizonsRho)
                depthToHorizon = determineDepthToConstantValue(slice,'Rho',depthHorizonsRho(i));
                figure(3);
                plot(slice.lon,depthToHorizon,'LineWidth',lineWidth,'Color',[0 0 0]);
            end
            
            
            if plotElevations == 1
                
                for m = 1 : 3
                    figure(m);
                    for i = 1 : globDeps.nSurf
                        %                     plot(globDeps.lon,globDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                    for i = 1 : basinDeps.nSurf
                        plot(basinDeps.lon,basinDeps.surfDep(i,:),'LineWidth',lineWidth2,'Color',colorArray(i,:));
                    end
                end
                
            end
            
        end
        
        
        
        
        
        
        xMax = max(gridVecX);
        xMin = min(gridVecX);
        yMax = slice.zmax;
        yMin = slice.zmin;
        axisLimits = [xMin xMax yMin yMax];
        
        nGridY = length(slice.dep) + 1;
        gridVecY = zeros(1,nGridY);
        delta = abs(slice.dep(1) - slice.dep(2))/2;
        gridVecY(1) = slice.dep(1) + delta;
        
        for i = 2 : nGridY
            gridVecY(i) = slice.dep(i-1) - delta;
        end
        
        
        
        
        figure(1);
        box on
        title(titleVp);
        c1 = colorbar;
        ylabel(c1,'Vp (km/s)');
        set(c1,'YDir','reverse')
        ylabel('Depth (km)')
        xlabel(xLab)
        xt=get(gca,'xtick');
        for kA=1:numel(xt);
            %             xt1{kA}=sprintf('%3.1f°',xt(kA));
        end
        %         set(gca,'xticklabel',xt1);
        figureHandle = gcf;
        %         set(findall(gcf,'type','axes'),'fontsize',fontSize);
        %         set(findall(gcf,'type','text'),'fontsize',fontSize);
        axis(axisLimits)
        box off
        
        
        figure(2);
        box on
        title(titleVs);
        c2 = colorbar;
        vsCmin = 0.5;
        vsCmax = 5.0;
        %         caxis([vsCmin vsCmax])
        set(c2,'YDir','reverse')
        ylabel(c2,'Vs (km/s)');
        ylabel('Depth (km)')
        xlabel(xLab)
        %             ax1 = gca;
        %             set(ax1, 'XColor', [1 1 1]) ;
        %             set(ax1, 'YColor', [1 1 1]) ;
        
        xt=get(gca,'xtick');
        for kA=1:numel(xt);
            %             xt1{kA}=sprintf('%3.1f°',xt(kA));
        end
        %         set(gca,'xticklabel',xt1);
        figureHandle = gcf;
        %             set(findall(gcf,'type','axes'),'fontsize',fontSize);
        %             set(findall(gcf,'type','text'),'fontsize',fontSize);
        axis(axisLimits)
        box off
        
        
        figure(3);
        box on
        title(titleRho);
        c3 = colorbar;
        ylabel(c3,'Rho (t/m^3)');
        set(c3,'YDir','reverse')
        ylabel('Depth (km)')
        xlabel(xLab)
        xt=get(gca,'xtick');
        for kA=1:numel(xt);
            %             xt1{kA}=sprintf('%3.1f°',xt(kA));
        end
        %         set(gca,'xticklabel',xt1);
        figureHandle = gcf;
        %         set(findall(gcf,'type','axes'),'fontsize',fontSize);
        %         set(findall(gcf,'type','text'),'fontsize',fontSize);
        axis(axisLimits)
        box off
        
        if plotSurfaceLegend == 1
            for i = 1 : 2
                figure(i)
                legend(basinDeps.surfName,'Location','WestOutside')
            end
        end
        
        %     for k = 1 : 3
        %         figure(k)
        %         for i = 1 : nGridX
        %             plot([gridVecX(i) gridVecX(i)],[gridVecY(1) gridVecY(end)],'linewidth',lineWidth)
        %         end
        %         for i = 1 : nGridY
        %             plot([gridVecX(1) gridVecX(end)],[gridVecY(i) gridVecY(i)],'linewidth',lineWidth)
        %         end
        %     end
        
        
        
        saveTitleVp = strcat(sliceLetter{k},titleVp,sprintf(' (dep %.0fkm to %.0fkm %s)',slice.zmax,slice.zmin,type{j}));
        saveTitleVs = strcat(sliceLetter{k},titleVs,sprintf(' (dep %.0fkm to %.0fkm %s)',slice.zmax,slice.zmin,type{j}));
        saveTitleRho = strcat(sliceLetter{k},titleRho,sprintf(' (dep %.0fkm to %.0fkm %s)',slice.zmax,slice.zmin,type{j}));
        
        saveTitleVpDir = strcat(saveDir,'/',saveTitleVp);
        saveTitleVsDir = strcat(saveDir,'/',saveTitleVs);
        saveTitleRhoDir = strcat(saveDir,'/',saveTitleRho);
        
        
        figure(1);
        hold off
        saveas(F1,sprintf('%s.png',saveTitleVpDir),'png');
        saveas(F1,sprintf('%s.fig',saveTitleVpDir),'fig');
        
        figure(2);
        hold off
        saveas(F2,sprintf('%s.png',saveTitleVsDir),'png');
        saveas(F2,sprintf('%s.fig',saveTitleVsDir),'fig');
        
        figure(3);
        hold off
        saveas(F3,sprintf('%s.png',saveTitleRhoDir),'png');
        saveas(F3,sprintf('%s.fig',saveTitleRhoDir),'fig');
        
        
        close(F1)
        close(F2)
        close(F3)
        
        
        figure(4)
        plot([slice.lonA slice.lonB],[slice.latA slice.latB],'LineWidth',lineWidth2,'Color',colorArrayB(k,:));
        
        
    end
    
    figure(4)
    legend(sliceLetter);
    [NZcoastLat,NZcoastLong]=NZCoastlineData;
    plot(NZcoastLong,NZcoastLat,'LineWidth',lineWidth3)
%     axis([168 179 -46 -37.5]);
%     xmin xmax ymin yma
    if strcmp(type{j},'Extracted') == 1
        latDom = [sliceParameters.latA  sliceParameters.latB sliceParameters.latC sliceParameters.latD sliceParameters.latA];
        lonDom = [sliceParameters.lonA  sliceParameters.lonB sliceParameters.lonC sliceParameters.lonD sliceParameters.lonA];
        plot(lonDom,latDom,'LineWidth',lineWidth3)
        [latA lonA] = loadBoundary('CanterburyBasinBoundary.txt');
        plot(lonA,latA,'LineWidth',lineWidth3,'Color',[0.5 0.5 0.5]);
        [latA lonA] = loadBoundary('BPVBoundary.txt');
        plot(lonA,latA,'LineWidth',lineWidth3,'Color',[0 0 0]);
    elseif strcmp(type{j},'Generated') == 1
        [latA lonA] = loadBoundary('CanterburyBasinBoundary.txt');
        plot(lonA,latA,'LineWidth',lineWidth3,'Color',[0.5 0.5 0.5]);
        [latB lonB] = loadBoundary('QuaternarySurfacesBoundary.txt');
        plot(lonB,latB,'LineWidth',lineWidth3,'Color',[0 0 0]);
        
    end
    ylabel('Latitude')
    xlabel('Longitude')
    figureHandle = gcf;
    set(findall(gcf,'type','axes'),'fontsize',fontSize);
    set(findall(gcf,'type','text'),'fontsize',fontSize);
    title4 = ('Slice locations');
    title(title4)
    grid on
    hold off
    saveas(F4,sprintf('%s/%s.png',saveDir,title4),'png');
    saveas(F4,sprintf('%s/%s.fig',saveDir,title4),'fig');
    close all
    
end















end

function sliceParameters = readSliceLogFile(dirName,type)


if strcmp(type,'Extracted') == 1
    fileName = strcat(dirName,'/Extracted_Slices/SliceLogFile.txt')
    
    fid = fopen(fileName);
    
    line = fgetl(fid); % desregard first line
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.nSlices = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.version = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.latA = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.latB = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.latC = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.latD = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.lonA = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.lonB = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.lonC = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.lonD = str2double(line(tabLocation(1):end));
    
elseif strcmp(type,'Generated') == 1
        fileName = strcat(dirName,'/Generated_Slices/SliceLogFile.txt')
    
    fid = fopen(fileName);
    
    line = fgetl(fid); % desregard first line
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.nSlices = str2double(line(tabLocation(1):end));
    
    line = fgetl(fid);
    tabLocation = strfind(line,sprintf('\t'));
    sliceParameters.version = str2double(line(tabLocation(1):end));
end

fclose(fid);


end






function depthToHorizon = determineDepthToConstantValue(slice,type,value)
depthToHorizon(1,1) = 0;
if strcmp(type,'Vp') == 1
    depthToHorizon = zeros(1,length(slice.lon));
    for i = 1 : length(slice.lat)
        ind = find(slice.vp(i,:)>value,1); % first indice that exceeds the threshold
        if isempty(ind) == 1
            depthToHorizon(i) = NaN(1,1);
        else
            depthToHorizon(i) = slice.dep(ind);
        end
    end
elseif strcmp(type,'Vs') == 1
    depthToHorizon = zeros(1,length(slice.lon));
    for i = 1 : length(slice.lat)
        ind = find(slice.vs(i,:)>value,1); % first indice that exceeds the threshold
        if isempty(ind) == 1
            depthToHorizon(i) = NaN(1,1);
        else
            depthToHorizon(i) = slice.dep(ind);
        end
    end
elseif strcmp(type,'Rho') == 1
    depthToHorizon = zeros(1,length(slice.lon));
    for i = 1 : length(slice.lat)
        ind = find(slice.rho(i,:)>value,1); % first indice that exceeds the threshold
        if isempty(ind) == 1
            depthToHorizon(i) = NaN(1,1);
        else
            depthToHorizon(i) = slice.dep(ind);
        end
    end
end

end
