%----------------------Material Constants----------------------%
%c = 921;                %J/(kgK) Specific heat capacity of aluminium
%k = 235;                %W/(mK)  Conductivity               ""
%k/c theoretical = 0.255
%-------At k = 280 and c = 1000 1st one is not bad-------------%
c = 921;
k = 180;
k_c = k/c;
p = 2700;               %kg/m^3  Density                    ""
kc = 10;                %W/m^2K  Conduction Constant        ""
C = k/(c*p);            %Thermal Diffusivity Constant       ""
e = 0.60;                %Emissivity                         ""
sig = 5.67*10^(-8);     %Boltzman Constant

%---------------------Material Dimentions----------------------%
l = 0.305;              %Length of rod (m)
r = 0.0125;             %Radius of rod (m)
area = r*r*pi();        %Area (m^2)

%--------------------Environment Constants---------------------%
Tamb = 293;             %Ambiant Tempurature
TempL = 19.34+273;      %Tempurature of far left segement        SET BASED ON CONDITIONS OF EXPERIMENT
TempR = 18.85+273;      %Tempurature of far right segment       SET BASED ON CONDITIONS OF EXPERIMENT
Pin = 8.25;             %Power in

%---------------Numerical Calculation Constants----------------%
dx = 0.01;              %Thickness of step (mm)
dt = 0.1;               %Time step (ms)
N = floor(l/dx);        %Number of segments
runTime = 7200;          %Run time (s)
startTime = 6.5;
step = 0;               %Number of loop run times
elapsedTime = 0;        %Total elapsed time

%--------------------Experiment Constants----------------------%
sensors = 5;            %Number of sensors
spacing = [floor(0.0125/dx) floor(0.0825/dx) floor(0.1525/dx) floor(0.2225/dx) floor(0.2925/dx)];
                        %Location of sensors along rod (based on dx)
period = 700;           %Total period of switch (s)

%-------------------------Array Setup--------------------------%
sensTemp = zeros(sensors,runTime/dt-startTime/dt );       %Sensor Tempurature in Time
time = linspace(startTime/dt,runTime/dt+1,runTime/dt-startTime/dt+1); %Plot time dimention
delta = zeros(1,N+1);                       %Tempurature changes array
distance = linspace(0,N,N+1);               %Plot length dimention
peaks = [0 0 0 0 0];
peaksTime = [0 0 0 0 0];
peakFlag = [0 0 0 0 0];

%----------------------Initial Tempurature---------------------%
rod_temp = linspace(TempL,TempR,N+1);

%-------------------------Heat Transfer------------------------%
for i = startTime:dt:runTime
    
    %-------------------------Powered Segment------------------------%
    flag = sign(sin(2*pi*elapsedTime/period));
    if  flag > 0.5
        powerIn = Pin*dt/(c*p*pi*r^2*dx);                                                  %Power in
    elseif flag < 0.5
        powerIn = 0;
    end
    
    adjacencyChange = -1*(C*dt*(rod_temp(1)-rod_temp(2)))/(dx^2);                          %Power lost to adjecent segment
    convectiveLoss = -1*(2*pi*r*dx+pi*r^2)*kc*(rod_temp(1)-Tamb)*dt/(c*p*pi*r^2*dx);       %Convective loss
    radiativeLoss = -1*(2*pi*r*dx+pi*r^2)*e*sig*(rod_temp(1)^4-Tamb^4)*dt/(c*p*pi*r^2*dx); %Raditive loss
    
    delta(1) = powerIn + adjacencyChange + convectiveLoss + radiativeLoss;                 %Record all changes
    
    %-------------------------Middle Segments------------------------%
    for j = 2:1:N-1
        
        adjacencyChange = (C*dt)*(rod_temp(j-1)-2*rod_temp(j)+rod_temp(j+1))/(dx^2);       %Power lost to adjecent segment
        convectiveLoss = -2*kc*(rod_temp(j)-Tamb)*dt/(c*p*r);                              %Convective loss
        radiativeLoss = -2*pi*r*dx*e*sig*(rod_temp(j)^4-Tamb^4)*dt/(c*p*pi*r^2*dx);        %Raditive loss
        
        delta(j) = adjacencyChange + convectiveLoss + radiativeLoss;                       %Record all changes
        
    end
    
    %--------------------------Final Segment-------------------------%
    adjacencyChange = (C*dt*(rod_temp(N-1)-rod_temp(N)))/(dx*dx);                          %Last Slice change
    convectiveLoss = (-2*pi*r*dx+pi*r^2)*kc*(rod_temp(N)-Tamb)*dt/(c*p*pi*r^2*dx);         %Convective loss
    radiativeLoss = (-2*pi*r*dx+pi*r^2)*e*sig*(rod_temp(N)^4-Tamb^4)*dt/(c*p*pi*r^2*dx);   %Raditive loss
    
    delta(N) = adjacencyChange + convectiveLoss + radiativeLoss;                           %Record all changes
    
    %--------------------Adjust Tempurature of Rod-------------------%
    rod_temp = rod_temp + delta;
    
    %------------------------Increment Values------------------------%
    step = step + 1;
    elapsedTime = elapsedTime + dt  
    
    %------------------------Increment Values------------------------%
    for k = 1:1:5
        sensTemp(k,step) = rod_temp(spacing(k));
%         if elapsedTime > 342
%             ad = 0;
%         end
%         
%         if elapsedTime > 100
%             if peakFlag(k) == 0 && sensTemp(k,step-1) <= sensTemp(k,step)
%                 peaks(k) = sensTemp(k,step-1)-273;
%                 peaksTime(k) = elapsedTime;
%             else
%                 peakFlag(k) = 1;
%             end
%         end
        
    end
    
%     for w = 1:1:5
%         plot(distance,rod_temp-273,'*');
%     end
%     xlim([0 31])
%     ylim([15 80])
%     pause(0.00001);
end
% 
plot(time*dt, sensTemp(1,:)-273, 'r');
hold on
plot(time*dt, sensTemp(2,:)-273, 'm');
plot(time*dt, sensTemp(3,:)-273, 'g');
plot(time*dt, sensTemp(4,:)-273, 'y');
plot(time*dt, sensTemp(5,:)-273, 'b');
 
test = csvread('black-horizontal-period700s-duration2h-power15w.txt',1,0);
plot(test(:,1)/1000, test(:,2), 'r');
hold on
plot(test(:,1)/1000, test(:,3), 'm');
plot(test(:,1)/1000, test(:,4), 'g');
plot(test(:,1)/1000, test(:,5), 'y');
plot(test(:,1)/1000, test(:,6), 'b');

xlabel('Time (s)')
ylabel('Tempurature (C)')
title('Horizontal Painted, 15W')
legend('S1','S2','S3','S4','S5','Location','southeast');


    
 