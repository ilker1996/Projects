function weight = weight(x)

	if x < 0 ,
		weight = 0;
	elseif  0 < x && x <= 1 ,
	    weight = (0.4)*x^3 - (0.6)*x^2 +0.3;
	elseif 1 < x && x <= 2 ,
	    weight = (-1.2)*(x-1)^3 + (1.8)*(x-1)^2 + 0.1;
	elseif 2 < x  && x <= 3 ,
	    weight = (1.2)*(x-2)^3 - (1.8)*(x-2) ^2 + 0.7;
	else ,
	    weight = 0;
end;
endfunction
