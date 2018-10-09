% cameraparams.m - Philipp Allgeuer - 02/07/13
% Set the required camera parameters (returns a struct)

% Main function
function [p] = cameraparams()

	% Camera resolution
	p.rx = 640;
	p.ry = 480;

	% Camera parameters
	p.fx = 4.4287928820709180e+02;
	p.fy = 3.3215946615531885e+02;
	p.cx = 2.6729258501920890e+02;
	p.cy = 1.4019139845541611e+02;

	% Radial distortion parameters
	p.k1 = -1.5962253400132906e-01;
	p.k2 = -8.4070423236496494e-02;
	p.k3 =  0.0;
	p.k4 =  8.5721031130242542e-02;
	p.k5 = -2.5799478136383130e-01;
	p.k6 =  3.6023908238447526e-02;
	
	% Tangential distortion parameters
	p.p1 =  4.4083494214094548e-02;
	p.p2 = -6.7532512590584931e-02;
	
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
