function result = mytest(data)
  %remove noise from transmitter
  data.beam(1,1:30) = zeros(1,30);

  %global variables 
  global qual;
  global left_side = 0;
  global right_side = 0;
  global scan_left = 0;
  global last_bearing = 0;
  global min_bearing = 0;
  global max_bearing = 0;  
  global scan_quality = 0;
  global left_counter = 0;
  global right_counter = 0;
  global last_beam_qual = 0;
  global ftt_plot;
  global plot_val = zeros(1,60);
  direction_change = 0;    % indicates that we have a direction change !!!

  %check if scan direction has changed
  if(last_bearing <= data.bearing.rad)
    if(scan_left == 0)
        scan_left = 1;
        direction_change = 1;
        min_bearing = last_bearing;
    endif
  else
    if(scan_left == 1)
        scan_left = 0;
        direction_change = 1;
        max_bearing = last_bearing;
    endif
  endif
  last_bearing = data.bearing.rad;

  %calculate center
  center = min_bearing + (max_bearing-min_bearing)*0.5;

  %%if scan direction has changed
  if(direction_change == 1)
    %  right_side/right_counter-left_side/left_counter

%      qual(length(qual)+1) = scan_quality;
      scan_quality = 0;
      left_side = 0;
      right_side = 0;
      left_counter = 0;
      right_counter = 0;

      %we have to flip the vector because the order depends on the scan direction
      if(scan_left == 1)
          qual = fliplr(qual);
      endif

     % qual = conv(qual,ones(1,3)/3);
    
      plot_val = shift(plot_val,-length(qual));
      plot_val(length(plot_val)-length(qual)+1:length(plot_val))=qual;

      figure(1)

      clear -g qual;
      global qual

      temp = conv(plot_val,ones(1,8)/8);
      plot(temp)

      figure(2)
      L  = length(plot_val);
      NFFT = 2^nextpow2(L)*4; % Next power of 2 from length of y
      Y = fft(temp,NFFT)/L;
      f = length(plot_val)/2*linspace(0,1,NFFT/2+1);

      ftt_plot(length(ftt_plot)+1) =  max(2*abs(Y(6:NFFT/2)));
      plot(ftt_plot)
    %  plot(f,2*abs(Y(1:NFFT/2+1))) 
 %     axis([0 10 0 0.2]);
  endif

  a = cumsum(data.beam);

  %calc quality
  b = [length(data.beam):-1:1];
  b = (b.*b/90000).^2;

  %compensate angle error 
  beam_quality = sum(data.beam.*data.beam.*b/255/255);%*(cos(center-data.bearing.rad))^2;
  scan_quality += (beam_quality-last_beam_qual)^2;
  qual(length(qual)+1) = beam_quality;
  last_beam_qual = beam_quality;

  %calculate distance value
  if(data.bearing.rad > center)
      right_counter+=1;
      right_side += beam_quality*(data.bearing.rad-center);
  else
      left_counter+=1;
      left_side += beam_quality*(center-data.bearing.rad);
  endif

  sleep(0.01)
  result = data;
endfunction
