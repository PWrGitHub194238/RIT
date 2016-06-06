function plotFromFile(dateFileName,lineStyle)
% Rysuje funkcje na podstawie pliku z danymi.
% 
% Wejście:
% dateFileName - nazwa pliku wejściowego
% lineStyle - styl linii np. "b--"
%
% Wyjście:
% 
% Użycie:
%
% Jest to prywatna funkcja generatePlots.m.
% plotFromFile('input','title','b-')
%
% Tomasz Strzałka, 19 listopada 2014.
if nargin < 2
	help plotFromFile 
end

[X,Y] = textread (dateFileName,'%f %f')
plot(X,Y,lineStyle);
endfunction;
