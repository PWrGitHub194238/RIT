function generatePlot(plotTitle,xlabelName,ylabelName,dataFileNameArray,styleArray,legendArray,outputName,outputExtension,n)
% Generuje wykresy
% 
% Wejście:
% plotTitle - nazwa wykresu
% xlabelName – etykieta osi pionowej na wykresie
% ylabelName – etykieta osi poziomej na wykresie
% dataFileNameArray - nzwy plików z danymi dla wykresu
% styleArray - style dla każdej funkcji na wykresie
% legendArray - legendy dla funckji
% outputName - nazwa pliku wyjściowego
% outputExtension - rozszerzenie pliku wyjściowego
% n - number of functions on one plot to be drawn from data files
%
% Wyjście:
% plik o nazwie <outputName>.<outputExtension>z wykresami funkcji
% 
% Użycie:
% 	dataArray = {"dataA","dataB"}
% 	styleArray = {"b-","r--"}
% 	legendArray = {"f(x) = x","f(x) = x/2"}
% 	generatePlot('Example','x','y',dataArray,styleArray,legendArray,'out','eps',2)

%
% Tomasz Strzałka, 19 listopada 2014.
if nargin < 8
	help generatePlots 
end

hold on;
grid on;


xlabel(xlabelName);
ylabel(ylabelName);
title(plotTitle);

for index = [1:1:n]
	plotFromFile(dataFileNameArray{index},styleArray{index});
endfor;

legend (legendArray);

print(outputName,strcat("-d",outputExtension));
endfunction;
