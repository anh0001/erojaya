% cameraparams.m - Philipp Allgeuer - 02/07/13
% Set the required camera parameters (returns a struct)

% Main function
function [p] = cameraparams()

	% Camera resolution
	p.rx = 640;
	p.ry = 480;

	% Camera parameters
	p.fx = 4.3101499868848003e+02;
	p.fy = 4.3101499868848003e+02;
	p.cx = 2.6139884044698385e+02;
	p.cy = 2.0516247546568135e+02;

	% Radial distortion parameters
	p.k1 = -1.4334388480908447e+00;
	p.k2 =  2.0700796519939146e+01;
	p.k3 =  1.8824323641297690e+03;
	p.k4 =  0.;
	p.k5 =  0.;
	p.k6 =  2.0187374017386162e+03;
	
	% Tangential distortion parameters
	p.p1 = -1.6149998446134701e-02;
	p.p2 = -2.6142641942786468e-02;
	
	% Linear distortion extension parameters
	p.uselinext = false; % The next line should set this to true internally
	p = calcparams(p,[-12.0 -8.0 8.0 14.0;-14.0 -8.0 8.0 12.0]);

%  	%
%  	% Old Parameters
%  	%
%  	
%  	% Camera resolution
%  	p.rx = 800;
%  	p.ry = 600;
%  
%  	% Camera parameters
%  	p.fx = 2.9923719209941140e+02;
%  	p.fy = 2.9869074219007530e+02;
%  	p.cx = 3.9217782062880826e+02;
%  	p.cy = 3.0355174547068725e+02;
%  
%  	% Radial distortion parameters
%  	p.k1 = -2.3508879327145624e-01;
%  	p.k2 =  4.0710216140580013e-02;
%  	p.k3 = -2.5554680737527942e-03;
%  	p.k4 = 0;
%  	p.k5 = 0;
%  	p.k6 = 0;
%  	
%  	% Tangential distortion parameters
%  	p.p1 = -1.7019357304546496e-03;
%  	p.p2 =  1.8013653365863906e-03;
%  	
%  	% Disable linear distortion extension
%  	p.uselinext = false;

end
% EOF
